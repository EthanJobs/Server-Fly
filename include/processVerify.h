#ifndef PROCESSVERIFY_H
#define PROCESSVERIFY_H

#include <coreTree.h>

typedef struct VerifyTree {
    Tree **n_Trees;
    int n_size;
} VerifyTree;

VerifyTree *VerifyTree_init(int size);
int VerifyTree_push(VerifyTree *vt, int level, const char *fileName);
int VerifyTree_isHave(VerifyTree *vt, int level, const char *fileName);
void VerifyTree_free(VerifyTree *vt);

#endif
