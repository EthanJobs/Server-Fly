#include <coreIterator.h>
#include <coreStack.h>
#include <coreLink.h>

Stack *Stack_init(void (*fun_free)(void *)) {
    return Link_init(fun_free);
}

int Stack_isEmpty(Stack *s) {
    return s->n_linkNodeNum == 0;
}

int Stack_push(Stack *s, void *value) {
    return Link_headInsertValue(s, value);
}

void *Stack_pop(Stack *s) {
   return Link_getAndDeleteNode(s, 0); 
}

void *Stack_getTopValue(Stack *s) {
    return Link_getValue(s, 0);
}

void Stack_free(Stack *s) {
    Link_free(s);

    return ;
}

Iterator *Stack_getIterator(Stack *s) {
    return Link_getIterator(s);
}
