#include <coreIterator.h>
#include <coreQueue.h>
#include <coreLink.h>

Queue *Queue_init(void (*fun_free)(void *)) {
    return Link_init(fun_free);
}

int Queue_isEmpty(Queue *q) {
    return q->n_linkNodeNum == 0;
}

int Queue_push(Queue *q, void *value) {
    return Link_tailInsertValue(q, value);
}

void *Queue_pop(Queue *q) {
    return Link_getAndDeleteNode(q, 0);
}

void *Queue_getHeadValue(Queue *q) {
    return Link_getValue(q, 0);
}

void Queue_free(Queue *q) {
    Link_free(q);

    return ;
}

Iterator *Queue_getIterator(Queue *q) {
    return Link_getIterator(q);
}
