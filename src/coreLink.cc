#include <coreLink.h>
#include <stdlib.h>

// 迭代器
typedef struct Link_Iterator_data {
    LinkNode *preln, *curln, *nextln;
    Link *l;
} Link_Iterator_data;

void Link_Iterator_freeData(void *lid);
int Link_Iterator_hasNext(Iterator *i);
void *Link_Iterator_next(Iterator *i);
int Link_Iterator_remove(Iterator *i);

LinkNode *LinkNode_init(void *value) {
    LinkNode *ln = (LinkNode *)malloc(sizeof(LinkNode));
    
    if (ln == NULL) return NULL;

    ln->n_value = value;
    ln->n_next = NULL;

    return ln;
}

void LinkNode_free(LinkNode *ln, void (*fun_freeValue)(void *)) {
    if (ln == NULL) return ;

    if (fun_freeValue != NULL)
        fun_freeValue((void *)ln->n_value);
    free(ln);

    return ;
}

Link *Link_init(void (*fun_freeValue)(void *)) {
    Link *l = (Link *)malloc(sizeof(Link));
    
    if (l == NULL) return NULL;
    
    l->n_linkNodeNum = 0;
    l->n_head = NULL;
    l->n_tail = NULL;
    l->fun_freeValue = fun_freeValue;

    return l;
}

int Link_getLength(Link *l) {
    if (l == NULL) return -1;
    
    return l->n_linkNodeNum;
}

void *Link_getHeadValue(Link *l) {
    if (l == NULL || l->n_head == NULL) return NULL;

    return l->n_head->n_value;
}

void *Link_getTailValue(Link *l) {
    if (l == NULL || l->n_head == NULL) return NULL;

    return l->n_tail->n_value;
}

int Link_queryValueIndex(Link *l, void *value, int (*fun_compareValue)(void *, void *)) {
    if (l == NULL || fun_compareValue == NULL) return -1;

    int i = 0;
    for (LinkNode *ln = l->n_head; ln != NULL; ln = ln->n_next, i++)
        if (fun_compareValue(value, ln->n_value))
            return i;
    
    return 0;
}

void *Link_queryValue(Link *l, void *value, int (*fun_compareValue)(void *, void *)) {
    if (l == NULL || fun_compareValue == NULL) return NULL;

    int i = 0;
    for (LinkNode *ln = l->n_head; ln != NULL; ln = ln->n_next, i++)
        if (fun_compareValue(value, ln->n_value))
            return ln->n_value;
    
    return NULL;
}

int Link_insertNode(Link *l, int nodeNum, LinkNode *newln) {
    if (nodeNum < 0 || nodeNum > l->n_linkNodeNum + 1) return 0;
    if (l == NULL || newln == NULL) return -1;

    if (nodeNum == 0)
        return Link_headInsertNode(l, newln);

    int i = 0;
    for (LinkNode *ln = l->n_head; ln != NULL; ln = ln->n_next, i++) {
        if (i == nodeNum - 1) {
            newln->n_next = ln->n_next;
            ln->n_next = newln;
            if (nodeNum == l->n_linkNodeNum)
                l->n_tail = newln;
            l->n_linkNodeNum++;
            
            return 1;
        }
    }

    return 0;
}

int Link_insertValue(Link *l, int nodeNum, void *value) {
    return Link_insertNode(l, nodeNum, LinkNode_init(value));
}

int Link_headInsertNode(Link *l, LinkNode *newln) {
    if (l == NULL || newln == NULL) return -1;

    if (l->n_head == NULL) {
        l->n_head = l->n_tail = newln;
        l->n_linkNodeNum++;
        return 1;
    }
    
    newln->n_next = l->n_head;
    l->n_head = newln;

    l->n_linkNodeNum++;

    return 1;
}

int Link_headInsertValue(Link *l, void *value) {
    return Link_headInsertNode(l, LinkNode_init(value));
}

int Link_tailInsertNode(Link *l, LinkNode *newln) {
    if (l == NULL || newln == NULL) return -1;

    if (l->n_head == NULL) {
        l->n_head = l->n_tail = newln;
        l->n_linkNodeNum++;
        return 1;
    }

    l->n_tail->n_next = newln;
    l->n_tail = newln;
    
    l->n_linkNodeNum++;

    return 1;
}

int Link_tailInsertValue(Link *l, void *value) {
    return Link_tailInsertNode(l, LinkNode_init(value));
}

void *Link_getValue(Link *l, int nodeNum) {
    if (nodeNum < 0 || nodeNum >= l->n_linkNodeNum) return NULL;
    if (l == NULL) return NULL;

    int i = 0;
    for (LinkNode *ln = l->n_head; ln != NULL; ln = ln->n_next, i++)
        if (i == nodeNum)
            return ln->n_value;
    
    return NULL;
}

