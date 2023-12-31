# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "general.c"


int read_points(vector *head_vec, int* d, char* file_name) {
    vector *last_vec = NULL, *curr_vec;
    cord *head_cord, *curr_cord;
    double dp_curr_cord;
    char c;
    int n = 0, d_flag = 1;
    FILE *f = fopen(file_name, "r");

    head_cord = head_vec->cords;
    curr_cord = head_cord;

    curr_vec = head_vec;

    while ((fscanf(f,"%lf%c", &dp_curr_cord, &c)) != EOF)
    {
        if (c == '\n') /**last cord of vector*/
        {
            curr_cord->value = dp_curr_cord;  /**Read last cord of vector*/
            curr_cord->next = NULL;
            curr_vec->cords = head_cord;

            last_vec = curr_vec;  /**Prepare the next vector*/
            head_cord = malloc(sizeof(struct cord));
            if (head_cord == NULL) {
                handle_errors();
            }
            head_cord->value = 0;
            head_cord->next = NULL;
            curr_cord = head_cord;

            curr_vec->next = malloc(sizeof(vector));
            if (curr_vec->next == NULL) {
                handle_errors();
            }
            curr_vec->next->cords = head_cord;
            curr_vec->next->next = NULL;

            curr_vec = curr_vec->next;
            if(d_flag == 1){  /**To compute d*/
                (*d)++;
            }
            d_flag = 0;
            n++;
            continue;
        }

        curr_cord->value = dp_curr_cord;
        curr_cord->next = malloc(sizeof(cord));
        if (curr_cord->next == NULL) {
            handle_errors();
        }
        curr_cord->next->value = 0;
        curr_cord->next->next = NULL;

        curr_cord = curr_cord->next;
        if(d_flag == 1){
            (*d)++;
        }
    }
    fclose(f);

    free_vec(last_vec->next);
    last_vec->next = NULL;

    return n;
}


vector* c_sym(vector* X, int n) {
    vector *A = init_zero_matrix(n, n), *curr_A_row = A;
    vector *X_i = X, *X_j;
    cord *curr_A_cord;
    int i, j;

    for (i = 0; i < n; ++i) {
        X_j = X;
        curr_A_cord = curr_A_row->cords;
        for (j = 0; j < n; ++j) {
            if (i != j) {
                curr_A_cord->value = exp(-0.5 * euclidean_dist(X_i->cords, X_j->cords));
            }
            X_j = X_j->next;
            curr_A_cord = curr_A_cord->next;
        }
        X_i = X_i->next;
        curr_A_row = curr_A_row->next;
    }
    return A;
}


vector* c_ddg(vector* A, int n) {
    vector *D = init_zero_matrix(n, n), *D_i = D;
    cord* D_j;
    vector *curr_vec = A;
    int i, j;
    double *vec_sums = malloc (n * sizeof (double));
    if (vec_sums == NULL) {
        handle_errors();
    }

    for (i = 0; i < n; ++i) {
        vec_sums[i] = get_vector_sum(curr_vec);
        curr_vec = curr_vec->next;
    }

    for (i = 0; i < n; ++i) {  /* Running on rows */
        D_j = D_i->cords;
        for (j = 0; j < n; ++j) {  /* Running on cols */
            if (i == j) {
                D_j->value = vec_sums[i];
                break;
            }
            D_j = D_j->next;
        }
        D_i = D_i->next;
    }
    free(vec_sums);
    return D;
}

vector* c_norm(vector* A, vector* D, int n) {
    vector *W = init_zero_matrix(n, n);
    int i, j;
    vector *curr_A_row;
    vector *curr_W_row;
    cord *curr_A_cord, *curr_W_cord;
    double* D_diag;
    D_diag = get_matrix_diag_values(D, n);
    D_diag = pow_mat_diag_values(D_diag, n, -0.5);
    curr_A_row = A;
    curr_W_row = W;

    for (i = 0; i < n; ++i) {
        curr_A_cord = curr_A_row->cords;
        curr_W_cord = curr_W_row->cords;
        for (j = 0; j < n; ++j) {
            if (i != j) {
                curr_W_cord->value = D_diag[i] * D_diag[j] * curr_A_cord->value;
            }
            curr_A_cord = curr_A_cord->next;
            curr_W_cord = curr_W_cord->next;
        }
        curr_A_row = curr_A_row->next;
        curr_W_row = curr_W_row->next;
    }
    free(D_diag);
    return W;
}

