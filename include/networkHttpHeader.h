#ifndef _NETWORKHTTPHEADER_H
#define _NETWORKHTTPHEADER_H

#include <coreTree.h>

typedef struct requestHeaderValue {
    char *n_header;
    char *n_value;
} requestHeaderValue;

typedef struct HttpHeader {
    char *n_requestLine;
    Tree *n_requestHeader;
} HttpHeader;

// requestHeaderValue
requestHeaderValue *requestHeaderValue_init(char *header, char *value);
void requestHeaderValue_print(requestHeaderValue *rhv);
void requestHeaderValue_fd_print(requestHeaderValue *rhv, int fd);
void requestHeaderValue_free(requestHeaderValue *rhv);

// HttpHeader
HttpHeader *HttpHeader_init();
HttpHeader *HttpHeader_fd_init(int fd);
int HttpHeader_insertRequestHeader(HttpHeader *hh, requestHeaderValue *rhv);
char *HttpHeader_getRequestHeader(HttpHeader *hh, char *header);
void HttpHeader_print(HttpHeader *hh);
void HttpHeader_fd_print(HttpHeader *hh, int fd);
void HttpHeader_free(HttpHeader *hh);

#endif

/*
 *  requestHeaderValue
 *      requestHeaderValue *requestHeaderValue_init(char *header, char *value);
 *          创建相应的一条消息头,失败返回NULL
 *      void requestHeaderValue_print(requestHeaderValue *rhv);
 *          输出消息头内容
 *      void requestHeaderValue_free(requestHeaderValue *rhv);
 *          释放消息头结构
 *  HttpHeader
 *      HttpHeader *HttpHeader_init();
 *          创建一个空的http协议结构,失败返回NULL
 *      HttpHeader *HttpHeader_fd_init(int fd);
 *          根据一个文件描述符创建相应的http结构,失败返回NULL
 *      int HttpHeader_insertRequestHeader(HttpHeader *hh, requestHeaderValue *rhv);
 *          插入新的消息头结构,成功返回0.失败返回-1
 *      void HttpHeader_print(HttpHeader *hh);
 *          输出一个http协议结构
 *      void HttpHeader_free(HttpHeader *hh);
 *          释放一个http协议结构
 */
