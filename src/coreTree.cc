#include <coreIterator.h>
#include <coreStack.h>
#include <coreQueue.h>
#include <coreTree.h>
#include <stdlib.h>
#include <stdio.h>

// 无实际应用 基本为递归函数
TreeNode *Tree_in_lRotate(TreeNode *tn);
TreeNode *Tree_in_rRotate(TreeNode *tn);
TreeNode *Tree_in_adjust(TreeNode *tn);
TreeNode *Tree_insertNode_in(TreeNode *tn, TreeNode *newtn, int (*fun_compareValue)(void *, void *));
TreeNode *Tree_getValue_in(TreeNode *tn, void *value, int (*fun_compareValue)(void *, void *));
TreeNode *Tree_deleteValue_in(TreeNode *tn, void *value, int (*fun_compareValue)(void *, void *), void (*fun_freeValue)(void *));
TreeNode *Tree_isHave_in(TreeNode *tn, void *value, int (*fun_compareValue)(void *, void*));
void Tree_free_in(TreeNode *tn, void (*fun_freeValue)(void *));

// 迭代器
typedef struct Tree_Iterator_StackData {
    TreeNode *lasttn;
    Tree *t;
    Stack *s;
} Tree_Iterator_StackData;

void Tree_Iterator_freeStackData(void *tisd);
int Tree_stackIterator_hasNext(Iterator *i);
int Tree_stackIterator_remove(Iterator *i);

void *Tree_preorderIterator_next(Iterator *i);
void *Tree_inorderIterator_next(Iterator *i);
void *Tree_postorderIterator_next(Iterator *i);

typedef struct Tree_Iterator_QueueData {
    TreeNode *lasttn;
    Tree *t;
    Queue *q;
} Tree_Iterator_QueueData;

void Tree_Iterator_freeQueueData(void *tiqd);
int Tree_levelordertIterator_hasNext(Iterator *i);
int Tree_levelorderIterator_remove(Iterator *i);

void *Tree_levelorderIterator_next(Iterator *i);

TreeNode *TreeNode_init(void *value) {
    TreeNode *tn = (TreeNode *)malloc(sizeof(TreeNode));

    tn->n_height = 1;
    tn->n_value = value;
    tn->n_lChild = NULL;
    tn->n_rChild = NULL;

    return tn;
}

void TreeNode_free(TreeNode *tn, void (*fun_freeValue)(void *)) {
    if (tn == NULL) return ;

    if (fun_freeValue != NULL)
    fun_freeValue((void *)tn->n_value);
    free(tn);

    return ;
}

Tree *Tree_init(int (*fun_compareValue)(void *, void *), void (*fun_freeValue)(void *)) {
    if (fun_compareValue == NULL) return NULL;
    Tree *t = (Tree *)malloc(sizeof(Tree));

    t->n_treeNodeNum = 0;
    t->n_root = NULL;
    t->fun_compareValue = fun_compareValue;
    t->fun_freeValue = fun_freeValue;

    return t;
}

void Tree_buildHeight(TreeNode *tn) {
    int lChildHeight = tn->n_lChild == NULL ? 0 : tn->n_lChild->n_height;
    int rChildHeight = tn->n_rChild == NULL ? 0 : tn->n_rChild->n_height;

    tn->n_height = lChildHeight > rChildHeight ? lChildHeight + 1 : rChildHeight + 1;
}

TreeNode *Tree_in_lRotate(TreeNode *tn) {
    TreeNode *rtn = tn->n_rChild;
    tn->n_rChild = rtn->n_lChild;
    rtn->n_lChild = tn;

    Tree_buildHeight(tn);
    Tree_buildHeight(rtn);

    return rtn;
}

TreeNode *Tree_in_rRotate(TreeNode *tn) {
    TreeNode *ltn = tn->n_lChild;
    tn->n_lChild = ltn->n_rChild;
    ltn->n_rChild = tn;

    Tree_buildHeight(tn);
    Tree_buildHeight(ltn);

    return ltn;
}

TreeNode *Tree_in_adjust(TreeNode *tn, int lChildHeight, int rChildHeight) {
    if (tn == NULL) return NULL;

    int fd = lChildHeight - rChildHeight;

    if (fd > 1) {
        int llChildHeight = tn->n_lChild->n_lChild == NULL ? 0 : tn->n_lChild->n_lChild->n_height;
        int lrChildHeight = tn->n_lChild->n_rChild == NULL ? 0 : tn->n_lChild->n_rChild->n_height;
        if (llChildHeight >= lrChildHeight) {
            tn = Tree_in_rRotate(tn);
        } else {
            tn->n_lChild = Tree_in_lRotate(tn->n_lChild);
            tn = Tree_in_rRotate(tn);
        }
    } else if (fd < -1) {
        int rlChildHeight = tn->n_rChild->n_lChild == NULL ? 0 : tn->n_rChild->n_lChild->n_height;
        int rrChildHeight = tn->n_rChild->n_rChild == NULL ? 0 : tn->n_rChild->n_rChild->n_height;
        if (rrChildHeight >= rlChildHeight) {
            tn = Tree_in_lRotate(tn);
        } else {
            tn->n_rChild = Tree_in_rRotate(tn->n_rChild);
            tn = Tree_in_lRotate(tn);
        }
    }

    return tn;
}

