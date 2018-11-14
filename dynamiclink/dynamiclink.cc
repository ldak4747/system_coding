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

//static int count_malloc = 0;
//static int count_free = 0;
//typedef void *(*myMalloc)(size_t);
//static myMalloc g_sys_malloc_func = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");//g_sys_malloc_func == read system malloc
 

/*
void *malloc(size_t size) {
    //printf("first, %p\n", g_sys_malloc_func);
    void *(*g_sys_malloc_func)(size_t) = (void *(*)(size_t))dlsym(RTLD_NEXT, "malloc");
    //printf("then, %p\n", g_sys_malloc_func);
    count_malloc++;
    printf("call my malloc, %d\n", count_malloc);
    //return myMalloc(size);
    //void *ptr = g_sys_malloc_func(size);
    //printf("ptr: %p\n", ptr);
    //return ptr;
    return nullptr;
}

void free (void *ptr) {
    void (*g_sys_free_func)(void *ptr) = (void (*)(void *))dlsym(RTLD_NEXT, "free");
    count_free++;
    printf("call my free, %d\n", count_free);
    g_sys_free_func(ptr);
}
*/
