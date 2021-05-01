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
#include <pthread.h>

void *compute_sum(void *);

int num_steps, num_threads;
double step;

// array to store the sum of each thread
double *t_sum;

int main(int argc, char *argv[]) {

    if(argc != 3){
        printf("Usage: ./pi_pth N T\n");
        printf("N: Number of steps\n");
        printf("T: Number of threads\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    // get the start time
    gettimeofday(&time_start, NULL);

    num_steps = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    step = 1.0 / (double) num_steps;

    // create an array of p_threads
    pthread_t threads[num_threads];

    // store each thread ID
    int t_id[num_threads];

    // allocate momory for the vector of results
    t_sum = (double *) malloc(num_threads * sizeof(double));

    // create the threads
    for(int i = 0; i < num_threads; i++){
        t_id[i] = i;
        pthread_create(&threads[i], NULL, compute_sum, (void *) &t_id[i]);
    }

    double sum = 0.0;

    // wait each thread to finish and get the local sum
    for(int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
        sum += t_sum[i];
    }

    double pi = step * sum;

    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("pi with %d steps is %.9lf in %lf seconds\n", num_steps, pi, exec_time);

    free(t_sum);

    return 0;
}

void *compute_sum(void *args){

    // thread id
    int id = *(int *) args;

    // calculate the chunk size
    int chunk = num_steps / num_threads;

    // calcute begin and end step of the thread
    int begin = id * chunk;
    int end = begin + chunk;

    // the last thread might have more work
    // if num_steps % num_threads != 0
    if( id == num_threads-1 )
        end = num_steps;

    double x, sum = 0.0;

    for(int i = begin + 1; i <= end; i++){
        x = (i - 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    t_sum[id] = sum;
}
