#ifndef _CORELINK_H
#define _CORELINK_H

#include <coreIterator.h>

// 链表节点
typedef struct LinkNode {
    void *n_value;                  //节点值
    struct LinkNode *n_next;        //next节点
} LinkNode;

// 链表
typedef struct Link {
    int n_linkNodeNum;              //链表节点数
    LinkNode *n_head;               //头节点
    LinkNode *n_tail;               //头节点
    void (*fun_freeValue)(void *);  //value释放函数 可以为NULL
} Link;

/* LinkNode */
LinkNode *LinkNode_init(void *value);
void LinkNode_free(LinkNode *ln, void (*fun_freeValue)(void *));

/* Link */
Link *Link_init(void (*fun_freeValue)(void *));
int Link_getLength(Link *l);
void *Link_getHeadValue(Link *l);
void *Link_getTailValue(Link *l);
int Link_queryValueIndex(Link *l, void *value, int (*fun_compareValue)(void *, void *));
void *Link_queryValue(Link *l, void *value, int (*fun_compareValue)(void *, void *));
int Link_insertNode(Link *l, int nodeNum, LinkNode *newln);
int Link_insertValue(Link *l, int nodeNum, void *value);
int Link_headInsertNode(Link *l, LinkNode *newln);
int Link_headInsertValue(Link *l, void *value);
int Link_tailInsertNode(Link *l, LinkNode *newln);
int Link_tailInsertValue(Link *l, void *value);
void *Link_getValue(Link *l, int nodeNum);
void Link_deleteNode(Link *l, int nodeNum);
void *Link_getAndDeleteNode(Link *l, int nodeNum);
void Link_free(Link *l);

/* Iterator */
Iterator *Link_getIterator(Link *l);

#endif

/*
 *  LinkNode
 *      LinkNode *LinkNode_init(void *value);
 *          将value作为n_value创建新的LinkNode,成功返回新建的LinkNode,失败返回NULL
 *
 *      void LinkNode_free(LinkNode *ln, void (*fun_freeValue)(void *));
 *          以fun_freeValue的方式释放ln的n_value并释放LinkNode
 *
 *  Link
 *      Link *Link_init(void (*fun_freeValue)(void *));
 *          创建新的Link,并记录其释放其值的方法fun_freeValue,成功返回新建的Link,失败返回NULL
 *      
 *      int Link_getLength(Link *l);
 *          返回Link的n_linkNodeNum值，如果l为NULL则返回-1
 *      
 *      void *Link_getHeadValue(Link *l);
 *          返回Link的头节点的值,失败返回NULL
 *      
 *      void *Link_getTailValue(Link *l);
 *          返回Link的尾节点的值,失败返回NULL
 *      
 *      int Link_queryValueIndex(Link *l, void *value, int (*fun_compareValue)(void *, void *));
 *          以fun_compareValue方法比较value值是否一致以查询l里是否存在value值,
 *          成功则返回该节点所在数,失败则返回0,如果l为NULL则返回-1
 *      
 *      void *ink_queryValue(Link *l, void *value, int (*fun_compareValue)(void *, void *));
 *          以fun_compareValue方法比较value值是否一致以查询l里是否存在value值,
 *          成功则返回该节点的Value值,失败则返回NULL
 *      
 *      int Link_insertNode(Link *l, int nodeNum, LinkNode *newln);
 *          将newln插入到l的第nodeNum个节点处,成功返回1,失败返回0,如果l为NULL则返回-1
 *      
 *      int Link_insertValue(Link *l, int nodeNum, void *value);
 *          新建一个值为value的newln插入到l的第nodeNum个节点处,成功返回1,失败返回0,如果l为NULL则返回-1
 *      
 *      int Link_headInsertNode(Link *l, LinkNode *newln);
 *          将ln插入l的头节点,成功返回1,失败返回0,如果l或newln为NULL则返回-1(失败的唯一方法为l或newln为NULL)
 *      
 *      int Link_headInsertValue(Link *l, void *value);
 *          新建一个值为value的ln插入l的头节点,成功返回1,失败返回0,
 *          如果l或newln为NULL则返回-1(失败的唯一方法为l或newln为NULL)
 *      
 *      int Link_tailInsertNode(Link *l, LinkNode *newln);
 *          将ln插入l的尾节点,成功返回1,失败返回0,如果l或newln为NULL则返回-1(失败的唯一方法为l或newln为NULL)
 *      
 *      int Link_tailInsertValue(Link *l, void *value);
 *          新建一个值为value的ln插入l的尾节点,成功返回1,失败返回0,
 *          如果l或newln为NULL则返回-1(失败的唯一方法为l或newln为NULL)
 *      
 *      void *Link_getValue(Link *l, int nodeNum);
 *          获取l中第nodeNum个节点的n_value值,失败返回NULL
 *      
 *      void Link_deleteNode(Link *l, int nodeNum);
 *          删除l中第nodeNum个节点,并以fun_freeValue方法释放其n_value值
 *
 *      void *Link_getAndDeleteNode(Link *l, int nodeNum);
 *          获取l中第nodeNum个节点的n_value值并释放该节点(注意不会释放其n_value值),失败返回NULL
 *
 *      void Link_free(Link *l);
 *          以fun_freeValue的方式释放l中每个节点的n_value并释放各个节点最后释放l
 *
 *      Iterator *Link_getIterator(Link *l);
 *          返回一个相应的LinkIterator
 */
