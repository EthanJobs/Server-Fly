#ifndef _COREJSON_h
#define _COREJSON_H

#include <coreTree.h>

// bool值为int
#define False 0
#define True 1

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

/* jsonValue */
jsonValue *jsonValue_init(char *name, jsonData jd, int dt);
void jsonValue_print(jsonValue *jv);
void jsonValue_free(jsonValue *jv);

/* Json */
Json *Json_init();
Json *Json_stringInit(char *jsonStr);
int Json_insertValue(Json *j, jsonValue *jv);
jsonValue *Json_getValue(Json *j, char *queryJson);
void Json_displayValue(Json *j);
void Json_free(Json *j);

#endif

/*
 *  jsonValue
 *      jsonValue *jsonValue_init(char *name, jsonData jd, int dt);
 *          创建一个相应的jsonValue,失败返回NULL
 *
 *      void jsonValue_print(jsonValue *jv);
 *          输出jsonValue值
 *
 *      void jsonValue_free(jsonValue *jv);
 *          jsonValue的释放
 *
 *  Json
 *      Json *Json_init();
 *          创建一个Json,失败返回NULL
 *
 *      Json *Json_stringInit(char *jsonStr);
 *          根据一个Json格式字符串创建一个Json,失败返回NULL
 *
 *      int Json_insertValue(Json *j, jsonValue *jv);
 *          将jv插入j中,成功返回0,失败返回-1
 *
 *      jsonValue *Json_getValue(Json *j, char *queryJson);
 *          根据queryJson查找相应的jsonValue值,失败返回NULL
 *          queryJson的格式应为{"ObjName"{"numsName"[num{"ValueName"}]}}
 *
 *      void Json_displayValue(Json *j);
 *          中序遍历j 其中nums值为遍历链表
 *
 *      void Json_free(Json *j);
 *          json的释放
 */
