#include <coreIterator.h>
#include <coreJson.h>
#include <coreLink.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* 内置函数 */

// 比较两个jv的大小 strcmp两个jv的n_name
int compareJVToJV(void *a, void *b);

// 比较字符串和jv的大小 strcmp字符串和jv的n_name
int compareStringToJV(void *a, void *b);

// 读取一部分字符串并返回一个dataType为JSONINT或JSONDOUBLE的jsonData
jsonData jsonData_return_num(char *jsonStr, int *index, int *fd);

// 读取一部分字符串并返回一个用于作为一个dataType为JSONSTR的jsonData的n_data值
char *jsonData_return_str(char *jsonStr, int *index);

// 读取一部分字符串并返回一个相应的jsonValue
jsonValue *jsonValue_creat(char *jsonStr, int *index, int cond);

// 读取一部分字符串并返回一个相应的Link
Link *Json_Nums_init(char *jsonStr, int *index);

// 读取一部分字符串并返回一个相应的Json
Json *Json_Obj_init(char *jsonStr, int *index);

// 无实际意义
void Json_print_in(void *j, int cond);

// 从j中获取相应的jsonValue
jsonValue *Json_getObjValue(Json *j, char *scStr, int *index);

// 从l中获取相应的jsonValue
jsonValue *Json_getNumsValue(Link *l, char *scStr, int *index);

/* 内置函数 */

int compareJVToJV(void *a, void *b) {
    return strcmp(((jsonValue *)a)->n_name, ((jsonValue *)b)->n_name);
}

int compareStringToJV(void *a, void *b) {
    return strcmp((char *)a, ((jsonValue *)b)->n_name);
}

void jsonValue_free(void *a) {
    jsonValue *jv= (jsonValue *)a;
    if (jv == NULL) return ;
    
    if (jv->n_name != NULL) free(jv->n_name);
    if (jv->n_dataType == JSONSTR)
        free((char *)jv->n_data.p_data);
    else if (jv->n_dataType == JSONOBJ)
        Json_free((Json *)jv->n_data.p_data);
    else if (jv->n_dataType == JSONNUMS)
        Link_free((Link *)jv->n_data.p_data);
    free(jv);

    return ;
}

Json *Json_init() {
    return Tree_init(compareJVToJV, jsonValue_free);
}

int Json_insertValue(Json *j, jsonValue *jv) {
    return Tree_insertValue(j, (void *)jv);
}

void Json_free(Json *j) {
    Tree_free(j);
}

jsonValue *Json_getObjValue(Json *j, char *scStr, int *index) {
    for (; scStr[(*index)] == ' ' || scStr[(*index)] == '\n' || scStr[(*index)] == '\r'; (*index)++);

    if (scStr[*index] == '{') {
        (*index)++;
        int tmp;
        for (tmp = *index; scStr[tmp] != '[' && scStr[tmp] != '{' && scStr[tmp] != '}' && scStr[tmp] != ']' && scStr[tmp] != ' ' && scStr[tmp] != '\0'; tmp++);
        char *searchName = jsonData_return_str(scStr, index);
        if (searchName == NULL) return NULL;
        jsonValue *jv = (jsonValue *)Tree_getValue(j, (void *)searchName, compareStringToJV);
        if (searchName != NULL) free(searchName);
        if (jv == NULL) return NULL;
        *index = tmp;
        for (; scStr[(*index)] == ' ' || scStr[(*index)] == '\n' || scStr[(*index)] == '\r'; (*index)++);
        if (jv->n_dataType == JSONOBJ && scStr[*index] == '{')
            return Json_getObjValue((Json *)jv->n_data.p_data, scStr, index);
        if (jv->n_dataType == JSONNUMS && scStr[*index] == '[')
            return Json_getNumsValue((Link *)jv->n_data.p_data, scStr, index); 
        else
            return jv;
    } else {
        return NULL;
    }
}

