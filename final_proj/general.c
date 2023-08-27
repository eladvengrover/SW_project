# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include "general.h"


void handle_errors() {
    printf("An Error Has Occurred");
    exit(1);
}

void free_cord(cord *cord1) {
    cord* curr_cord = cord1;
    cord* next_cord;

    while(curr_cord != NULL) {
        next_cord = curr_cord->next;
        free(curr_cord);
        curr_cord = next_cord;
    }
}

void free_vec(vector *vec) {
    vector* curr_vec = vec;
    vector* next_vec;

    while(curr_vec != NULL) {
        next_vec = curr_vec->next;
        free_cord(curr_vec->cords);
        free(curr_vec);
        curr_vec = next_vec;
    }
}

double euclidean_dist(cord* first, cord* second) {
    double dist = 0;
    cord* curr_first = first;
    cord* curr_second = second;
    while(curr_first != NULL && curr_second != NULL) {
        dist += pow((curr_first->value - curr_second->value), 2);
        curr_first = curr_first->next;
        curr_second = curr_second->next;
    }
    return dist;
}


vector* init_zero_matrix(int n) {
    vector *zero_mat = malloc(sizeof (vector)), *curr = zero_mat;
    cord* cord_curr;
    vector *curr_i = curr;  // running pointers on X rows/cols
    if (zero_mat == NULL) {
        handle_errors();
    }

    for (int i = 0; i < n; ++i) {  // Running on rows
        curr->cords = malloc(sizeof (cord));
        if (curr->cords == NULL) {
            handle_errors();
        }
        cord_curr = curr->cords;
        for (int j = 0; j < n; ++j) {  // Running on cols
            cord_curr->value = 0;
            if (j != n - 1) {  // Handle all cols except the last one
                cord_curr->next = malloc(sizeof (cord));
                cord_curr = cord_curr->next;
            }
        }
        cord_curr->next = NULL;
        if (i != n - 1) {  // Handle all lines except the last one
            curr->next = malloc(sizeof (vector));
            curr = curr->next;
        }
        curr_i = curr_i->next;
    }
    curr->next = NULL;

    return zero_mat;
}

double get_vector_sum(vector* vec) {
    cord* curr_cord = vec->cords;
    double sum = 0;

    while (curr_cord != NULL) {
        sum += curr_cord->value;
        curr_cord = curr_cord->next;
    }
    return sum;
}

double* get_matrix_diag_values(vector* mat, int n) {
    double* diag_values = malloc(n * sizeof (double ));
    vector* curr_vec = mat;
    cord* curr_cord;

    for (int i = 0; i < n; ++i) {  // Running on rows
        curr_cord = curr_vec->cords;
        for (int j = 0; j < n; ++j) {  // Running on cols
            if (i == j) {
                diag_values[i] = curr_cord->value;
                break;
            }
            curr_cord = curr_cord->next;
        }
        curr_vec = curr_vec->next;
    }
    return diag_values;
}

double* pow_mat_diag_values(double* mat_diag, int n, double x) {
    for (int i = 0; i < n; ++i) {
        mat_diag[i] = pow(mat_diag[i], x);
    }
    return mat_diag;
}


void print_vec_arr(vector* vec_arr) {
    vector* curr_vec = vec_arr;
    cord* curr_cord;

    while (curr_vec != NULL) {
        curr_cord = curr_vec->cords;
        while (curr_cord != NULL) {
            printf("%.4f", curr_cord->value);
            if (curr_cord->next != NULL) {
                printf(",");
            }
            curr_cord = curr_cord->next;
        }
        printf("\n");
        curr_vec = curr_vec->next;
    }
}