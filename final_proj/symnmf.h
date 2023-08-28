#ifndef SYM_NMF_H
#define SYM_NMF_H

#include "general.h"  // Include general.h to access shared structures and functions

// Function prototypes specific to symnmf.c
int read_points(vector *head_vec, int* d, char* file_name);
vector *c_sym(vector *X, int n);
vector *c_ddg(vector *A, int n);
vector *c_norm(vector *A, vector *D, int n);
vector *c_symnmf(vector* H, vector* W, int n, int k);

#endif // SYM_NMF_H