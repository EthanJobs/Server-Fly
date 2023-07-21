#include <resourceFileCntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int writeFdFile(int filefd, int writefd, int *index, int max_size) {
    int readLen, writeLen = 0, tmp;
    char buf[max_size];
    lseek(filefd, *index, SEEK_SET);

    while (1) {
        *index += writeLen;
        readLen = read(filefd, buf, max_size);
        if (readLen <= 0)
            return readLen;
        writeLen = write(writefd, buf, readLen);
        tmp = writeLen;
        while (1) {
            readLen -= tmp;
            if (readLen == 0)
                break;
            tmp = write(writefd, buf + writeLen, readLen);
            if (tmp == -1) {
                *index += writeLen;
                return -1;
            }
            writeLen += tmp;
        }
    }
}

int readFdFile(int readfd, int filefd, int *index, int max_size) {
    int readLen, writeLen = 0;
    char buf[max_size];

    while (1) {
        *index -= writeLen;
        if (*index == 0)
            return 0;
        readLen = read(readfd, buf, max_size);
        if (readLen <= 0)
            return readLen;
        writeLen = write(filefd, buf, readLen);
    }
}