TreeNode *Tree_insertNode_in(TreeNode *tn, TreeNode *newtn, int (*fun_compareValue)(void *, void *)) {
    if (tn == NULL) return newtn;

    int cmpTmp = fun_compareValue((void *)newtn->n_value, (void *)tn->n_value);

    if (cmpTmp < 0)
        tn->n_lChild = Tree_insertNode_in(tn->n_lChild, newtn, fun_compareValue);
    else if (cmpTmp > 0)
        tn->n_rChild = Tree_insertNode_in(tn->n_rChild, newtn, fun_compareValue);
    else
        return tn;

    int lChildHeight = tn->n_lChild == NULL ? 0 : tn->n_lChild->n_height;
    int rChildHeight = tn->n_rChild == NULL ? 0 : tn->n_rChild->n_height;
    
    tn->n_height = lChildHeight > rChildHeight ? lChildHeight + 1 : rChildHeight + 1;

    return Tree_in_adjust(tn, lChildHeight, rChildHeight);
}

int Tree_insertNode(Tree *t, TreeNode *newtn) {
    if (t == NULL || newtn == NULL) return -1;

    if (t->n_root == NULL) {
        t->n_root = newtn;
        t->n_root->n_height = 1;
        t->n_treeNodeNum++;
        return 0;
    }

    t->n_root = Tree_insertNode_in(t->n_root, newtn, t->fun_compareValue);
    t->n_treeNodeNum++;
    return 0;
}

int Tree_insertValue(Tree *t, void *value) {
    return Tree_insertNode(t, TreeNode_init(value));
}

TreeNode *Tree_deleteValue_in(TreeNode *tn, void *value, int (*fun_compareValue)(void *, void *), void (*fun_freeValue)(void *)) {
    int cmpTmp = fun_compareValue((void *)value, (void *)tn->n_value);

    if (cmpTmp < 0)
        tn->n_lChild = Tree_deleteValue_in(tn->n_lChild, value, fun_compareValue, fun_freeValue);
    else if (cmpTmp > 0)
        tn->n_rChild = Tree_deleteValue_in(tn->n_rChild, value, fun_compareValue, fun_freeValue);
    else {
        if (tn->n_lChild == NULL && tn->n_rChild == NULL) {
            TreeNode_free(tn, fun_freeValue);
            return NULL;
        } else if (tn->n_lChild != NULL && tn->n_rChild == NULL) {
            TreeNode *retNode = tn->n_lChild;
            TreeNode_free(tn, fun_freeValue);
            tn = retNode;
        } else if (tn->n_lChild == NULL && tn->n_rChild != NULL) {
            TreeNode *retNode = tn->n_rChild;
            TreeNode_free(tn, fun_freeValue);
            tn = retNode;
        } else {
            TreeNode *targetNode = tn->n_lChild;
            while (targetNode->n_rChild != NULL)
                targetNode = targetNode->n_rChild;
            void *tmpValue = targetNode->n_value;
            tn->n_lChild = Tree_deleteValue_in(tn->n_lChild, tmpValue, fun_compareValue, NULL);
            if (fun_freeValue) fun_freeValue(tn->n_value);
            tn->n_value = tmpValue;
        }
    }

    int lChildHeight = tn->n_lChild == NULL ? 0 : tn->n_lChild->n_height;
    int rChildHeight = tn->n_rChild == NULL ? 0 : tn->n_rChild->n_height;
    
    tn->n_height = lChildHeight > rChildHeight ? lChildHeight + 1 : rChildHeight + 1;

    return Tree_in_adjust(tn, lChildHeight, rChildHeight);
}

int Tree_deleteValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *)) {
    if (t == NULL || value == NULL ) return -1;

    if (Tree_isHave(t, value, fun_compareValue) == 1) {
        t->n_root = Tree_deleteValue_in(t->n_root, value, fun_compareValue, t->fun_freeValue);
        t->n_treeNodeNum--;
        return 0;
    }

    return -1;
}

TreeNode *Tree_isHave_in(TreeNode *tn, void *value, int (*fun_compareValue)(void *, void*)) {
    if (tn == NULL) return NULL;

    int cmpTmp = fun_compareValue((void *)value, (void *)tn->n_value);

    if (cmpTmp < 0)
        return Tree_isHave_in(tn->n_lChild, value, fun_compareValue);
    else if (cmpTmp > 0)
        return Tree_isHave_in(tn->n_rChild, value, fun_compareValue);
    else
        return tn;
}