jsonValue *Json_getNumsValue(Link *l, char *scStr, int *index) {
    for (; scStr[(*index)] == ' ' || scStr[(*index)] == '\n' || scStr[(*index)] == '\r'; (*index)++);
    
    if (scStr[*index] == '[') {
        (*index)++;
        int num = 0;
        for (; scStr[*index] >= '0' && scStr[*index] <= '9'; (*index)++) num = num * 10 + (scStr[*index] - '0');
        jsonValue *jv = (jsonValue *)Link_getValue(l, num);
        if (jv == NULL) return NULL;
        for (; scStr[(*index)] == ' ' || scStr[(*index)] == '\n' || scStr[(*index)] == '\r'; (*index)++);
        if (jv->n_dataType == JSONOBJ && scStr[*index] == '{')
            return Json_getObjValue((Json *)jv->n_data.p_data, scStr, index);
        if (jv->n_dataType == JSONNUMS && scStr[*index] == '[')
            return Json_getNumsValue((Link *)jv->n_data.p_data, scStr, index); 
        else
            return jv;
    } else {
        return NULL;
    }
}


jsonValue *Json_getValue(Json *j, char *queryJson) {
    if (!j || !queryJson) return NULL;

    int index = 0;
    return Json_getObjValue(j, queryJson, &index);
}

jsonData jsonData_return_num(char *jsonStr, int *index, int *fd) {
    int i;
    jsonData ret;
    ret.i_data = 0;
    for (; jsonStr[*index] >= '0' && jsonStr[*index] <= '9'; (*index)++)
        ret.i_data = ret.i_data * 10 + (jsonStr[*index] - '0');
    *fd = JSONINT;
    if (jsonStr[*index] == '.') {
        *fd = JSONDOUBLE;
        (*index)++;
        ret.d_data = (double)ret.i_data;
        double l = 0, tmp = 10;
        for (; jsonStr[*index] >= '0' && jsonStr[*index] <= '9'; (*index)++, tmp *= 10)
            l = l + (jsonStr[*index] - '0') / tmp;
        ret.d_data += l;
    }
    return ret;
}

jsonValue *jsonValue_init(char *name, jsonData jd, int dt) {
    if (dt > 6 || dt < 0) return NULL;
    
    jsonValue *jv = (jsonValue *)malloc(sizeof(jsonValue));

    jv->n_name = name;
    jv->n_dataType = dt;
    jv->n_data = jd;

    return jv;
}

char *jsonData_return_str(char *jsonStr, int *index) {
    if (jsonStr[(*index)++] != '\"') return NULL;
    int tmp = (*index);
    while (jsonStr[(*index)] != '\"')
        if (jsonStr[(*index)++] == '\0') return NULL;
    char *retStr = (char *)malloc(sizeof(char) * ((*index) - tmp));
    int i;
    for (i = 0; jsonStr[tmp] != '\"'; i++)
        retStr[i] = jsonStr[tmp++];
    (*index)++;
    retStr[i] = '\0';
    return retStr;
}

jsonValue *jsonValue_creat(char *jsonStr, int *index, int cond) {
    for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r'; (*index)++);

    char *name;
    if (cond) {
        name = jsonData_return_str(jsonStr, index);
        if (!name) return NULL;

        for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r' || jsonStr[(*index)] == ':'; (*index)++);
    } else {
        name = NULL;
    }

    int dt;
    jsonData jd;
    if (jsonStr[(*index)] >= '0' && jsonStr[(*index)] <= '9') {
        jd = jsonData_return_num(jsonStr, index, &dt);
    } else if (jsonStr[(*index)] == '\"') {
        dt = JSONSTR;
        jd.p_data = (void *)jsonData_return_str(jsonStr, index);
    } else if (jsonStr[(*index)] == 't' || jsonStr[(*index)] == 'f') { 
        // bool值只支持小写 暂时
        dt = JSONBOOL;
        if (strncmp(jsonStr + (*index), "true", 4) == 0) {
            jd.p_data = (void *)"true";
            (*index) += 4;
        } else if (strncmp(jsonStr + (*index), "false", 5) == 0) {
            jd.p_data = (void *)"false";   
            (*index) += 5;
        } else {
            free(name);
            return NULL;
        }
    } else if (jsonStr[(*index)] == 'n') {
        dt = JSONNULL;
        if (strncmp(jsonStr + (*index), "null", 4) == 0) {
            jd.p_data = (void *)"null";
            (*index) += 4;
        } else {
            free(name);
            return NULL;
        }
    } else if (jsonStr[(*index)] == '[') {
        dt = JSONNUMS;
        jd.p_data = (void *)Json_Nums_init(jsonStr, index);
    } else if (jsonStr[(*index)] == '{') {
        dt = JSONOBJ;
        jd.p_data = (void *)Json_Obj_init(jsonStr, index);
    } else {
        free(name);
        return NULL;
    }
    
    return jsonValue_init(name, jd, dt);
}

