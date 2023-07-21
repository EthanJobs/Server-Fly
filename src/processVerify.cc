#include <stdlib.h>
#include <string.h>
#include <coreTree.h>
#include <processVerify.h>

int ConstCharCompare(void *a, void *b) {
    return strcmp((const char *)a, (const char *)b);
}

VerifyTree *VerifyTree_init(int size) {
    VerifyTree *vt = (VerifyTree *)malloc(sizeof(VerifyTree));
    vt->n_Trees = (Tree **)malloc(sizeof(Tree *) * size);
    vt->n_size = size;
    for (int i = 0; i < size; i++)
        vt->n_Trees[i] = Tree_init(ConstCharCompare, NULL);

    return vt;
}

int VerifyTree_push(VerifyTree *vt, int level, const char *fileName) {
    if (vt == NULL || level < 0 || level >= vt->n_size || fileName == NULL) return -1;

    return Tree_insertValue(vt->n_Trees[level], (void *)fileName);
}

int VerifyTree_isHave(VerifyTree *vt, int level, const char *fileName) {
    if (vt == NULL || level < 0 || level >= vt->n_size || fileName == NULL) return -1;

    return Tree_isHave(vt->n_Trees[level], (void *)fileName, ConstCharCompare);
}

void VerifyTree_free(VerifyTree *vt) {
    if (vt == NULL) return ;

    for (int i = 0; i < vt->n_size; i++)
        Tree_free(vt->n_Trees[i]);
    free(vt);

    return ;
}
