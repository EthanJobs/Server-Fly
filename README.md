# FlyServerEngine

FlyServerEngine是基于Linux操作系统下由C语言编写的服务器引擎,该引擎由四个层次组成:

| 层次           | 说明                                              |
| -------------- | ------------------------------------------------- |
| Process Layer  | 处理层 数据的识别,处理与输出等数据加工接口        |
| Network Layer  | 网络层 基础的网络协议与网络数据传输等网络相关接口 |
| Resource Layer | 资源层 相关文件操作和数据库操作等资源相关接口     |
| Core Layer     | 核心层 基础的数据结构和数学函数等基础函数接口     |

以上四个层次由低至高为 Core Layer -> Resource Layer -> Network Layer -> Process Layer,应基于"高层能调用低层的接口,而低层无法调用高层的接口"的原则来构建该引擎

用户应使用Linux操作系统运行以下代码(已用makefile文件集合),以在bin文件夹输出server.out可执行文件

```shell
make
```

makefile 文件

```shell
.PHONY: clear
all: ./src/coreLink.o ./src/coreTree.o ./src/coreJson.o ./src/coreMd5.o ./src/corePthreadPool.o ./src/resourceSQL.o ./src/networkServer.o ./src/networkHttpHeader.o main.o
	g++ -g ./src/coreLink.o ./src/coreTree.o ./src/coreJson.o ./src/coreMd5.o ./src/corePthreadPool.o ./src/resourceSQL.o ./src/networkServer.o ./src/networkHttpHeader.o main.o -o ./bin/server.out -lpthread -lmysqlclient
./src/coreLink.o: ./src/coreLink.cc
	g++ -g -I./include -c ./src/coreLink.cc -o ./src/coreLink.o
./src/coreTree.o: ./src/coreTree.cc
	g++ -g -I./include -c ./src/coreTree.cc -o ./src/coreTree.o
./src/coreJson.o: ./src/coreJson.cc 
	g++ -g -I./include -c ./src/coreJson.cc -o ./src/coreJson.o
./src/coreMd5.o: ./src/coreMd5.cc 
	g++ -g -I./include -c ./src/coreMd5.cc -o ./src/coreMd5.o
./src/corePthreadPool.o: ./src/corePthreadPool.cc 
	g++ -g -I./include -c ./src/corePthreadPool.cc -o ./src/corePthreadPool.o
./src/resourceSQL.o: ./src/resourceSQL.cc 
	g++ -g -I./include -c ./src/resourceSQL.cc -o ./src/resourceSQL.o
./src/networkServer.o: ./src/networkServer.cc 
	g++ -g -I./include -c ./src/networkServer.cc -o ./src/networkServer.o
./src/networkHttpHeader.o: ./src/networkHttpHeader.cc 
	g++ -g -I./include -c ./src/networkHttpHeader.cc -o ./src/networkHttpHeader.o
main.o: main.cpp
	g++ -g -I./include -c main.cpp -o main.o
clear:
	rm ./src/*.o main.o
```



**用户应仅在Procss Layer层改写并加入数据加工的具体代码以达到数据的自定义加工,越低层的代码可修改性越差**



以下是各层基本函数接口，相关数据结构和相关说明:



## Process Layer 处理层

------

**该层在main.cpp文件中**

未实现：文件权限管理\

### processMes 信息加工与回复

​	int processMes(int fd);

​	对文件描述符fd中的信息加工并回复

## Network Layer 网络层

------

未实现：http头读写（可以优化HttpHeader_fd_init()）

### Srever 网络监听和多线程调用

**server_start**

​	void server_start(const short unsigned int PORT, const char *IP, const unsigned int PTHREAD_MAX, const unsigned int EPOLL_SIZE, int (*processMes)(int fd));

​	新建一个在IP:PORT上的监听,其中使用了线程池(PTHREAD_MAX为线程个数)和EPOLL(EPOLL_SIZE为EPOLL最大监听个数)

### HttpHeader Http协议头的创建与读取

**数据结构**

```c
typedef struct requestHeaderValue {
    char *n_header;
    char *n_value;
} requestHeaderValue;

typedef struct HttpHeader {
    char *n_requestLine;
    Tree *n_requestHeader;
} HttpHeader;
```

**requestHeaderValue_init**

​	requestHeaderValue *requestHeaderValue_init(char *header, char *value);

​	创建相应的一条消息头,失败返回NULL

**requestHeaderValue_print**

​	void requestHeaderValue_print(requestHeaderValue *rhv);

​	输出消息头内容

**requestHeaderValue_free**

