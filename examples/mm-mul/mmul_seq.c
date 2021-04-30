/*
    This program multiply two N x N dense, square matrices A and B
    to yield the product matrix C = A x B
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

// save the marix in a file
void save_matrix(int **matrix, int size){

    char file_name[30];
    sprintf(file_name, "result_matrix.txt");

    // save the result
    FILE *file;
    file = fopen(file_name, "w");

    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            fprintf(file, "%d ", matrix[i][j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Usage: ./mmul_seq N\n");
        printf("N: Matrix size on each axis\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    int size = atoi(argv[1]);

    // alloc memory for the matrices
    int **a = (int **) malloc(size * sizeof(int *));
    int **b = (int **) malloc(size * sizeof(int *));
    int **c = (int **) malloc(size * sizeof(int *));

    for(int i = 0; i < size; i++){
        a[i] = (int *) malloc(size * sizeof(int));
        b[i] = (int *) malloc(size * sizeof(int));
        c[i] = (int *) malloc(size * sizeof(int));
    }

    // initialize the matrices
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){
            a[i][j] = i + 1;
            b[i][j] = j + 1;
        }
    }

    // get the start time
    gettimeofday(&time_start, NULL);

    // multiply a x b
    for(int i = 0; i < size; i++){
        for(int j = 0; j < size; j++){

            c[i][j] = 0;

            for(int k = 0; k < size; k++){
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // get the end time
    gettimeofday(&time_end, NULL);

    // save the result on a file
    save_matrix(c, size);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("Matrices multiplied in %lf seconds\n", exec_time);

    return 0;
}