double get_delta_h_norm(vector* h_new, vector *h_old) {
    double output;
    vector *h_old_row = h_old, *h_new_row = h_new;
    cord *h_old_col, *h_new_col;
    output = 0;

    if (h_new == NULL) {
        return 1;
    }

    while (h_new_row != NULL && h_old_row != NULL) {
        h_new_col = h_new_row->cords;
        h_old_col = h_old_row->cords;
        while (h_old_col != NULL && h_new_col != NULL) {
            output += (pow((h_new_col->value - h_old_col->value), 2));
            h_new_col = h_new_col->next;
            h_old_col = h_old_col->next;
        }
        h_old_row = h_old_row->next;
        h_new_row = h_new_row->next;
    }
    return output;
}


vector* c_symnmf(vector* H, vector* W, int n, int k) {
    double beta = 0.5, epsilon = 0.0001, new_value;
    int max_iter = 300, iter = 0, i, j;
    vector *H_new = NULL, *H_col = NULL, *H_multi_H_transpose = NULL;
    vector *curr_H_new_row = NULL, *curr_H_row = NULL;
    cord *curr_H_new_cord, *curr_H_cord;

    while (iter < max_iter) {
        H_col = get_col_matrix(H, n, k);
        H_multi_H_transpose = get_mat_multi_with_its_transpose(H, n);
        H_new = init_zero_matrix(n, k);
        curr_H_new_row = H_new;
        curr_H_row = H;

        for (i = 0; i < n; ++i) {
            curr_H_new_cord = curr_H_new_row->cords;
            curr_H_cord = curr_H_row->cords;
            for (j = 0; j < k; ++j) {
                new_value =  curr_H_cord->value * ((1-beta) +
                                                   beta * (get_the_ij_value_of_matrix_multi(W, H_col, i, j) /
                                                           get_the_ij_value_of_matrix_multi(H_multi_H_transpose, H_col, i, j)));
                curr_H_new_cord->value = new_value;
                curr_H_new_cord = curr_H_new_cord->next;
                curr_H_cord = curr_H_cord->next;
            }
            curr_H_new_row = curr_H_new_row->next;
            curr_H_row = curr_H_row->next;
        }
        if (get_delta_h_norm(H_new, H) < epsilon) {
            free_vec(H_col);
            free_vec(H_multi_H_transpose);
            free_vec(H);
            break;
        }
        free_vec(H_col);
        free_vec(H_multi_H_transpose);
        free_vec(H);
        H = H_new;
        iter++;
    }
    return H_new;
}


int main(int argc, char **argv) {
    vector *X, *A = NULL, *D = NULL, *W = NULL, *H = NULL, *optimized_H = NULL;
    int n, d_value = 0;
    char *goal, *file_name;
    if (argc != 3) {
        exit(1);
    }

    /** Read args*/
    goal = argv[1];
    file_name = argv[2];

    /**Allocating memory for head of vectors LL*/
    X = malloc(sizeof(struct vector));
    if (X == NULL) {
        handle_errors();
    }
    X->next = NULL;
    X->cords = malloc(sizeof(cord));
    if (X->cords == NULL) {
        handle_errors();
    }
    X->cords->value = 0;
    X->cords->next = NULL;
    n = read_points(X, &d_value, file_name);

    if (strcmp(goal, "sym") == 0) {
        A = c_sym(X, n);
        print_vec_arr(A);
    } else if (strcmp(goal, "ddg") == 0) {
        A = c_sym(X, n);
        D = c_ddg(A, n);
        print_vec_arr(D);
    } else if (strcmp(goal, "norm") == 0) {
        A = c_sym(X, n);
        D = c_ddg(A, n);
        W = c_norm(A, D, n);
        print_vec_arr(W);
    }

    /**Free reest of memory*/
    free_vec(X);
    free_vec(A);
    free_vec(D);
    free_vec(W);
    free_vec(H);
    free_vec(optimized_H);



    return 0;
}