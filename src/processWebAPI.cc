#include <stdlib.h>
#include <string.h>
#include <mysql/mysql.h>
#include <coreTree.h>
#include <coreJson.h>
#include <processWebAPI.h>

int APITreeValueCompare(void *a, void *b) {
    return strcmp(((APITreeValue *)a)->n_APIName, ((APITreeValue *)b)->n_APIName);
}

int APINameCompareAPITreeValue(void *a, void *b) {
    return strcmp((const char *)a, ((APITreeValue *)b)->n_APIName);
}

void APITreeValueFree(void *a) {
    if (a == NULL) return ;

    free((APITreeValue *)a);
    return ;
}

APITree *APITree_init() {
    APITree *at = Tree_init(APITreeValueCompare, APITreeValueFree);
    
    return at;
}

int APITree_push(APITree *at, const char *APIName, int (*fun_APIProcess)(Json *, int, MYSQL *)) {
    if (at == NULL || APIName == NULL || fun_APIProcess == NULL) return -1;

    APITreeValue *atv = (APITreeValue *)malloc(sizeof(APITreeValue));
    atv->n_APIName = APIName;
    atv->fun_APIProcess = fun_APIProcess;

    return Tree_insertValue(at, (void *)atv);
}

APITreeValue *APITree_getValue(APITree *at, const char *APIName) {
    if (at == NULL || APIName == NULL) return NULL; 

    return (APITreeValue *)Tree_getValue(at, (void *)APIName, APINameCompareAPITreeValue);
}

void APITree_free(APITree *at) {
    Tree_free(at);
}
