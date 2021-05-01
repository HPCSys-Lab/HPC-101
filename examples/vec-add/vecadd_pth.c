/*
    Simple program to add two vectors and verify the results.

    Based on Tim Mattson's (November 2017) implementation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#define TOL 0.0000001

void *add_vector(void *);

float *a;
float *b;
float *c;
float *res;

// encapsulate the thread args
struct thread_data{
    int begin; //begin iterator of the thread's chunk
    int end; //end iterator of the thread's chunk
};

// array to store the args of each thread
struct thread_data *thread_data_array;

int main(int argc, char *argv[]) {

    if(argc != 3){
        printf("Usage: ./vecadd_pth N T\n");
        printf("N: Size of the vectors\n");
        printf("T: Number of threads\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    // get the start time
    gettimeofday(&time_start, NULL);

    int size = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    int err = 0;

    a = (float*) malloc(size * sizeof(float));
    b = (float*) malloc(size * sizeof(float));
    c = (float*) malloc(size * sizeof(float));
    res = (float*) malloc(size * sizeof(float));

    // fill the arrays
    // this loop will not be parallelized in this example
    // but it should be as well
    for (int i = 0; i < size; i++){
        a[i] = (float) i;
        b[i] = 2.0 * (float) i;
        c[i] = 0.0;
        res[i] = i + 2 * i;
    }

    // create an array of p_threads
    pthread_t threads[num_threads];

    // store the input data of each thread
    thread_data_array = (struct thread_data *) malloc(num_threads * sizeof(struct thread_data));

    // calculate the chunk size
    int chunk = size / num_threads;

    // create the threads
    for(int i = 0; i < num_threads; i++){
        // set the thread args
        thread_data_array[i].begin = i * chunk;
        thread_data_array[i].end = thread_data_array[i].begin + chunk;

        // the last thread might have more work
        // if size % num_threads != 0
        if( i == num_threads-1 )
            thread_data_array[i].end = size;

        // create the thread
        pthread_create(&threads[i], NULL, add_vector, (void *) &thread_data_array[i]);
    }

    // wait each thread to finish
    for(int i = 0; i < num_threads; i++)
        pthread_join(threads[i], NULL);

    // test results
    // this loop will not be parallelized in this example
    // but it should be as well
    for (int i = 0; i < size; i++){
        float val = c[i] - res[i];
        val = val * val;

        if(val > TOL)
            err++;
    }

    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("vectors added with %d errors in %lf seconds\n", err, exec_time);

    free(thread_data_array);
    free(a);
    free(b);
    free(c);
    free(res);

    return 0;
}

void *add_vector(void *args){

    struct thread_data *argument = (struct thread_data *) args;

    int begin = argument->begin;
    int end = argument->end;

    // add two vectors
    for (int i = begin; i < end; i++){
        c[i] = a[i] + b[i];
    }

}
