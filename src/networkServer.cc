#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <coreLog.h>
#include <resourceSQL.h>
#include <networkPthreadPool.h>
#include <networkServer.h>

#include <defineHead.h>

void server_start(void *(*processMes)(void *)) {
    LOG("[SERVER]: LOG TEST\n");
    LOGERROR("[SERVER]: LOGERROR TEST\n");
    DBG("[SERVER]: DBG TEST\n");
    
    signal(SIGPIPE, SIG_IGN);

    sigset_t signal_mask;
    sigemptyset(&signal_mask);
    sigaddset(&signal_mask, SIGPIPE);
    int rc = pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);
    if (rc != 0) {
        LOGERROR("[SERVER]: Block sigpipe error\n");
        return ;
    }

    int server_sockfd; //服务端套接字描述符
    int client_sockfd; //客户端套接字描述符
    char str[INET_ADDRSTRLEN];
    struct sockaddr_in server_addr; //服务端套接字信息
    struct sockaddr_in client_addr; //客户端套接字信息

    taskQueuePool *tqp = taskQueuePool_init(); //线程池
    taskTreePool *ttp = taskTreePool_init(); //线程判断池

    /*获取服务端套接字描述符*/
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sockfd == -1) {
        LOGERROR("[SERVER]: Socket error\n");
        exit(1);
    }

    // epoll
    int epfd = epoll_create(EPOLL_SIZE);
    struct epoll_event ev, events[EPOLL_SIZE];
    ev.events = EPOLLIN;// | EPOLLET;
    ev.data.fd = server_sockfd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_sockfd, &ev);

    /*初始化服务端套接字信息*/
    memset(&server_addr, 0, sizeof(struct sockaddr_in));
    server_addr.sin_family = AF_INET; //使用IPv4
    server_addr.sin_port = htons(PORT); //对端口进行字节转化
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); //对IP地址进行格式化转化

    /*因为最后需要服务器主动关闭连接，所以要设置服务器套接字为可重用*/
    int option = 1;
    setsockopt(server_sockfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    /*绑定服务器套接字信息*/
    if (bind(server_sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr_in)) < 0) {
        LOGERROR("[SERVER]: Bind error\n");
        exit(1);
    }

    /*将服务器套接字转化为被动监听*/
    if (listen(server_sockfd, 3) < 0) {
        LOGERROR("[SERVER]: Listen error\n");
        exit(1);
    }

    // 多线程
    pthread_t tid;
    pthreadARG parg;
    parg.tqp = tqp;
    parg.ttp = ttp;
    parg.epfd = epfd;
    for (int i = 0; i < PTHREAD_MAX; i++) {
        pthread_create((&tid), NULL, processMes, (void *)&parg);
        pthread_detach(tid);
        LOG("[SERVER]: New thread is %#lx\n", tid);
    }

    /*与客户端进行串行连接*/
    /*等待客户端连接*/
    LOG("[SERVER]: Server is waiting\n");
    while(1) {
        int nfds = epoll_wait(epfd, events, EPOLL_SIZE, -1);
        if (nfds == -1) {
            if (errno == EINTR) continue;
            LOGERROR("[SERVER]: Epoll_wait error\n");
            exit(1);
        }

        for (int i = 0; i < nfds; i++) {
            if (events[i].data.fd == server_sockfd) {
                socklen_t len = sizeof(struct sockaddr_in);
                client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &len);
                if (client_sockfd < 0) {
                    LOGERROR("[SERVER]: Accept error\n");
                    exit(1);
                }

                int flag = fcntl(client_sockfd, F_GETFL);
                flag |= O_NONBLOCK;
                fcntl(client_sockfd, F_SETFL, flag);

                ev.events = EPOLLIN | EPOLLONESHOT | EPOLLET;
                ev.data.fd = client_sockfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_sockfd, &ev);
            } else {
                int clifd = events[i].data.fd;
                if (clifd < 3) continue;
                taskQueuePoolValue *tqpv = taskQueuePoolValue_init(clifd);
                taskQueuePool_push(tqp, tqpv);
            }
        }
    }

    taskQueuePool_free(tqp);
    taskTreePool_free(ttp);
}
