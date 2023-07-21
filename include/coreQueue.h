#ifndef _COREQUEUE_H
#define _COREQUEUE_H

#include <coreIterator.h>
#include <coreLink.h>

typedef Link Queue;

Queue *Queue_init(void (*fun_free)(void *));
int Queue_isEmpty(Queue *q);
int Queue_push(Queue *q, void *value);
void *Queue_pop(Queue *q);
void *Queue_getHeadValue(Queue *q);
void Queue_free(Queue *q);
Iterator *Queue_getIterator(Queue *q);

#endif

/*
 *  Queue *Queue_init(void (*fun_free)(void *));
 *      初始化一个队列并记录其释放value值的方法fun_free(可以为NULL),失败返回NULL 
 *  
 *  int Queue_isEmpty(Queue *q);
 *      判断队列是否为空,如果为空返回1,否则返回0
 *  
 *  int Queue_push(Queue *q, void *value);
 *      队列入队,成功返回1,失败返回0
 *  
 *  void *Queue_pop(Queue *q);
 *      队列出队,成功返回其值,失败返回NULL
 *  
 *  void *Queue_getHeadValue(Queue *q);
 *      获取队头value值,成功返回其值,失败返回NULL
 *  
 *  void Queue_free(Queue *q, void (*fun_free)(void *));
 *      队列释放
 *  
 *  Iterator *Queue_getIterator(Queue *q);
 *      返回QueueIterator
 */
