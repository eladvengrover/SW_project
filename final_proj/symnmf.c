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
    FILE *f = fopen("C:\\Users\\Elad\\CLionProjects\\SW_project\\final_proj\\input.txt", "r");
    // TODO - remove hard-coded path and use file_name instead


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

    for (int i = 0; i < n; ++i) {
        X_j = X;
        curr_A_cord = curr_A_row->cords;
        for (int j = 0; j < n; ++j) {
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
    double vec_sums[n];
    for (int i = 0; i < n; ++i) {
        vec_sums[i] = get_vector_sum(curr_vec);
        curr_vec = curr_vec->next;
    }

    for (int i = 0; i < n; ++i) {  // Running on rows
        D_j = D_i->cords;
        for (int j = 0; j < n; ++j) {  // Running on cols
            if (i == j) {
                D_j->value = vec_sums[i];
                break;
            }
            D_j = D_j->next;
        }
        D_i = D_i->next;
    }
    return D;
}

vector* c_norm(vector* A, vector* D, int n) {
    vector *W = init_zero_matrix(n, n);
    double* D_diag = get_matrix_diag_values(D, n);
    D_diag = pow_mat_diag_values(D_diag, n, -0.5);
    vector* curr_A_row = A, *curr_W_row = W;
    cord* curr_A_cord, *curr_W_cord;

    for (int i = 0; i < n; ++i) {
        curr_A_cord = curr_A_row->cords;
        curr_W_cord = curr_W_row->cords;
        for (int j = 0; j < n; ++j) {
            if (i != j) {
                curr_W_cord->value = D_diag[i] * D_diag[j] * curr_A_cord->value;
            }
            curr_A_cord = curr_A_cord->next;
            curr_W_cord = curr_W_cord->next;
        }
        curr_A_row = curr_A_row->next;
        curr_W_row = curr_W_row->next;
    }
    return W;
}

double get_delta_h_norm(vector* h_new, vector *h_old) {
    return 1;
}


vector* c_symnmf(vector* H, vector* W, int n, int k) {
    double beta = 0.5, epsilon = 0.0001;
    int max_iter = 300, iter = 0;
    vector *H_new = init_zero_matrix(n, k);

    while (iter < max_iter && get_delta_h_norm(H_new, H) >= epsilon) {

        iter++;
    }
}


int main(int argc, char **argv) {
    vector *X, *A = NULL, *D = NULL, *W = NULL;
    int n, d_value = 0;

    /** Read args*/
    char* goal = argv[1];
    char* file_name = argv[2];

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
    //TODO - consider removing d
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
    } else if (strcmp(goal, "symm") == 0) {
        A = c_sym(X, n);
        print_vec_arr(A);
        printf("------------\n");
        print_vec_arr(get_col_matrix(A, n, n));
    }

    /**Free reest of memory*/
    free_vec(X);
    free_vec(A);
    free_vec(D);
    free_vec(W);

    return 0;
}