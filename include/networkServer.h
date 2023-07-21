#ifndef _NETWORKSERVER_H
#define _NETWORKSERVER_H

#include <networkPthreadPool.h>

// 线程参数传递
typedef struct pthreadARG {
    taskQueuePool *tqp;
    taskTreePool *ttp;
    int epfd;
} pthreadARG;

void server_start(void *(*processMes)(void *));

#endif

/*
 *  void server_start(const short unsigned int PORT, const char *IP, const unsigned int PTHREAD_MAX, const unsigned int EPOLL_SIZE, int (*processMes)(int fd));
 *      新建一个在IP:PORT上的监听,其中使用了线程池(PTHREAD_MAX为线程个数)和EPOLL(EPOLL_SIZE为EPOLL最大监听个数)
 */
