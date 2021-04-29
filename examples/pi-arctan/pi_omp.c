/*
    This program calculates pi. From trigonometry, we have:

    arctan(1) = pi / 4

    So: pi = arctan(1) * 4

    Using the Gregory-Leibniz/Madhava–Gregory series, we have a simple
    formula to calculate arctan(x):

    arctan(x) = sum( pow(-1, i) * pow(x, 2*i+1) / 2*i+1  ),

    where x = 1 and i = [0 ... infinite]
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Usage: ./pi_omp N\n");
        printf("N: Number of iterations (terms of the series)\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    // get the start time
    gettimeofday(&time_start, NULL);

    int num_terms = atoi(argv[1]);
    double sum = 0.0;
    double numerator, denominator;

    #pragma omp parallel for private(numerator, denominator) reduction(+: sum)
    for(int i = 0; i < num_terms; i++){
        numerator = pow(-1, i) * pow(1, 2 * i + 1);
        denominator = 2 * i + 1;
        sum += numerator/denominator;
    }

    double pi = sum * 4;

    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("pi with %d iterations is %.9lf in %lf seconds\n ", num_terms, pi, exec_time);

    return 0;
}