int Tree_isHave(Tree *t, void *value, int (*fun_compareValue)(void *, void *)) {
    if (Tree_isHave_in(t->n_root, value, fun_compareValue) != NULL) return 1;
    return 0;
}

TreeNode *Tree_getValue_in(TreeNode *tn, void *value, int (*fun_compareValue)(void *, void *)) {
    if (tn == NULL) return NULL;

    int cmpTmp = fun_compareValue((void *)value, (void *)tn->n_value);
    if (cmpTmp < 0)
    return Tree_getValue_in(tn->n_lChild, value, fun_compareValue);
    else if (cmpTmp > 0)
    return Tree_getValue_in(tn->n_rChild, value, fun_compareValue);
    else
    return tn;
}

void *Tree_getValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *)) {
    if (t == NULL || t->n_root == NULL || value == NULL) return NULL;

    TreeNode *rettn = Tree_getValue_in(t->n_root, value, fun_compareValue);
    if (rettn != NULL)
    return rettn->n_value;
    else
    return NULL;
}

void Tree_free_in(TreeNode *tn, void (*fun_freeValue)(void *)) {
    if (tn == NULL) return ; 

    Tree_free_in(tn->n_lChild, fun_freeValue);
    Tree_free_in(tn->n_rChild, fun_freeValue);
    if (fun_freeValue != NULL)
    TreeNode_free(tn, fun_freeValue);

    return ;
}

void Tree_free(Tree *t) {
    if (t == NULL) return ;

    Tree_free_in(t->n_root, t->fun_freeValue);
    free(t);

    return ;
}

void Tree_Iterator_freeStackData(void *tisd) {
    if (tisd == NULL) return ;

    Stack_free(((Tree_Iterator_StackData *)tisd)->s);
    free((Tree_Iterator_StackData *)tisd);

    return ;
}

void Tree_Iterator_freeQueueData(void *tiqd) {
    if (tiqd == NULL) return ;

    Queue_free(((Tree_Iterator_QueueData *)tiqd)->q);
    free((Tree_Iterator_QueueData *)tiqd);

    return ;
}

int Tree_stackIterator_hasNext(Iterator *i) {
    return !Stack_isEmpty(((Tree_Iterator_StackData *)i->n_data)->s);
}

int Tree_stackIterator_remove(Iterator *i) {
    if (i == NULL) return -1;
    if (((Tree_Iterator_StackData *)i->n_data)->lasttn == NULL) return -2;

    Tree_deleteValue(((Tree_Iterator_StackData *)i->n_data)->t, ((Tree_Iterator_StackData *)i->n_data)->lasttn, ((Tree_Iterator_StackData *)i->n_data)->t->fun_compareValue);
    ((Tree_Iterator_StackData *)i->n_data)->lasttn = NULL;
    
    return 0;
}

void *Tree_preorderIterator_next(Iterator *i) {
    if (i == NULL) return NULL;

    ((Tree_Iterator_StackData *)i->n_data)->lasttn = (TreeNode *)Stack_pop(((Tree_Iterator_StackData *)i->n_data)->s);
    if (((Tree_Iterator_StackData *)i->n_data)->lasttn->n_lChild != NULL)
        Stack_push(((Tree_Iterator_StackData *)i->n_data)->s, (void *)((Tree_Iterator_StackData *)i->n_data)->lasttn->n_lChild);
    if (((Tree_Iterator_StackData *)i->n_data)->lasttn->n_rChild != NULL)
        Stack_push(((Tree_Iterator_StackData *)i->n_data)->s, (void *)((Tree_Iterator_StackData *)i->n_data)->lasttn->n_rChild);

    return (void *)((Tree_Iterator_StackData *)i->n_data)->lasttn->n_value;
}

Iterator *Tree_getPreorderIterator(Tree *t) {
    Tree_Iterator_StackData *tisd = (Tree_Iterator_StackData *)malloc(sizeof(Tree_Iterator_StackData));
    tisd->lasttn = NULL;
    tisd->t = t;
    tisd->s = Stack_init(NULL);

    Stack_push(tisd->s, t->n_root);

    return Iterator_init((void *)tisd, Tree_Iterator_freeStackData, Tree_stackIterator_hasNext, Tree_preorderIterator_next, Tree_stackIterator_remove); 
}

void Tree_inorderIterator_pushSelfAndAllLChild(Stack *s, TreeNode *t) {
    if (t == NULL) return ;
    
    Stack_push(s, (void *)t);
    Tree_inorderIterator_pushSelfAndAllLChild(s, t->n_lChild);

    return ;
}

