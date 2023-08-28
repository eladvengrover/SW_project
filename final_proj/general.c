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

void free_cords_arr(cord **arr, int k) {
    int i;
    for (i = 0; i < k; i++) {
        free_cord(arr[i]);
    }
    free(arr);
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


vector* init_zero_matrix(int n, int d) {
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
        for (int j = 0; j < d; ++j) {  // Running on cols
            cord_curr->value = 0;
            if (j != d - 1) {  // Handle all cols except the last one
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


vector* get_col_matrix(vector *mat, int rows, int cols) {
    /** Get vector* mat s.t each vector represents a row,
     * and returns vector * out s.t each vector represents column.**/
    vector *out = init_zero_matrix(cols, rows), *curr_vec = mat;
    cord **curr = malloc(rows * sizeof (cord*)), *curr_col;
    int i = 0, j;
    while (curr_vec != NULL) {
        curr[i] = curr_vec->cords;
        curr_vec = curr_vec->next;
        i++;
    }
    curr_vec = out;
    for (i = 0; i < cols; ++i) {
        curr_col = curr_vec->cords;
        for (j = 0; j < rows; ++j) {
            curr_col->value = curr[j]->value;
            curr[j] = curr[j]->next;
            curr_col = curr_col->next;
        }
        curr_vec = curr_vec->next;
    }
    free_cords_arr(curr, rows);
    return out;
}

double dot_product(vector *X, vector *Y) {
    double output = 0;
    cord *X_curr = X->cords, *Y_curr = Y->cords;
    while (X_curr != NULL) {
        output += (X_curr->value * Y_curr->value);
        X_curr = X_curr->next;
        Y_curr = Y_curr->next;
    }
    return output;
}

vector *get_mat_multi_with_its_transpose(vector* mat, int rows, int cols) {
    vector *output = init_zero_matrix(rows, rows), *mat_curr_row = mat, *mat_transpose_row;
    vector *curr_output_row = output;
    cord *curr_output_col;
    int i, j;

    for (i = 0; i < rows; ++i) {
        curr_output_col = curr_output_row->cords;
        mat_transpose_row = mat;
        for (j = 0; j < rows; ++j) {
            curr_output_col->value = dot_product(mat_curr_row, mat_transpose_row);
            mat_transpose_row = mat_transpose_row->next;
            curr_output_col = curr_output_col->next;
        }
        mat_curr_row = mat_curr_row->next;
        curr_output_row = curr_output_row->next;
    }
    return output;
}

double get_the_ij_value_of_matrix_multi(vector *X, vector *Y_col, int i, int j) {
    /** Y_col is the matrix from right, that's why it has to be a column matrix.**/
    int k;
    for (k = 0; k < i; ++k) {
        X = X->next;
    }
    for (k = 0; k < j; ++k) {
        Y_col = Y_col->next;
    }
    return dot_product(X, Y_col);
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