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