#ifndef _CORETREE_H
#define _CORETREE_H

#include <coreIterator.h>

// 树节点
typedef struct TreeNode {
    int n_height;               //树高
    void *n_value;              //树节点值
    struct TreeNode *n_lChild;  //左子树节点
    struct TreeNode *n_rChild;  //右子树节点
} TreeNode;

// AVL树
typedef struct Tree {
    int n_treeNodeNum;                          //树节点数
    TreeNode *n_root;                           //根节点
    int (*fun_compareValue)(void *, void *);    //比较两个Value值 不可为NULL
    void (*fun_freeValue)(void *);              //value释放函数 可以为NULL
} Tree;

/* TreeNode */
TreeNode *TreeNode_init(void *value);
void TreeNode_free(TreeNode *tn, void (*fun_freeValue)(void *));

/* Tree */
Tree *Tree_init(int (*fun_compareValue)(void *, void *), void (*fun_freeValue)(void *));
int Tree_insertNode(Tree *t, TreeNode *newtn);
int Tree_insertValue(Tree *t, void *value);
int Tree_deleteValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
int Tree_isHave(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
void *Tree_getValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
void *Tree_getAndDeleteValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
void Tree_free(Tree *t);

/* Iterator */
Iterator *Tree_getPreorderIterator(Tree *t);
Iterator *Tree_getInorderIterator(Tree *t);
Iterator *Tree_getPostorderIterator(Tree *t);
Iterator *Tree_getLevelorderIterator(Tree *t);

#endif

/*
 *  TreeNode
 *      TreeNode *TreeNode_init(void *value);
 *          创建一个n_value值为value的TreeNode,成功返回TreeNode,失败返回NULL
 *      
 *      void TreeNode_free(TreeNode *tn, void (*fun_freeValue)(void *));
 *          以fun_freeValue的方式释放tn的n_value值并释放tn
 *  Tree
 *      Tree *Tree_init(int (*fun_compareValue)(void *, void *), void (*fun_freeValue)(void *));
 *          创建Tree并记录其比较方法fun_compareValue(不可为NULL)和其value释放方法fun_freeValue,成功返回新建的Tree,失败返回NULL
 *      
 *      int Tree_insertNode(Tree *t, TreeNode *newtn);
 *          将newtn插入t中,其间需要用fun_compareValue方法比较大小,
 *          成功返回0,失败返回-1
 *      
 *      int Tree_insertValue(Tree *t, void *value);
 *          新建一个值为value的TreeNode插入到t中,期间需要用fun_compareValue方法比较大小
 *          成功返回0,失败返回-1
 *      
 *      int Tree_deleteValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
 *          删除和value一样的值,以fun_compareValue方法寻找,以fun_freeValue方法释放value
 *          成功返回0,失败返回-1
 *
 *      int Tree_isHave(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
 *          判断树中有没有和value一样的值,以fun_compareValue方法寻找
 *          成功返回1，失败返回0
 *
 *      void *Tree_getValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
 *          在t中寻找并返回和value一样的值,以fun_compareValue方法比较大小
 *          成功返回该值,失败返回NULL
 *      
 *      void Tree_free(Tree *t);
 *          树的释放
 *      
 *      Iterator *Tree_getPreorderIterator(Tree *t);
 *          获取前序遍历的TreeIterator
 *
 *      Iterator *Tree_getInorderIterator(Tree *t);
 *          获取中序遍历的TreeIterator
 *      
 *      Iterator *Tree_getPostorderIterator(Tree *t);
 *          获取后序遍历的TreeIterator
 *
 *      Iterator *Tree_getLevelorderIterator(Tree *t);
 *          获取层序遍历的TreeIterator
 */
