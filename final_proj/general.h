#ifndef GENERAL_H
#define GENERAL_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/*Structure definitions */
typedef struct cord {
    double value;
    struct cord *next;
} cord;

typedef struct vector {
    struct vector *next;
    struct cord *cords;
} vector;

/* Function prototypes */
void handle_errors();
void free_cord(cord *cord1);
void free_vec(vector *vec);
vector *get_col_matrix(vector *mat, int rows, int cols);
double get_the_ij_value_of_matrix_multi(vector *X, vector *Y_col, int i, int j);
double dot_product(vector *X, vector *Y);
vector *get_mat_multi_with_its_transpose(vector* mat, int rows);
double euclidean_dist(cord *first, cord *second);
vector *init_zero_matrix(int n, int d);
double get_vector_sum(vector *vec);
double *get_matrix_diag_values(vector *mat, int n);
double *pow_mat_diag_values(double *mat_diag, int n, double x);
void print_vec_arr(vector *vec_arr);

#endif /* GENERAL_H */