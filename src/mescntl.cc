/*************************************************************************
	> File Name: mesdata.c
	> Author: 
	> Mail: 
	> Created Time: Wed 05 Oct 2022 02:31:09 PM CST
 ************************************************************************/

#include <head.h>
#include <mescntl.h>

void mesread(const char *path, char *date, int sendfd) {
    printf("输送数据\n");

    FILE *f = fopen(path, "r");
    if (!f) {
        printf("sendNull\n");
        write(sendfd, "null\n", 6);
        return ;
    }
    char *buf = (char *)malloc(sizeof(char) * MAX_SIZE);
    size_t *len;
    ssize_t n;
    int flag = 0;
    if (!date) flag = 1;

    while ((n = getline(&buf, len, f)) > 0) {
        if (date && strncmp(buf, date, 21) > 0) flag = 1;
        if (flag) write(sendfd, buf, n);
    }

    free(buf);

    //int fd = open(path, O_RDONLY);
    //if (fd < 0) {
    //    write(sendfd, "无记录\n", 4);
    //    close(fd);
    //    return ;
    //}

    //int n;
    //char buf[1024];

    //while (n = read(fd, buf, 1024))
    //    write(sendfd, buf, n);

    //close(fd);

    return ;
}

void meswrite(const char *path, char *mes, char *name) {
    int fd = open(path, O_WRONLY|O_APPEND|O_CREAT, 0666);
    if (fd < 0)
        prrexit("open");

    time_t t = time(NULL);
    char date[22];
    strftime(date, sizeof(date), "[%Y-%m-%d %H:%M:%S]", localtime(&t));

    dprintf(fd, "%s %s : %s", date, name, mes);

    close(fd);

    return ;
}

//void mesread(int sendfd, const char *path) {
//    int fd = open(path, O_RDONLY, 0666);
//    if (fd < 0) {
//        write(sendfd, "无记录\n", 5);
//        close(fd);
//        return ;
//    }
//
//    int n;
//    char buf[1024];
//
//    while (n = read(fd, buf, 1024))
//        write(sendfd, buf, n);
//
//    close(fd);
//
//    return ;
//}

//void meswrite(const char *path, char *mes, int meslen, char *name, int namelen) {
//    int fd = open(path, O_WRONLY|O_APPEND|O_CREAT, 0666);
//    if (fd < 0)
//        prrexit("open");
//
//    time_t t = time(NULL);
//    char date[22];
//    strftime(date, sizeof(date), "[%Y-%m-%d %H:%M:%S]", localtime(&t));
//    write(fd, date, sizeof(date));
//    write(fd, " ", 1);
//    write(fd, name, namelen);
//    write(fd, " : ", 3);
//    write(fd, mes, meslen);
//
//    close(fd);
//
//    return ;
//}