​	void requestHeaderValue_free(requestHeaderValue *rhv);

​	释放消息头结构

**HttpHeader_init**

​	HttpHeader *HttpHeader_init();

​	创建一个空的http协议结构,失败返回NULL

**HttpHeader_fd_init**

​	HttpHeader *HttpHeader_fd_init(int fd);

​	根据一个文件描述符创建相应的http结构,失败返回NULL

**HttpHeader_insertRequestHeader**

​	int HttpHeader_insertRequestHeader(HttpHeader *hh, requestHeaderValue *rhv);

​	插入新的消息头结构,成功返回0.失败返回-1

**HttpHeader_print**

​	void HttpHeader_print(HttpHeader *hh);

​	输出一个http协议结构

**HttpHeader_free**

​	void HttpHeader_free(HttpHeader *hh);

​	释放一个http协议结构

## Resource Layer 资源层

------

未实现：文件创建和删除返回文件描述符\文件大小格式等属性的获取\文件描述符的读写\分段文件管理

### SQL MYSQL数据库操作

**MYSQL_init**

​	MYSQL *MYSQL_init(const char *HOST, const char *USERNAME, const char *PASSWORD, const char *DATABASE, unsigned int timeout);

​	创建一个相应的MYSQL

**exeSql**

​	void exeSql(MYSQL *sqlConnection, const char *sql, int fd);

​	在sqlConnection所对应的数据库中执行sql语言并将其输出到fd中

MYSQL_free

​	void MYSQL_free(MYSQL *sqlConnection);

​	释放sqlConnection

## Core Layer 核心层

------

未实现：日志输出(调试日志和系统日志)\字符串类型\开根号\日期\图\哈希表\循环队列\红黑树\hashMap

### Link 链表

**数据结构**

```c
// 链表节点
typedef struct LinkNode {
    void *n_value;              //节点值
    struct LinkNode *n_next;    //next节点
} LinkNode;

// 链表
typedef struct Link {
    int n_linkNodeNum;  //链表节点数
    LinkNode *n_head;   //头节点
    LinkNode *n_tail;   //头节点
} Link;
```

**LinkNode_init**

​	LinkNode *LinkNode_init(void *value);

​	将value作为n_value创建新的LinkNode，成功返回新建的LinkNode，失败返回NULL

**LinkNode_print**

​	void LinkNode_print(LinkNode *ln, void (*fun_printValue)(void *));

​	以fun_printValue函数方法输出ln的n_value值

**LinkNode_free**

​	void LinkNode_free(LinkNode *ln, void (*fun_freeValue)(void *));

​	以fun_freeValue的方式释放ln的n_value 并 释放LinkNode

**Link_init**

​	Link *Link_init();

​	创建新的Link，成功返回新建的Link，失败返回NULL

**Link_disPlayNode**

​	void Link_disPlayNode(Link *l, void (*fun_printValue)(void *));

​	遍历Link的Node，并以fun_printValue的方法输出各个Node的n_value值

**Link_getLength**

​	int Link_getLength(Link *l);

​	返回Link的n_linkNodeNum值，如果l为NULL则返回-1

**Link_queryValue**

​	int Link_queryValue(Link *l, void *value, int (*fun_compareValue)(void *, void *));

​	以fun_compareValue方法比较value值是否一致以查询l里是否存在value值，成功则返回该节点所在数，失败则返回0，如果l为NULL则返回-1

**Link_insertNode**

​	int Link_insertNode(Link *l, int nodeNum, LinkNode *newln);

​	将newln插入到l的第nodeNum个节点处(从0开始)，成功返回1，失败返回0，如果l为NULL则返回-1

**Link_insertValue**

​	int Link_insertValue(Link *l, int nodeNum, void *value);

​	新建一个值为value的newln插入到l的第nodeNum个节点处(从0开始)，成功返回1，失败返回0，如果l为NULL则返回-1

**Link_headInsertNode**

​	int Link_headInsertNode(Link *l, LinkNode *newln);

​	将ln插入l的头节点，成功返回1，失败返回0，如果l或newln为NULL则返回-1（失败的唯一方法为l或newln为NULL）

**Link_headInsertValue**

​	int Link_headInsertValue(Link *l, void *value);

​	新建一个值为value的ln插入l的头节点，成功返回1，失败返回0，如果l或newln为NULL则返回-1（失败的唯一方法为l或	newln为NULL）

**Link_tailInsertNode**

​	int Link_tailInsertNode(Link *l, LinkNode *newln);

​	将ln插入l的尾节点，成功返回1，失败返回0，如果l或newln为NULL则返回-1（失败的唯一方法为l或newln为NULL）

