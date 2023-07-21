#include <coreIterator.h>
#include <coreTree.h>
#include <networkHttpHeader.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <pthread.h>

int compareRhvToRhv(void *a, void *b);
int compareStringToRhv(void *a, void *b);
void HttpHeader_print_in(void *a);
void HttpHeader_fd_print_in(void *a, void *fd);
void HttpHeader_free_in(void *a);

int compareRhvToRhv(void *a, void *b) {
    return strcmp(((requestHeaderValue *)a)->n_header, ((requestHeaderValue *)b)->n_header);
}

int compareStringToRhv(void *a, void *b) {
    return strcmp((char *)a, ((requestHeaderValue *)b)->n_header);
}

requestHeaderValue *requestHeaderValue_init(char *header, char *value) {
    requestHeaderValue *rhv = (requestHeaderValue *)malloc(sizeof(requestHeaderValue));
    rhv->n_header = header;
    rhv->n_value = value;

    return rhv;
}

void requestHeaderValue_printf(requestHeaderValue *rhv) {
    if (rhv == NULL) return ;

    printf("%s:%s\r\n", rhv->n_header, rhv->n_value);

    return ;
}

void requestHeaderValue_fd_printf(requestHeaderValue *rhv, int fd) {
    if (rhv == NULL || fd < 0) return ;

    dprintf(fd, "%s:%s\r\n", rhv->n_header, rhv->n_value);

    return ;
}

void requestHeaderValue_free(requestHeaderValue *rhv) {
    if (rhv == NULL) return ;

    if (rhv->n_header != NULL) free(rhv->n_header);
    if (rhv->n_value != NULL) free(rhv->n_value);
    free(rhv);

    return ;
}

HttpHeader *HttpHeader_init() {
    HttpHeader *hh = (HttpHeader *)malloc(sizeof(HttpHeader));
    hh->n_requestLine = NULL;
    hh->n_requestHeader = Tree_init(compareRhvToRhv, HttpHeader_free_in);

    return hh;
}

HttpHeader *HttpHeader_fd_init(int fd) {
    char tmp, buf[1024];
    int index = 0, isFree = read(fd, &tmp, 1);
    for (; tmp != '\r' && tmp != EOF; index++, isFree = read(fd, &tmp, 1)) {
        if (isFree <= 0)
            return NULL;
        buf[index] = tmp;
    }
    if (tmp == EOF) return NULL;
    isFree = read(fd, &tmp, 1);
    if (isFree <= 0)
        return NULL;
    if (tmp != '\n') return NULL;
    buf[index++] = '\0';
    char *requestLine = (char *)malloc(sizeof(char) * index);
    strncpy(requestLine, buf, index);
    HttpHeader *hh = HttpHeader_init();
    hh->n_requestLine = requestLine;
    //printf("[%ld]requestLine : %s\n", pthread_self(), requestLine);

    while (1) {
        index = 0;
        int judge = 0; // 空格判断
        for (isFree = read(fd, &tmp, 1); isFree > 0 && tmp != '\r' && tmp != ':' && tmp != EOF; isFree = read(fd, &tmp, 1)) {
            if (tmp != ' ') judge = 1;
            if (judge) buf[index++] = tmp;
        }
        if (isFree <= 0 || tmp == EOF) break;
        if (tmp == '\r') {
            isFree = read(fd, &tmp, 1);
            if (isFree <= 0 || tmp != '\n') break;
            else return hh;
        }
        buf[index++] = '\0';
        char *header = (char *)malloc(sizeof(char) * index);
        strncpy(header, buf, index);

        index = 0;
        judge = 0;
        for (isFree = read(fd, &tmp, 1); isFree > 0 && tmp != '\r' && tmp != EOF; isFree = read(fd, &tmp, 1)) {
            if (tmp != ' ') judge = 1;
            if (judge) buf[index++] = tmp;
        }
        if (isFree <= 0 || tmp == EOF) break;
        isFree = read(fd, &tmp, 1);
        if (isFree <= 0 || tmp != '\n') break;
        buf[index++] = '\0';
        char *value = (char *)malloc(sizeof(char) * index);
        strncpy(value, buf, index);

        requestHeaderValue *rhv = requestHeaderValue_init(header, value);
        HttpHeader_insertRequestHeader(hh, rhv);
        //printf("[%ld]%s : %s\n", pthread_self(), header, value);
    }

    HttpHeader_free(hh);
    return NULL;
}

int HttpHeader_insertRequestHeader(HttpHeader *hh, requestHeaderValue *rhv) {
    if (hh == NULL || rhv == NULL) return -1;

    return Tree_insertValue(hh->n_requestHeader, rhv);
}

char *HttpHeader_getRequestHeader(HttpHeader *hh, char *header) {
    if (hh == NULL || header == NULL) return NULL;

    requestHeaderValue *rhv = (requestHeaderValue *)Tree_getValue(hh->n_requestHeader, (void *)header, compareStringToRhv);
    if (rhv == NULL) return NULL;
    return rhv->n_value;
}

void HttpHeader_print_in(void *a) {
    requestHeaderValue_printf((requestHeaderValue *)a);

    return ;
}

void HttpHeader_print(HttpHeader *hh) {
    if (hh == NULL) return ;

    if (hh->n_requestLine != NULL) printf("%s\n", hh->n_requestLine);
    else return ;
    
    Iterator *i = Tree_getPreorderIterator(hh->n_requestHeader);
    while (i->fun_hasNext(i))
        HttpHeader_print_in(i->fun_next(i));
    Iterator_free(i);
    printf("\r\n");

    return ;
}

void HttpHeader_fd_print_in(void *a, void *fd) {
    requestHeaderValue_fd_printf((requestHeaderValue *)a, *(int *)fd);

    return ;
}

void HttpHeader_fd_print(HttpHeader *hh, int fd) {
    if (hh == NULL) return ;

    if (hh->n_requestLine != NULL) dprintf(fd, "%s", hh->n_requestLine);
    else return ;

    Iterator *i = Tree_getInorderIterator(hh->n_requestHeader);
    while (i->fun_hasNext(i))
        HttpHeader_fd_print_in(i->fun_next(i), (void *)&fd);
    Iterator_free(i);
    dprintf(fd, "\r\n");

    return ;
}

void HttpHeader_free_in(void *a) {
    requestHeaderValue_free((requestHeaderValue *)a);

    return ;
}

void HttpHeader_free(HttpHeader *hh) {
    if (hh == NULL) return ;

    if (hh->n_requestLine != NULL) free(hh->n_requestLine);
    Tree_free(hh->n_requestHeader);

    return ;
}
