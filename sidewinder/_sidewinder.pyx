import os

cdef extern from "errno.h":
    int errno

cdef extern from "sys/types.h":
    ctypedef int size_t
    ctypedef int ssize_t
    ctypedef unsigned long int socklen_t
    struct iovec:
        void  *iov_base
        size_t iov_len
    struct msghdr:
        void       *msg_name
        socklen_t   msg_namelen
        iovec      *msg_iov
        size_t      msg_iovlen
        void       *msg_control
        size_t      msg_controllen
        int         msg_flags
    struct cmsghdr:
        socklen_t   cmsg_len
        int         cmsg_level
        int         cmsg_type
        # followed by unsigned char[]

cdef extern from "sys/socket.h":
    int socketpair(int domain, int type, int protocol, int sv[2])
    ssize_t sendmsg(int sockfd, msghdr *msg, int flags)
    ssize_t recvmsg(int sockfd, msghdr *msg, int flags)
    cmsghdr *CMSG_FIRSTHDR(msghdr *msgh)
    cmsghdr *CMSG_NXTHDR(msghdr *msgh, cmsghdr *cmsg)
    size_t CMSG_ALIGN(size_t length)
    size_t CMSG_SPACE(size_t length)
    size_t CMSG_LEN(size_t length)
    unsigned char *CMSG_DATA(cmsghdr *cmsg)
    int SOL_SOCKET
    int SCM_RIGHTS
    int AF_UNIX
    int SOCK_STREAM

cdef extern from "string.h":
    void memcpy(void *dest, void *src, size_t n)

cdef extern from "stdlib.h":
    void *malloc(size_t size)
    void free(void *)

cdef extern from "stdio.h":
    int printf(char *format, ...)
    void perror(char *)

cdef extern from "sidewinder.h":
    int c_sendsocket "sendsocket" (int channel, int sockfd)
    int c_recvsocket "recvsocket" (int channel)

def get_fd(fd):
    if hasattr(fd, 'fileno'):
        return fd.fileno()
    return fd

def sendsocket(fd, sockfd):
    if c_sendsocket(get_fd(fd), get_fd(sockfd)) < 0:
        raise IOError(errno, os.strerror(errno))

def recvsocket(fd):
    result = c_recvsocket(get_fd(fd))
    if result < 0:
        raise IOError(errno, os.strerror(errno))
    if result == 0:
        raise IOError(errno, os.strerror(errno))
    return result