**Link_tailInsertValue**

​	int Link_tailInsertValue(Link *l, void *value);

​	新建一个值为value的ln插入l的尾节点，成功返回1，失败返回0，如果l或newln为NULL则返回-1（失败的唯一方法为l或newln为NULL）

**Link_getValue**

​	void *Link_getValue(Link *l, int nodeNum);

​	获取l中第nodeNum个节点的n_value值，失败返回NULL

**Link_deleteNode**

​	void Link_deleteNode(Link *l, int nodeNum, void (*fun_freeValue)(void *));

​	删除l中第nodeNum个节点(从0开始)，并以fun_freeValue方法释放其n_value值

**Link_free**

​	void Link_free(Link *l, void (*fun_freeValue)(void *));

​	以fun_freeValue的方式释放l中每个节点的n_value 并 释放各个节点 最后 释放l

------

### Tree AVL树

**数据结构**

```c
// 树节点
typedef struct TreeNode {
    int n_height;               //树高
    void *n_value;              //树节点值
    struct TreeNode *n_lChild;  //左子树节点
    struct TreeNode *n_rChild;  //右子树节点
} TreeNode;

// AVL树
typedef struct Tree {
    int n_treeNodeNum;  //树节点数
    TreeNode *n_root;     //根节点
} Tree;
```

**TreeNode_init**

​	TreeNode *TreeNode_init(void *value);

​	创建一个n_value值为value的TreeNode,成功返回TreeNode,失败返回NULL

**TreeNode_print**

​	void TreeNode_print(TreeNode *tn, void (*fun_printValue)(void *));

​	以fun_printValue函数方法输出tn的n_value值

**TreeNode_free**

​	void TreeNode_free(TreeNode *tn, void (*fun_freeValue)(void *));

​	以fun_freeValue的方式释放tn的n_value值并释放tn

**Tree_init**

​	Tree *Tree_init();

​	创建Tree,成功返回新建的Tree,失败返回NULL

**Tree_insertNode**

​	int Tree_insertNode(Tree *t, TreeNode *newtn, int (*fun_compareValue)(void *, void *));

​	将newtn插入t中,其间需要用fun_compareValue方法比较大小,成功返回0,失败返回-1

**Tree_insertValue**

​	int Tree_insertValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));

​	新建一个值为value的TreeNode插入到t中,期间需要用fun_compareValue方法比较大小,成功返回0,失败返回-1

**Tree_deleteValue**

​	int Tree_deleteValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *), void (*fun_freeValue)(void *));

​	删除和value一样的值,以fun_compareValue方法寻找,以fun_freeValue方法释放value,成功返回0,失败返回-1

**Tree_isHave**

​	int Tree_isHave(Tree *t, void *value, int (*fun_compareValue)(void *, void *));

​	判断树中有没有和value一样的值,以fun_compareValue方法寻找,成功返回1,失败返回0

**Tree_getValue**

​	void *Tree_getValue(Tree *t, void *value, int (*fun_compareValue)(void *, void *));ls

​	在t中寻找并返回和value一样的值,以fun_compareValue方法比较大小,成功返回该值,失败返回NULL

**Tree_inorderTraverse**

​	void Tree_inorderTraverse(Tree *t, void (*fun_printValue)(void *));

​	树的中序遍历

**Tree_preorderTraverse**

​	void Tree_preorderTraverse(Tree *t, void (*fun_printValue)(void *));

​	树的前序遍历

**Tree_postorderTraverse**

​	void Tree_postorderTraverse(Tree *t, void (*fun_printValue)(void *));

​	树的后序遍历

**Tree_levelTraverse**

​	void Tree_levelTraverse(Tree *t, void (*fun_printValue)(void *));

​	利用链表队列进行树的层序遍历

**Tree_free**

​	void Tree_free(Tree *t, void (*fun_freeValue)(void *));

树的释放

------

### Json  json数据

对象为AVL树(coreTree)存储,数组为Link(coreLink)链表存储

**数据结构**

```c
// 判断json值类型
#define JSONINT 0
#define JSONDOUBLE 1
#define JSONSTR 2
#define JSONBOOL 3
#define JSONNULL 4
#define JSONNUMS 5      //Link结构
#define JSONOBJ 6       //AVL树结构

// json值
typedef union jsonData {
    int i_data;     // 整数
    double d_data;  // 浮点数
    int b_data;     // 布尔值
    void *p_data;   // null 或 数组 或 对象 或 字符串
} jsonData;

// 联合体JSON数据值
typedef struct jsonValue {
    char *n_name;           // 键名
    int n_dataType;         // 数据类型
    jsonData n_data;        // json值
} jsonValue;

// AVL树型结构
typedef Tree Json;
```