void Link_deleteNode(Link *l, int nodeNum) {
    if (nodeNum < 0 || nodeNum >= l->n_linkNodeNum) return ;
    if (l == NULL) return ;

    if (nodeNum == 0) {
        LinkNode *delln = l->n_head;
        l->n_head = delln->n_next;
        LinkNode_free(delln, l->fun_freeValue);
        l->n_linkNodeNum--;

        return ;
    }

    int i = 0;
    for (LinkNode *ln = l->n_head; ln != NULL; ln = ln->n_next, i++) {
        if (i == nodeNum - 1) {
            LinkNode *delln = ln->n_next;
            ln->n_next = delln->n_next;
            LinkNode_free(delln, l->fun_freeValue);
            if (nodeNum == l->n_linkNodeNum)
                l->n_tail = ln;
            l->n_linkNodeNum--;

            return ;
        }
    }

    return ;
}

void *Link_getAndDeleteNode(Link *l, int nodeNum) {
    if (nodeNum < 0 || nodeNum >= l->n_linkNodeNum) return NULL;
    if (l == NULL) return NULL;

    if (nodeNum == 0) {
        LinkNode *delln = l->n_head;
        void *ret = delln->n_value;
        l->n_head = delln->n_next;
        LinkNode_free(delln, NULL);
        l->n_linkNodeNum--;

        return ret;
    }

    int i = 0;
    for (LinkNode *ln = l->n_head; ln != NULL; ln = ln->n_next, i++) {
        if (i == nodeNum - 1) {
            LinkNode *delln = ln->n_next;
            void *ret = delln->n_value;
            ln->n_next = delln->n_next;
            LinkNode_free(delln, NULL);
            if (nodeNum == l->n_linkNodeNum)
                l->n_tail = ln;
            l->n_linkNodeNum--;

            return ret;
        }
    }

    return NULL;
}

void Link_free(Link *l) {
    if (l == NULL) return ;
    
    while (l->n_head != NULL) {
        LinkNode *delln = l->n_head;
        l->n_head = delln->n_next;
        LinkNode_free(delln, l->fun_freeValue);
    }

    free(l);

    return ;
}

void Link_Iterator_freeData(void *lid) {
    if (lid == NULL) return ;
    
    free((Link_Iterator_data *)lid);

    return ;
}

int Link_Iterator_hasNext(Iterator *i) {
    if (i == NULL) return 0;
    
    return ((Link_Iterator_data *)i->n_data)->nextln == NULL;
}

void *Link_Iterator_next(Iterator *i) {
    if (i == NULL) return NULL;

    ((Link_Iterator_data *)i->n_data)->preln = ((Link_Iterator_data *)i->n_data)->curln;
    ((Link_Iterator_data *)i->n_data)->curln = ((Link_Iterator_data *)i->n_data)->nextln;
    ((Link_Iterator_data *)i->n_data)->nextln = ((Link_Iterator_data *)i->n_data)->nextln->n_next;

    return (void *)((Link_Iterator_data *)i->n_data)->curln->n_value;
}

int Link_Iterator_remove(Iterator *i) {
    if (i == NULL) return -2;
    if (((Link_Iterator_data *)i->n_data)->curln == NULL) return -1;

    if (((Link_Iterator_data *)i->n_data)->preln == NULL) {
        ((Link_Iterator_data *)i->n_data)->l->n_head = ((Link_Iterator_data *)i->n_data)->curln->n_next;
        LinkNode_free(((Link_Iterator_data *)i->n_data)->curln, ((Link_Iterator_data *)i->n_data)->l->fun_freeValue);
        ((Link_Iterator_data *)i->n_data)->l->n_linkNodeNum--;
        ((Link_Iterator_data *)i->n_data)->curln = NULL;

        return 0;
    }

    ((Link_Iterator_data *)i->n_data)->preln->n_next = ((Link_Iterator_data *)i->n_data)->nextln;
    LinkNode_free(((Link_Iterator_data *)i->n_data)->curln, ((Link_Iterator_data *)i->n_data)->l->fun_freeValue);
    ((Link_Iterator_data *)i->n_data)->l->n_linkNodeNum--;
    ((Link_Iterator_data *)i->n_data)->curln = NULL;

    return 0;
}

Iterator *Link_getIterator(Link *l) {
    Link_Iterator_data *lid = (Link_Iterator_data *)malloc(sizeof(Link_Iterator_data));
    lid->preln = lid->curln = NULL;
    lid->nextln = l->n_head;
    lid->l = l;
    return Iterator_init((void *)lid, Link_Iterator_freeData, Link_Iterator_hasNext, Link_Iterator_next, Link_Iterator_remove);
}
