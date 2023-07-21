#ifndef _COREITERATOR_H
#define _COREITERATOR_H

typedef struct Iterator {
    void *n_data;
    void (*fun_freeData)(void *);
    int (*fun_hasNext)(Iterator *);
    void *(*fun_next)(Iterator *);
    int (*fun_remove)(Iterator *);
} Iterator;

Iterator *Iterator_init(void *data, void (*fun_freeData)(void *), int (*fun_hasNext)(Iterator *), void *(*fun_next)(Iterator *), int (*fun_remove)(Iterator *));
void Iterator_free(Iterator *i);

/*
 *  Iterator *Iterator_init(void *data, void (*fun_freeData)(void *), int (*fun_hasNext)(Iterator *), void *(*fun_next)(Iterator *), int (*fun_remove)(Iterator *));
 *      初始化一个相应的Iterator 必须有以下四个函数
 *          fun_freeData(Iterator *i)
 *              释放Iterator的n_data
 *          fun_hasNext(Iterator *i)
 *              判断集合中是否还有下一个值,如果有则返回1,否则返回0
 *          fun_next(Iterator *i)
 *              返回集合的下一个值,此方法只能在fun_hasNext方法返回1时有返回值,其他时候返回NULL
 *          fun_remove(Iterator *i)
 *              在集合中删除上一次fun_next返回的值,一次fun_next方法只能使用一次fun_remove方法,成功返回0,失败返回其他值
 *      成功返回相应的Iterator,失败返回NULL
 *      
 *  void Iterator_free(Iterator *i);
 *      释放相应的Iterator
 */

#endif
