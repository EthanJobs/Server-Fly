#include <coreIterator.h>
#include <stdlib.h>

Iterator *Iterator_init(void *data, void (*fun_freeData)(void *), int (*fun_hasNext)(Iterator *), void *(*fun_next)(Iterator *), int (*fun_remove)(Iterator *)) {
    if (fun_freeData == NULL || fun_hasNext == NULL || fun_next == NULL || fun_remove == NULL) return NULL;
    Iterator *i = (Iterator *)malloc(sizeof(Iterator));
    
    if (i == NULL) return NULL;

    i->n_data = data;
    i->fun_freeData = fun_freeData;
    i->fun_hasNext = fun_hasNext;
    i->fun_next = fun_next;
    i->fun_remove = fun_remove;

    return i;
}

void Iterator_free(Iterator *i) {
    if (i == NULL) return ;

    free(i);
    return ;
}
