#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "sidewinder.h"

int
sendsocket(int channel, int sockfd) {
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    /* Contains the file descriptors to pass. */
    int myfds[1];
    /* ancillary data buffer */
    char buf[CMSG_SPACE(sizeof myfds)];
    int *fdptr;
    size_t result;
    char ping = 'a';
    struct iovec iov = {
        .iov_base = &ping,
        .iov_len = sizeof ping,
    };
    myfds[0] = sockfd;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof buf;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    cmsg = CMSG_FIRSTHDR(&msg);
    cmsg->cmsg_level = SOL_SOCKET;
    cmsg->cmsg_type = SCM_RIGHTS;
    cmsg->cmsg_len = CMSG_LEN(sizeof(int));
    /* Initialize the payload: */
    fdptr = (int *) CMSG_DATA(cmsg);
    memcpy(fdptr, myfds, sizeof(int));
    /* Sum of the length of all control messages in the buffer: */
    msg.msg_controllen = cmsg->cmsg_len;
    if ((result = sendmsg(channel, &msg, 0)) < 0) {
        perror("sendmsg");
        return -1;
    }
    return result;
}

int
recvsocket(int channel) {
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    ssize_t result;
    char io_buf;
    char buf[CMSG_SPACE(sizeof (int *)) * 2];
    struct iovec iov;
    /* we are only receiving 1 byte of data. */
    iov.iov_base = &io_buf;
    iov.iov_len = 1;
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof buf;
    if ((result = recvmsg(channel, &msg, 0)) < 0) {
        perror("recvmsg");
        return result;
    }
    if (result == 0) {
        return 0;
    }
    cmsg = CMSG_FIRSTHDR(&msg);
    return *((int *)CMSG_DATA(cmsg));
}
