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
int Tree_isHave(Tree *t, void *value);
void *Tree_getValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
int Tree_deleteValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
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
 *      void TreeNode_print(TreeNode *tn, void (*fun_printValue)(void *));
 *      void TreeNode_print_p(TreeNode *tn, void *p, void (*fun_printValue)(void *, void *));
 *          以fun_printValue函数方法输出tn的n_value值(分为带参和不带参)
 *      
 *      void TreeNode_free(TreeNode *tn, void (*fun_freeValue)(void *));
 *          以fun_freeValue的方式释放tn的n_value值并释放tn
 *  Tree
 *      Tree *Tree_init();
 *          创建Tree,成功返回新建的Tree,失败返回NULL
 *      
 *      int Tree_insertNode(Tree *t, TreeNode *newtn, int (*fun_compareValue)(void *, void *));
 *          将newtn插入t中,其间需要用fun_compareValue方法比较大小,
 *          成功返回0,失败返回-1
 *      
 *      int Tree_insertValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));
 *          新建一个值为value的TreeNode插入到t中,期间需要用fun_compareValue方法比较大小
 *          成功返回0,失败返回-1
 *      
 *      int Tree_deleteValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *), void (*fun_freeValue)(void *));
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
 *      void Tree_inorderTraverse(Tree *t, void (*fun_printValue)(void *));
 *      void Tree_inorderTraverse_p(Tree *t, void *p, void (*fun_printValue)(void *, void *));
 *          树的中序遍历(分为带参和不带参)
 *      
 *      void Tree_preorderTraverse(Tree *t, void (*fun_printValue)(void *));
 *      void Tree_preorderTraverse_p(Tree *t, void *p, void (*fun_printValue)(void *, void *));
 *          树的前序遍历(分为带参和不带参)
 *      
 *      void Tree_postorderTraverse(Tree *t, void (*fun_printValue)(void *));
 *      void Tree_postorderTraverse_p(Tree *t, void *p, void (*fun_printValue)(void *, void *));
 *          树的后序遍历(分为带参和不带参)
 *      
 *      void Tree_levelTraverse(Tree *t, void (*fun_printValue)(void *));
 *      void Tree_levelTraverse_p(Tree *t, void *p, void (*fun_printValue)(void *, void *));
 *          树的层序遍历(分为带参和不带参)
 *      
 *      void Tree_free(Tree *t, void (*fun_freeValue)(void *));
 *          树的释放
 */
