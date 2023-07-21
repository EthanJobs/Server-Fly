#include <coreDate.h>
#include <time.h>
#include <stdio.h>

void printGMT(int fd) {
    time_t timep = time(NULL);
    struct tm *p_tm = gmtime(&timep);
    if (fd == 1)
        printf("%d-%02d-%02d %02d:%02d:%02d", p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
    else 
        dprintf(fd, "%d-%d-%d %d:%d:%d", p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);

    return ;
}

void printLocalTime(int fd) {
    time_t timep = time(NULL);
    struct tm *p_tm = localtime(&timep);
    if (fd == 1)
        printf("%d-%02d-%02d %02d:%02d:%02d", p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);
    else 
        dprintf(fd, "%d-%d-%d %d:%d:%d", p_tm->tm_year + 1900, p_tm->tm_mon + 1, p_tm->tm_mday, p_tm->tm_hour, p_tm->tm_min, p_tm->tm_sec);

    return ;
}
