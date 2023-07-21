#ifndef PROCESSWEBAPI_H
#define PROCESSWEBAPI_H

#include <mysql/mysql.h>
#include <coreTree.h>
#include <coreJson.h>

typedef struct APITreeValue {
    const char *n_APIName;
    int (*fun_APIProcess)(Json *, int, MYSQL *);
} APITreeValue;

typedef Tree APITree;

APITree *APITree_init();
int APITree_push(APITree *at, const char *APIName, int (*fun_APIProcess)(Json *, int, MYSQL *));
APITreeValue *APITree_getValue(APITree *at, const char *APIName);
void APITree_free(APITree *at);

#endif
