#include <sys/socket.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "sidewinder.h"

static char PING = 'a';
static struct iovec IOV = {
    .iov_base = &PING,
    .iov_len = sizeof(PING),
};

int
sendsocket(int channel, int sockfd) {
    struct msghdr msg = {0};
    struct cmsghdr *cmsg;
    /* Contains the file descriptors to pass. */
    int myfds[1];
    /* ancillary data buffer */
    char buf[CMSG_SPACE(sizeof myfds)];
    int *fdptr;
    myfds[0] = sockfd;
    msg.msg_control = buf;
    msg.msg_controllen = sizeof buf;
    msg.msg_iov = &IOV;
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
    return sendmsg(channel, &msg, 0);
}

int
recvsocket(int channel, sw_status_t *status) {
    struct cmsghdr *cmsg;
    ssize_t result;
    char io_buf;
    /* struct iovec iov; */
    /* we are only receiving 1 byte of data. */
    status->iov.iov_base = &io_buf;
    status->iov.iov_len = 1;
    status->msg.msg_iov = &status->iov;
    status->msg.msg_iovlen = 1;
    status->msg.msg_control = status->buf;
    status->msg.msg_controllen = sizeof(status->buf);
    /* 0 means shutdown */
    if ((result = recvmsg(channel, &status->msg, 0)) <= 0) {
        return result;
    }
    if (status->msg.msg_controllen >= sizeof(int)) {
        cmsg = CMSG_FIRSTHDR(&status->msg);
        return *((int *)CMSG_DATA(cmsg));
    }
    return -1;
}
