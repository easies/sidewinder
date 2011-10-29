#ifndef _SIDEWINDER_H_
#define _SIDEWINDER_H_

#include <sys/socket.h>
#include <sys/types.h>

typedef struct sw_status {
    struct msghdr msg;
    struct iovec iov;
    char buf[CMSG_SPACE(sizeof(int *))];
} sw_status_t;

int
sendsocket(const int channel, const int sockfd);

int
recvsocket(const int channel, sw_status_t *status);

#endif
