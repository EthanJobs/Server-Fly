#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <coreIterator.h>
#include <coreQueue.h>
#include <coreTree.h>
#include <networkPthreadPool.h>
#include <networkHttpHeader.h>

// 内置函数
void taskQueuePoolValue_fun_freeValue(void *tqpv);
void taskTreePoolValue_fun_freeValue(void *ttpv);
int taskTreePoolValue_fun_compareValue(void *a, void *b);
int taskTreePoolValue_fun_searchCompareValue(void *a, void *b);

taskQueuePoolValue *taskQueuePoolValue_init(int clientFd) {
    taskQueuePoolValue *tqpv = (taskQueuePoolValue *)malloc(sizeof(taskQueuePoolValue));
    tqpv->clientFd = clientFd;

    return tqpv;
}

void taskQueuePoolValue_print(taskQueuePoolValue *tqpv) {
    printf("%d ", tqpv->clientFd);

    return ;
}

void taskQueuePoolValue_free(taskQueuePoolValue *tqpv) {
    if (tqpv == NULL) return ;

    free(tqpv);

    return ;
}

void taskQueuePoolValue_fun_freeValue(void *tqpv) {
    taskQueuePoolValue_free((taskQueuePoolValue *)tqpv);

    return ;
}

taskQueuePool *taskQueuePool_init() {
    taskQueuePool *tqp = (taskQueuePool *)malloc(sizeof(taskQueuePool));
    tqp->n_queue = Queue_init(taskQueuePoolValue_fun_freeValue);
    pthread_mutex_init(&tqp->n_lock, NULL);
    pthread_cond_init(&tqp->n_haveTask, NULL);

    return tqp;
}

int taskQueuePool_push(taskQueuePool *tqp, taskQueuePoolValue *tqpv) {
    pthread_mutex_lock(&tqp->n_lock);

    int ret = Queue_push(tqp->n_queue, (void *)tqpv);

    pthread_cond_broadcast(&tqp->n_haveTask);
    pthread_mutex_unlock(&tqp->n_lock);

    return ret;
}

taskQueuePoolValue *taskQueuePool_pop(taskQueuePool *tqp) {
    pthread_mutex_lock(&tqp->n_lock);

    while (Queue_isEmpty(tqp->n_queue))
        pthread_cond_wait(&tqp->n_haveTask, &tqp->n_lock);

    void *ret = Queue_pop(tqp->n_queue);

    pthread_mutex_unlock(&tqp->n_lock);

    return (taskQueuePoolValue *)ret;
}

void taskQueuePool_display(taskQueuePool *tqp) {
    pthread_mutex_lock(&tqp->n_lock);

    Iterator *i = Queue_getIterator(tqp->n_queue);

    while (i->fun_hasNext(i))
        taskQueuePoolValue_print((taskQueuePoolValue *)i->fun_next(i));

    pthread_mutex_unlock(&tqp->n_lock);

    return ;
}

void taskQueuePool_free(taskQueuePool *tqp) {
    pthread_mutex_lock(&tqp->n_lock);

    Queue_free(tqp->n_queue);

    pthread_mutex_unlock(&tqp->n_lock);
    pthread_mutex_destroy(&tqp->n_lock);
    pthread_cond_destroy(&tqp->n_haveTask);

    return ;
}

taskTreePoolValue *taskTreePoolValue_init(int clientFd, HttpHeader *hh, int fileFd, int fileIndex) {
    taskTreePoolValue *ttpv = (taskTreePoolValue *)malloc(sizeof(taskTreePoolValue));
    ttpv->clientFd = clientFd;
    ttpv->hh= hh;
    ttpv->fileFd = fileFd;
    ttpv->fileIndex = fileIndex;

    return ttpv;
}

void taskTreePoolValue_print(taskTreePoolValue *ttpv) {
    printf("[%d, %s, %d, %d] ", ttpv->clientFd, ttpv->hh == NULL ? "NULL" : "hasHH", ttpv->fileFd, ttpv->fileIndex);

    return ;
}

void taskTreePoolValue_free(taskTreePoolValue *ttpv) {
    if (ttpv == NULL) return ;

    HttpHeader_free(ttpv->hh);
    free(ttpv);

    return ;
}

void taskTreePoolValue_fun_freeValue(void *ttpv) {
    taskTreePoolValue_free((taskTreePoolValue *)ttpv);

    return ;
}

int taskTreePoolValue_fun_compareValue(void *a, void *b) {
    return ((taskTreePoolValue *)a)->clientFd - ((taskTreePoolValue *)b)->clientFd;
}

int taskTreePoolValue_fun_searchCompareValue(void *a, void *b) {
    return *(int *)a - ((taskTreePoolValue *)b)->clientFd;
}

taskTreePool *taskTreePool_init() {
    taskTreePool *ttp = (taskTreePool *)malloc(sizeof(taskTreePool));
    ttp->n_tree = Tree_init(taskTreePoolValue_fun_compareValue, taskTreePoolValue_fun_freeValue);
    pthread_mutex_init(&ttp->n_lock, NULL);

    return ttp;
}

int taskTreePool_insert(taskTreePool *ttp, taskTreePoolValue *ttpv) {
    pthread_mutex_lock(&ttp->n_lock);
    
    int ret = Tree_insertValue(ttp->n_tree, (void *)ttpv);

    pthread_mutex_unlock(&ttp->n_lock);

    return ret;
}

taskTreePoolValue *taskTreePool_get(taskTreePool *ttp, int clientFd) {
    pthread_mutex_lock(&ttp->n_lock);

    void *ret = Tree_getValue(ttp->n_tree, (void *)&clientFd, taskTreePoolValue_fun_searchCompareValue);

    pthread_mutex_unlock(&ttp->n_lock);

    return (taskTreePoolValue *)ret;
}

void taskTreePool_display(taskTreePool *ttp, int index) {
    pthread_mutex_lock(&ttp->n_lock);

    Iterator *i;

    if (index = 0)
        i = Tree_getPreorderIterator(ttp->n_tree);
    else if (index == 1)
        i = Tree_getInorderIterator(ttp->n_tree);
    else if (index == 2)
        i = Tree_getPostorderIterator(ttp->n_tree);
    else
        i = Tree_getLevelorderIterator(ttp->n_tree);

    while (i->fun_hasNext(i))
        taskTreePoolValue_print((taskTreePoolValue *)i->fun_next(i));

    pthread_mutex_unlock(&ttp->n_lock);

    return ;
}

void taskTreePool_free(taskTreePool *ttp) {
    pthread_mutex_lock(&ttp->n_lock);

    Tree_free(ttp->n_tree);

    pthread_mutex_unlock(&ttp->n_lock);
    pthread_mutex_destroy(&ttp->n_lock);

    return ;
}
