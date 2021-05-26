/*
    This program calculates pi.

    MPI implementation.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mpi.h>
#include <unistd.h>
#define MY_TAG 10
int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Usage: ./pi_mpi N\n");
        printf("N: Number of steps\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    // get the start time
    gettimeofday(&time_start, NULL);

    int myid, nprocs;
    MPI_Status status;
    MPI_Init(&argc,&argv);              /* starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);  /* get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);     /* get number of processes */

    int num_local_steps = 0;
    int num_steps = atoi(argv[1]);
    int remainder = num_steps % (nprocs - 1);       /* if num_steps is not divisible by nprocs */
    if (remainder > 0)
      num_local_steps = num_steps / (nprocs - 1);   /* number of steps per worker */
    else
      num_local_steps = num_steps / nprocs;

    double x, sum = 0.0;
    double step = 1.0 / (double) num_steps;
    int start, stop;

    if (myid == 0){     /* master process */
      start = 1;
      if (remainder >0) stop = start + remainder;
      else stop = start + num_local_steps;
    } else {     /*  worker processes */
      if (remainder > 0) start = (myid - 1) * num_local_steps + remainder + 1;
      else start = myid * num_local_steps + 1;
      stop = start + num_local_steps;
    }
    /* the work */
    for(int i = start; i < stop; i++){
        x = (i - 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }

    double buff_sum = 0.0;

    MPI_Reduce(&sum, &buff_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    double pi = step * buff_sum;
    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                         (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("pi with %d steps is %.9lf in %lf seconds\n", num_steps, pi, exec_time);

    MPI_Finalize();
    return 0;
}
