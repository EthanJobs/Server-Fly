#ifndef _CORESTACK_H
#define _CORESTACK_H

#include <coreIterator.h>
#include <coreLink.h>

typedef Link Stack;

Stack *Stack_init(void (*fun_free)(void *));
int Stack_isEmpty(Stack *s);
int Stack_push(Stack *s, void *value);
void *Stack_pop(Stack *s);
void *Stack_getTopValue(Stack *s);
void Stack_free(Stack *s);
Iterator *Stack_getIterator(Stack *s);

#endif

/*
 *  Stack *Stack_init(void (*fun_free)(void *));
 *      初始化栈并记录释放value值的方法fun_free(可以为NULL),失败返回NULL
 *  
 *  int Stack_isEmpty(Stack *s);
 *      判断栈是否为空,如果为空返回1,否则返回0
 *  
 *  int Stack_push(Stack *s, void *value);
 *      入栈,成功返回1,失败返回0
 *  
 *  void *Stack_pop(Stack *s);
 *      出栈,成功返回其值,失败返回NULL
 *  
 *  void *Stack_getTopValue(Stack *s);
 *      获取栈顶value值,成功返回其值,失败返回NULL
 *  
 *  void Stack_free(Stack *s);
 *      释放栈
 *  
 *  Iterator *Stack_getIterator(Stack *s);
 *      返回StackIterator
 */