void *Tree_inorderIterator_next(Iterator *i) {
    if (i == NULL) return NULL;

    ((Tree_Iterator_StackData *)i->n_data)->lasttn = (TreeNode *)Stack_pop(((Tree_Iterator_StackData *)i->n_data)->s);
    Tree_inorderIterator_pushSelfAndAllLChild(((Tree_Iterator_StackData *)i->n_data)->s, ((Tree_Iterator_StackData *)i->n_data)->lasttn->n_rChild);

    return (void *)((Tree_Iterator_StackData *)i->n_data)->lasttn->n_value;
}

Iterator *Tree_getInorderIterator(Tree *t) {
    Tree_Iterator_StackData *tisd = (Tree_Iterator_StackData *)malloc(sizeof(Tree_Iterator_StackData));
    tisd->lasttn = NULL;
    tisd->t = t;
    tisd->s = Stack_init(NULL);

    Tree_inorderIterator_pushSelfAndAllLChild(tisd->s, t->n_root);

    return Iterator_init((void *)tisd, Tree_Iterator_freeStackData, Tree_stackIterator_hasNext, Tree_inorderIterator_next, Tree_stackIterator_remove); 
}

void Tree_postorderIterator_pushSelfAndAllRChild(Stack *s, TreeNode *t) {
    if (t == NULL) return ;

    Stack_push(s, (void *)t);
    Tree_postorderIterator_pushSelfAndAllRChild(s, t->n_rChild);

    return ;
}

void *Tree_postorderIterator_next(Iterator *i) {
    if (i == NULL) return NULL;

    ((Tree_Iterator_StackData *)i->n_data)->lasttn = (TreeNode *)Stack_pop(((Tree_Iterator_StackData *)i->n_data)->s);
    Tree_postorderIterator_pushSelfAndAllRChild(((Tree_Iterator_StackData *)i->n_data)->s, ((Tree_Iterator_StackData *)i->n_data)->lasttn->n_lChild);

    return (void *)((Tree_Iterator_StackData *)i->n_data)->lasttn->n_value;
}

Iterator *Tree_getPostorderIterator(Tree *t) {
    Tree_Iterator_StackData *tisd = (Tree_Iterator_StackData *)malloc(sizeof(Tree_Iterator_StackData));
    tisd->lasttn = NULL;
    tisd->t = t;
    tisd->s = Stack_init(NULL);

    Tree_postorderIterator_pushSelfAndAllRChild(tisd->s, t->n_root);

    return Iterator_init((void *)tisd, Tree_Iterator_freeStackData, Tree_stackIterator_hasNext, Tree_postorderIterator_next, Tree_stackIterator_remove); 
}

int Tree_levelordertIterator_hasNext(Iterator *i) {
    if (i == NULL) return 0;

    return !Queue_isEmpty(((Tree_Iterator_QueueData *)i->n_data)->q);
}

void *Tree_levelorderIterator_next(Iterator *i) {
    if (i == NULL) return NULL;

    ((Tree_Iterator_QueueData *)i->n_data)->lasttn = (TreeNode *)Queue_pop(((Tree_Iterator_QueueData *)i->n_data)->q);
    if (((Tree_Iterator_QueueData *)i->n_data)->lasttn->n_lChild != NULL)
        Queue_push(((Tree_Iterator_QueueData *)i->n_data)->q, (void *)((Tree_Iterator_QueueData *)i->n_data)->lasttn->n_lChild);
    if (((Tree_Iterator_QueueData *)i->n_data)->lasttn->n_rChild != NULL)
        Queue_push(((Tree_Iterator_QueueData *)i->n_data)->q, (void *)((Tree_Iterator_QueueData *)i->n_data)->lasttn->n_rChild);

    return (void *)((Tree_Iterator_QueueData *)i->n_data)->lasttn->n_value;
}

int Tree_levelorderIterator_remove(Iterator *i) {
    if (i == NULL) return -1;
    if (((Tree_Iterator_QueueData *)i->n_data)->lasttn == NULL) return -2;

    Tree_deleteValue(((Tree_Iterator_QueueData *)i->n_data)->t, ((Tree_Iterator_QueueData *)i->n_data)->lasttn, ((Tree_Iterator_QueueData *)i->n_data)->t->fun_compareValue);
    ((Tree_Iterator_QueueData *)i->n_data)->lasttn = NULL;
    
    return 0;
}

Iterator *Tree_getLevelorderIterator(Tree *t) {
    Tree_Iterator_QueueData *tiqd = (Tree_Iterator_QueueData *)malloc(sizeof(Tree_Iterator_QueueData));
    tiqd->lasttn = NULL;
    tiqd->t = t;
    tiqd->q = Queue_init(NULL);

    Queue_push(tiqd->q, (void *)t->n_root);

    return Iterator_init((void *)tiqd, Tree_Iterator_freeQueueData, Tree_levelordertIterator_hasNext, Tree_levelorderIterator_next, Tree_levelorderIterator_remove); 
}
