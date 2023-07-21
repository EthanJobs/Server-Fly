#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <mysql/mysql.h>
#include <coreJson.h>
#include <coreLog.h>
#include <resourceSQL.h>
#include <resourceFileCntl.h>
#include <networkPthreadPool.h>
#include <networkServer.h>
#include <networkHttpHeader.h>
#include <processMes.h>
#include <processCookie.h>

#include <defineHead.h>

#define paramCreate(param, type) \
    jsonValue * JV_##param = Json_getValue(json, (char *)"{\"##param\"}");\
    if (JV_##param == NULL || JV_##param != type)\
        return 0;

int processMes_in(taskTreePoolValue *ttpv, MYSQL *sqlConnection) {
    if (ttpv->hh == NULL) {
        ttpv->hh = HttpHeader_fd_init(ttpv->clientFd);
        if (ttpv->hh == NULL) return 0;
    }
    
    HttpHeader *hh = ttpv->hh;
    int fd = ttpv->clientFd;
 
    DBG("[THREAD : %ld]: Get %d HttpHeader:\n", pthread_self(), fd);
    if (DBGSIGN)
        HttpHeader_print(hh);
    
    if (strncmp(hh->n_requestLine, "GET ", 4) == 0) {
        int index = 4;
        for (; hh->n_requestLine[index] != 'H' && hh->n_requestLine[index] != '\0'; index++);
        if (hh->n_requestLine[index] == '\0')
            return 0;
        if (index - 5 == 1 && hh->n_requestLine[4] == '/') {
            // 请求主页
            if (ttpv->fileFd == -1) {
                ttpv->fileFd = open("./resource/index.html", O_RDONLY);

                if (ttpv->fileFd <= 0) {
                    dprintf(fd, "HTTP/1.1 404 Not Found\r\n\r\n");
                    return 0;
                }
    
                dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n"); 
            }

            if (writeFdFile(ttpv->fileFd, fd, &ttpv->fileIndex, BUFFER_SIZE) == -1) {
                if (errno == EAGAIN)
                    return 1;
                return 0;
            }
        } else {
            // 请求其他文件
            if (ttpv->fileFd == -1) {
                char *path = (char *)malloc(sizeof(char) * (index + 6));
                strncpy(path, "./resource/", 10);
                strncpy(path + 10, hh->n_requestLine + 4, index - 5);
                path[index + 5] = '\0';
                ttpv->fileFd = open(path, O_RDONLY);
                free(path);

                if (ttpv->fileFd <= 0) {
                    dprintf(fd, "HTTP/1.1 404 Not Found\r\n\r\n");
                    return 0;
                }

                int index;
                for (index = 4; hh->n_requestLine[index] != '.'; index++);
                printf("%s\n", hh->n_requestLine + index + 1);
                if (!strncmp("pdf", hh->n_requestLine + index + 1, 3))
                    dprintf(fd, "HTTP/1.1 200 OK\r\nContent-Type: application/pdf\r\nContent-Disposition:inline\r\n\r\n"); 
                else
                    dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n"); 
            }

            if (writeFdFile(ttpv->fileFd, fd, &ttpv->fileIndex, BUFFER_SIZE) == -1) {
                if (errno == EAGAIN)
                    return 1;
                return 0;
            }
        }

        return 0;
    } else if (strncmp(hh->n_requestLine, "POST /json HTTP/1.1", 19) == 0) {
        char buf[BUFFER_SIZE];
        int n = read(fd, buf, BUFFER_SIZE);
        
        // 接受json
        Json *json = Json_stringInit(buf);
        if (json == NULL)
            return 0;
        DBG("[THREAD : %ld]: Get %d LogInMes:\n", pthread_self(), fd);
        if (DBGSIGN) { 
            Json_displayValue(json);
            printf("\n");
        }
        
        // 判断jsonTask
        jsonValue *queryJVTask = Json_getValue(json, (char *)"{\"task\"}");
        if (!queryJVTask || queryJVTask->n_dataType != JSONSTR)
            return 0;

        char sql[SQL_MAX_SIZE];

        // 进入相应jsonTask
        if (!strncmp((char *)queryJVTask->n_data.p_data, "account_login", 13)) {
            // 账户登录
            jsonValue *JV_TI_no = Json_getValue(json, (char *)"{\"TI_no\"}");
            if (JV_TI_no == NULL || JV_TI_no->n_dataType != JSONSTR)
                return 0;
            jsonValue *JV_TI_password = Json_getValue(json, (char *)"{\"TI_password\"}");
            if (JV_TI_password == NULL || JV_TI_password->n_dataType != JSONSTR)
                return 0;
            jsonValue *JV_lastDate = Json_getValue(json, (char *)"{\"lastDate\"}");
            if (JV_lastDate == NULL || JV_lastDate->n_dataType != JSONSTR)
                return 0;

            char *md5_cookie = getCookie((char *)JV_TI_no->n_data.p_data, (char *)JV_TI_password->n_data.p_data, (char *)JV_lastDate->n_data.p_data);
            printf("\n!COOKIE[ %s ]!\n", md5_cookie);
            free(md5_cookie);
            sprintf(sql, "select TI_name from teacherInformation where TI_no = \'%s\' and TI_password = \'%s\';", (char *)JV_TI_no->n_data.p_data, (char *)JV_TI_password->n_data.p_data);
            dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n");
            sendSqlRes(sqlConnection, sql, fd);
            LOG("[THREAD : %ld]: TI_no(%s) try to login\n", pthread_self(), (char *)JV_TI_no->n_data.p_data);
        } else if (!strncmp((char *)queryJVTask->n_data.p_data, "account_query", 13)) {
            // 账户信息查询
            jsonValue *JV_TI_no = Json_getValue(json, (char *)"{\"TI_no\"}");
            if (JV_TI_no == NULL || JV_TI_no->n_dataType != JSONSTR)
                return 0;
            jsonValue *JV_TI_password = Json_getValue(json, (char *)"{\"TI_password\"}");
            if (JV_TI_password == NULL || JV_TI_password->n_dataType != JSONSTR)
                return 0;
            sprintf(sql, "select TI_no, TI_name, TI_title, TI_duties from teacherInformation where TI_no = \'%s\' and TI_password = \'%s\';", (char *)JV_TI_no->n_data.p_data, (char *)JV_TI_password->n_data.p_data);
            dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n");
            sendSqlRes(sqlConnection, sql, fd);
            LOG("[THREAD : %ld]: TI_no(%s) try to get accountInfo\n", pthread_self(), (char *)JV_TI_no->n_data.p_data);
        } else if (!strncmp((char *)queryJVTask->n_data.p_data, "fileGet", 7)) {
            // 账户信息查询
            jsonValue *JV_fileShowID = Json_getValue(json, (char *)"{\"fileShowID\"}");
            if (JV_fileShowID == NULL || JV_fileShowID->n_dataType != JSONSTR)
                return 0;
            sprintf(sql, "select F_id, F_name from files where F_type = %s;", (char *)JV_fileShowID->n_data.p_data);
            printf("%s", sql);
            dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n");
            sendSqlRes(sqlConnection, sql, fd);
        }
        
        return 0;
    } else if (strncmp(hh->n_requestLine, "POST /fileUpload HTTP/1.1", 19) == 0) {    
        char *fileType = HttpHeader_getRequestHeader(hh, (char *)"file-type");
        if (fileType == NULL) {
            dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n{\"error\":\"file-type\"}");
            return 0;
        }
        char *Content_Length = HttpHeader_getRequestHeader(hh, (char *)"Content-Length");
        if (Content_Length == NULL) {
            dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n{\"error\":\"content-length\"}");
            return 0;
        }
        char *upload_id = HttpHeader_getRequestHeader(hh, (char *)"upload-id");
        if (upload_id == NULL) {
            dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n{\"error\":\"upload-id\"}");
            return 0;
        }
        if (ttpv->fileFd == -1) {
            ttpv->fileIndex = strtol(Content_Length, NULL, 0);

            sleep(1);
            
            char fileName[100], buf;
            int index = 0;
            int len = read(fd, fileName, 97);
            fileName[98] = '\0';

            read(fd, &buf, 1);
            while (buf != '"') {
                fileName[index++] = buf;
                read(fd, &buf, 1);
            }
            fileName[index++] = '\0';

            ttpv->fileIndex -= 97 + index;
            
            long int fid = time(NULL);
            char *filePath = (char *)malloc(sizeof(char) * 37);
            sprintf(filePath, "./resource/file/%ld.pdf", fid);
        
            char sql[SQL_MAX_SIZE];
            
            sprintf(sql, "insert into files (F_id, F_name, F_type, F_TI_no) value ('%ld', '%s', %s, '%s');", fid, fileName, fileType, upload_id);
            sendSqlRes(sqlConnection, sql, 2);

            ttpv->fileFd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            free(filePath);
        }
        
        if (readFdFile(fd, ttpv->fileFd, &ttpv->fileIndex, FILE_UPLOAD_SIZE) == 0) {
            dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n{\"need_type\":%s,\"lost_type\":%d}", Content_Length, ttpv->fileIndex);
            LOG("[THREAD : %ld]: Uplod file %s\n", pthread_self(), "upload001.pdf");
            return 0;
        } else {
            if (errno == EAGAIN)
                return 2;
            return 0;
        }
    }     
    dprintf(fd, "HTTP/1.1 200 OK\r\n\r\n{\"error\":\"0\"}");
    
    return 0;
}

void *processMes(void *arg) {
    int index;
    MYSQL *sqlConnection = MYSQL_init(HOST, USERNAME, PASSWORD, DATABASE, 0);
    taskQueuePool *tqp = ((pthreadARG *)arg)->tqp;
    taskTreePool *ttp = ((pthreadARG *)arg)->ttp;
    int epfd = ((pthreadARG *)arg)->epfd;

    while (1) {
        /*接收客户端信息*/
        taskQueuePoolValue *tqpv = taskQueuePool_pop(tqp);
        DBG("[THREAD : %ld]: Get %d fd\n", pthread_self(), tqpv->clientFd);
        taskTreePoolValue *ttpv = taskTreePool_get(ttp, tqpv->clientFd);

        if (ttpv == NULL) {
            ttpv = taskTreePoolValue_init(tqpv->clientFd, NULL, -1, 0);
            taskTreePool_insert(ttp, ttpv); 
        }

        index = processMes_in(ttpv, sqlConnection);

        if (index == 0) {
            // 结束
            HttpHeader_free(ttpv->hh);
            ttpv->hh = NULL;
            if (ttpv->fileFd > 0) close(ttpv->fileFd);
            ttpv->fileFd = -1;
            ttpv->fileIndex = 0;
            DBG("[THREAD : %ld]: %d get close\n", pthread_self(), ttpv->clientFd);
            close(ttpv->clientFd);
        } else if (index == 1) {
            // 未读完文件
            struct epoll_event ev;
            ev.events = EPOLLOUT | EPOLLONESHOT | EPOLLET;
            ev.data.fd = ttpv->clientFd;
            epoll_ctl(epfd, EPOLL_CTL_MOD, ttpv->clientFd, &ev);
        } else if (index == 2) {
            // 未写完文件
            struct epoll_event ev;
            ev.events = EPOLLIN | EPOLLONESHOT | EPOLLET;
            ev.data.fd = ttpv->clientFd;
            epoll_ctl(epfd, EPOLL_CTL_MOD, ttpv->clientFd, &ev);
        }
        
        taskQueuePoolValue_free(tqpv);
    }

    return NULL;
}
