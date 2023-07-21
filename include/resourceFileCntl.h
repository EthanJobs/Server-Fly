#ifndef _RESOURCEFILECNTL_H
#define _RESOURCEFILECNTL_H

int writeFdFile(int filefd, int writefd, int *index, int max_size);
int readFdFile(int readfd, int filefd, int *index, int max_size);

#endif
