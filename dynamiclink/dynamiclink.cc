#include <stdlib.h>
#include <dlfcn.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
 
int connect (int sockfd, const struct sockaddr *serveraddr, socklen_t len) {
    printf("call my connet\n");
    int (*g_sys_connect_func)(int, const struct sockaddr *, socklen_t) = (int (*)(int, const struct sockaddr *, socklen_t))dlsym(RTLD_NEXT, "connect");

    int res = g_sys_connect_func(sockfd, serveraddr, len);
    printf("res: %d\n", res);
    return res;
}

