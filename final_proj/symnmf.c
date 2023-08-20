# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
# include "general.h"


void handle_errors() {
    printf("An Error Has Occurred");
    exit(1);
}

int read_points(vector *head_vec, int* d, char* file_name) {
    vector *last_vec, *curr_vec;
    cord *head_cord, *curr_cord;
    double dp_curr_cord;
    char c;
    int n = 0, d_flag = 1;
    FILE *f = fopen("C:\\Users\\Elad\\CLionProjects\\SW_project\\final_proj\\input.txt", "r");



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


vector* sym(vector* X, int n) {
    vector *A = malloc(sizeof (vector)), *A_curr = A;
    cord* cord_curr;
    vector *X_i = X, *X_j;  // running pointers on X rows/cols
    if (A == NULL) {
        handle_errors();
    }

    for (int i = 0; i < n; ++i) {  // Running on rows
        A_curr->cords = malloc(sizeof (cord));
        if (A_curr->cords == NULL) {
            handle_errors();
        }
        cord_curr = A_curr->cords;
        X_j = X;
        for (int j = 0; j < n; ++j) {  // Running on cols
            if (i == j) {
                cord_curr->value = 0;
            } else {
                cord_curr->value = exp(-0.5 * euclidean_dist(X_i->cords, X_j->cords));
            }
            if (j != n - 1) {  // Handle all cols except the last one
                cord_curr->next = malloc(sizeof (cord));
                cord_curr = cord_curr->next;
            }
            X_j = X_j->next;
        }
        cord_curr->next = NULL;
        if (i != n - 1) {  // Handle all lines except the last one
            A_curr->next = malloc(sizeof (vector));
            A_curr = A_curr->next;
        }
        X_i = X_i->next;
    }
    A_curr->next = NULL;

    return A;
}



int main(int argc, char **argv) {
    vector *head_vec, *A = NULL;
    cord *curr_cord;
    int n,  i, d_value = 0;

    /** Read args*/
    char* goal = argv[1];
    char* file_name = argv[2];

    /**Allocating memory for head of vectors LL*/
    head_vec = malloc(sizeof(struct vector));
    if (head_vec == NULL) {
        handle_errors();
    }
    head_vec->next = NULL;
    head_vec->cords = malloc(sizeof(cord));
    if (head_vec->cords == NULL) {
        handle_errors();
    }
    head_vec->cords->value = 0;
    head_vec->cords->next = NULL;


    n = read_points(head_vec, &d_value, file_name);

    if (strcmp(goal, "sym") == 0) {
        A = sym(head_vec, n);
    }

    print_vec_arr(A);

    /**Free reest of memory*/
    free_vec(head_vec);

    return 0;
}