/*
    This program calculates pi.

    In order to do that, we compute the integral of:

    4 / (1 + x * x)

    from 0 to 1.

    The value of this integral is pi.

    Based on Tim Mattson's (11/99) sequential implementation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <omp.h>

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Usage: ./pi_omp N\n");
        printf("N: Number of steps\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    // get the start time
    gettimeofday(&time_start, NULL);

    int num_steps = atoi(argv[1]);
    double x, sum = 0.0;
    double step = 1.0 / (double) num_steps;

    #pragma omp parallel for private(x) reduction(+: sum)
    for(int i = 1; i <= num_steps; i++){
        x = (i - 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double pi = step * sum;

    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
    printf("pi with %d steps is %.9lf in %lf seconds in %d procs\n", num_steps, pi, exec_time,omp_get_max_threads());

    return 0;
}