**jsonValue_init**

​	jsonValue *jsonValue_init(char *name, jsonData jd, int dt);

​	创建一个相应的jsonValue,失败返回NULL

**jsonValue_print**

​	void jsonValue_print(jsonValue *jv);

​	输出jsonValue值

**jsonValue_free**

​	void jsonValue_free(jsonValue *jv);

​	jsonValue的释放

**Json_init**

​	Json *Json_init();

​	创建一个Json,失败返回NULL

**Json_stringInit**

​	Json *Json_stringInit(char *jsonStr);

​	根据一个Json格式字符串创建一个Json,失败返回NULL

**Json_insertValue**

​	int Json_insertValue(Json *j, jsonValue *jv);

​	将jv插入j中,成功返回0,失败返回-1

**Json_getValue**

​	jsonValue *Json_getValue(Json *j, char *queryJson);

​	根据queryJson查找相应的jsonValue值,失败返回NULL

​	queryJson的格式应为{"ObjName"{"numsName"[num{"ValueName"}]}}

**Json_displayValue**

​	void Json_displayValue(Json *j);

​	中序遍历j 其中nums值为遍历链表

**Json_free**

​	void Json_free(Json *j);

​	json的释放

------

### PthreadPool 线程池

**数据结构**

```c
// 任务链表接收池
typedef struct taskLinkPool {
    Link *n_link;
    pthread_mutex_t n_lock;
    pthread_cond_t n_haveTask;
} taskLinkPool;

// 任务avl树节点值
typedef struct taskTreePoolValue {
    int fd;
    int isRun;
} taskTreePoolValue;

// 任务avl树判断池
typedef struct taskTreePool {
    Tree *n_tree;
    pthread_mutex_t n_lock;
} taskTreePool;
```

**taskLinkPool_init**

​	taskLinkPool *taskLinkPool_init();

​	创建一个新的线程link池(队列),用于记录需要服务的fd,失败返回NULL

**taskLinkPool_push**

​	int taskLinkPool_push(taskLinkPool *tlp, int fd);

​	将fd加入到线程队列中,成功返回1,失败返回-1

**taskLinkPool_pop**

​	int taskLinkPool_pop(taskLinkPool *tlp);

​	将队头的fd取出,成功返回正常fd,失败返回-1

**taskLinkPool_print**

​	void taskLinkPool_print(taskLinkPool *tlp);

​	遍历线程池队列

**taskLinkPool_free**

​	void taskLinkPool_free(taskLinkPool *tlp);

​	线程池的释放

**taskTreePool_init**

​	taskTreePool *taskTreePool_init();

​	创建一个新的线程Tree判断池,用于判断某个fd是否正在被服务,失败返回NULL

**taskTreePool_push**

​	int taskTreePool_push(taskTreePool *ttp, int fd);

​	如果fd已在线程判断池中,则将其isRun置为1,否则添加新的isRun为1的相应节点,成功返回0,失败返回-1

**taskTreePool_pop**

​	void taskTreePool_pop(taskTreePool *ttp, int fd);

​	将线程判断池中相应的节点的isRun置为0

**taskTreePool_print**

​	void taskTreePool_print(taskTreePool *ttp);

​	遍历线程判断池

**taskTreePool_free**

​	void taskTreePool_free(taskTreePool *ttp);

​	线程判断池的释放



**废弃**

**~~数据结构~~**

```c
// 任务节点
typedef struct taskNode {
    int fd;
    struct taskNode *next;
} taskNode;

// 线程池 (任务池)
typedef struct taskPool {
    taskNode *head;
    taskNode *tail;
    pthread_mutex_t lock;
    pthread_cond_t haveTask;
} taskPool;
```

**~~taskPool_init~~**

​	void taskPool_init(taskPool **);

​	线程池的创建

**~~taskPool_push~~**

​	void taskPool_push(taskPool *, int);

​	线程池加入新任务

**~~taskPool_pop~~**

​	void taskPool_pop(taskPool *, int *);

​	线程池因完成任务将任务退出

**~~taskPool_free~~**

​	void taskPool_free(taskPool *);

​	释放线程池

**~~taskPool_print~~**

​	void taskPool_print(taskPool *);

​	线程池输出任务列表

------

### Md5 MD5算法

**rol**

​	 bit32 rol(bit32 val, int size);

​	循环左移

**md5**

​	char *md5(char *data, int bitLen);

​	输出data的md5值

------

**章逸飞版权所有**