//
// Created by Elad on 26/08/2023.
//

#ifndef SW_PROJECT_GENERAL_H
#define SW_PROJECT_GENERAL_H

#endif //SW_PROJECT_GENERAL_H

# include <stdio.h>
# include <stdlib.h>
# include <math.h>


typedef struct cord
{
    double value;
    struct cord *next;
} cord;

typedef struct vector
{
    struct vector *next;
    struct cord *cords;
} vector;


void handle_errors();

void free_cord(cord *cord1) ;

void free_vec(vector *vec);
double euclidean_dist(cord* first, cord* second);

vector* init_zero_matrix(int n);
double get_vector_sum(vector* vec);

double* get_matrix_diag_values(vector* mat, int n);

double* pow_mat_diag_values(double* mat_diag, int n, double x);

void print_vec_arr(vector* vec_arr);