Link *Json_Nums_init(char *jsonStr, int *index) {
    for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r'; (*index)++); //剔除空格
    if (jsonStr[(*index)] != '[')
        return NULL;
    Link *l = Link_init(jsonValue_free);
    (*index)++;
    while (1) {
        for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r'; (*index)++);
        jsonValue *myjv = jsonValue_creat(jsonStr, index, False);
        Link_tailInsertValue(l, (void *)myjv);
        for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r'; (*index)++);
        if (jsonStr[(*index)] == ',') {
            (*index)++;
            continue;
        } else if (jsonStr[(*index)] == ']') {
            (*index)++;
            return l; 
        } else {
            Link_free(l);
            return NULL;
        }
    }
}

Json *Json_Obj_init(char *jsonStr, int *index) {
    for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r'; (*index)++); //剔除空格
    if (jsonStr[(*index)] != '{')
        return NULL;

    Json *j = Json_init();
    (*index)++;
    while (1) {
        for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r'; (*index)++);
        Json_insertValue(j, jsonValue_creat(jsonStr, index, True));
        for (; jsonStr[(*index)] == ' ' || jsonStr[(*index)] == '\n' || jsonStr[(*index)] == '\r'; (*index)++);
        if (jsonStr[(*index)] == ',') {
            (*index)++;
            continue;
        } else if (jsonStr[(*index)] == '}') {
            (*index)++;
            return j; 
        } else {
            Json_free(j);
            return NULL;
        }
    }
}    

Json *Json_stringInit(char *jsonStr) {
    if (!jsonStr)
        return NULL;
    int index = 0;
    return Json_Obj_init(jsonStr, &index);
}

void jsonValue_print(jsonValue *jv) {
    if (!jv) return ;
    if (jv->n_name) printf("\"%s\" : ", jv->n_name);
    if (jv->n_dataType == JSONINT) {
        printf("%d", jv->n_data.i_data);
    } else if (jv->n_dataType == JSONDOUBLE) {
        printf("%lf", jv->n_data.d_data);
    } else if (jv->n_dataType == JSONSTR) {
        printf("\"%s\"", (char *)jv->n_data.p_data);
    } else if (jv->n_dataType == JSONBOOL || jv->n_dataType == JSONNULL) {
        printf("%s", (char *)jv->n_data.p_data);
    } else if (jv->n_dataType == JSONNUMS) {
        Json_print_in(jv->n_data.p_data, 1);
    } else if (jv->n_dataType == JSONOBJ) {
        Json_print_in(jv->n_data.p_data, 0);
    }
}

void printJsonValue(void *a) {
    jsonValue *jv = (jsonValue *)a;
    if (!jv) return ;
    if (jv->n_name) printf("\"%s\" : ", jv->n_name);
    if (jv->n_dataType == JSONINT) {
        printf("%d", jv->n_data.i_data);
    } else if (jv->n_dataType == JSONDOUBLE) {
        printf("%lf", jv->n_data.d_data);
    } else if (jv->n_dataType == JSONSTR) {
        printf("\"%s\"", (char *)jv->n_data.p_data);
    } else if (jv->n_dataType == JSONBOOL || jv->n_dataType == JSONNULL) {
        printf("%s", (char *)jv->n_data.p_data);
    } else if (jv->n_dataType == JSONNUMS) {
        Json_print_in(jv->n_data.p_data, 1);
    } else if (jv->n_dataType == JSONOBJ) {
        Json_print_in(jv->n_data.p_data, 0);
    }
    printf(", ");
}

void Json_print_in(void *j, int cond) {
    if (!j) {
        printf("Format error!\n");
        return ;
    }

    if (cond) {
        printf("[");
        Iterator *i = Link_getIterator((Link *)j);
        while (i->fun_hasNext(i))
            jsonValue_print((jsonValue *)i->fun_next(i));
        Iterator_free(i);
        printf("\b\b]");
    } else {
        printf("{");
        Iterator *i = Tree_getInorderIterator((Json *)j);
        while (i->fun_hasNext(i))
            jsonValue_print((jsonValue *)i->fun_next(i));
        Iterator_free(i);
        printf("\b\b}");
    }

    return ;
}

void Json_displayValue(Json *j) {
    Json_print_in((void *)j, 0);

    return ;
}
