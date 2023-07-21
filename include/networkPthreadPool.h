#ifndef _NETWORKPTHREADPOOL_H
#define _NETWORKPTHREADPOOL_H

#include <pthread.h>
#include <coreQueue.h>
#include <coreTree.h>
#include <networkHttpHeader.h>

// 任务池队列节点值
typedef struct taskQueuePoolValue {
    int clientFd;
} taskQueuePoolValue;

taskQueuePoolValue *taskQueuePoolValue_init(int clientFd);
void taskQueuePoolValue_print(taskQueuePoolValue *tqpv);
void taskQueuePoolValue_free(taskQueuePoolValue *tqpv);

// 任务队列池
typedef struct taskQueuePool {
    Queue *n_queue;
    pthread_mutex_t n_lock;
    pthread_cond_t n_haveTask;
} taskQueuePool;

taskQueuePool *taskQueuePool_init();
int taskQueuePool_push(taskQueuePool *tqp, taskQueuePoolValue *tqpv);
taskQueuePoolValue *taskQueuePool_pop(taskQueuePool *tqp);
void taskQueuePool_display(taskQueuePool *tqp); 
void taskQueuePool_free(taskQueuePool *tqp);

// 任务树节点值
typedef struct taskTreePoolValue {
    int clientFd;
    HttpHeader *hh;
    int fileFd;
    int fileIndex;
} taskTreePoolValue;

taskTreePoolValue *taskTreePoolValue_init(int clientFd, HttpHeader *hh, int fileFd, int fileIndex);
void taskTreePoolValue_print(taskQueuePoolValue *tqpv);
void taskTreePoolValue_free(taskTreePoolValue *ttpv);

// 任务树池
typedef struct taskTreePool {
    Tree *n_tree;
    pthread_mutex_t n_lock;
} taskTreePool;

taskTreePool *taskTreePool_init();
int taskTreePool_insert(taskTreePool *ttp, taskTreePoolValue *ttpv);
taskTreePoolValue *taskTreePool_get(taskTreePool *ttp, int clientFd);
void taskTreePool_display(taskTreePool *ttp, int index);
void taskTreePool_free(taskTreePool *ttp);

#endif
