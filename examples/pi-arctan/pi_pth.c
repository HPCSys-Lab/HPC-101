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
#include <pthread.h>

void *compute_sum(void *);

// array to store the sum of each thread
double *t_sum;

// encapsulate the thread args
struct thread_data{
    int id; //thread id
    int begin; //begin iterator of the thread's chunck
    int end; //end iterator of the thread's chunck
};

// array to store the input data (args) of each thread
struct thread_data *thread_data_array;

int main(int argc, char *argv[]) {

    if(argc != 3){
        printf("Usage: ./pi_pth N T\n");
        printf("N: Number of iterations (terms of the series)\n");
        printf("T: Number of threads\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    // get the start time
    gettimeofday(&time_start, NULL);

    int num_terms = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    // allocate momory for the vector of results
    t_sum = (double *) malloc(num_threads * sizeof(double));

    // create an array of p_threads
    pthread_t threads[num_threads];

    // store the input data of each thread
    thread_data_array = (struct thread_data *) malloc(num_threads * sizeof(struct thread_data));

    // calculate the chunck size
    int chunck_size = num_terms / num_threads;

    // create the threads
    for(int i = 0; i < num_threads; i++){

        // set the thread args
        thread_data_array[i].id = i;
        thread_data_array[i].begin = i * chunck_size;
        thread_data_array[i].end = thread_data_array[i].begin + chunck_size;

        // the last thread might have more work
        // if num_terms % num_threads != 0
        if( i == num_threads-1 )
            thread_data_array[i].end = num_terms;

        // create the thread
        int error_code = pthread_create(&threads[i], NULL, compute_sum, (void *) &thread_data_array[i]);

        if (error_code){
            printf("ERROR: return code from pthread_create() is %d\n", error_code);
            exit(-1);
        };

    }

    double sum = 0.0;

    // wait each thread to finish and get the local sum
    for(int i = 0; i < num_threads; i++){
        pthread_join(threads[i], NULL);
        sum += t_sum[i];
    }

    free(thread_data_array);
    free(t_sum);

    double pi = sum * 4;

    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("pi with %d iterations is %.9lf in %lf seconds\n", num_terms, pi, exec_time);

    return 0;
}

void *compute_sum(void *args){

    struct thread_data *argument = (struct thread_data *) args;

    int my_id = argument->id;
    int begin = argument->begin;
    int end = argument->end;

    double sum = 0.0;
    double numerator, denominator;

    for(int i = begin; i < end; i++){
        numerator = pow(-1, i) * pow(1, 2 * i + 1);
        denominator = 2 * i + 1;
        sum += numerator/denominator;
    }

    t_sum[my_id] = sum;
}
