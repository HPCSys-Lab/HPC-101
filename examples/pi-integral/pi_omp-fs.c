/*
    This program calculates pi using the integral method.

    The objetctive is to ilustrate the false sharing problem.
*/


#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

static long num_steps = 1000000000;
double step;
#define NUM_THREADS 8

int main(int argc, char *argv[])
{
  int i, nthreads; double pi, sum[NUM_THREADS];
  step = 1.0/(double) num_steps;

  struct timeval time_start;
  struct timeval time_end;

  gettimeofday(&time_start, NULL);

  omp_set_num_threads(NUM_THREADS);
  #pragma omp parallel
  {
     int i, id,nthrds;
     double x;
     id = omp_get_thread_num();
     nthrds = omp_get_num_threads();
     if (id == 0) nthreads = nthrds;
     for (i=id, sum[id]=0.0;i< num_steps; i=i+nthrds) {
       x = (i+0.5)*step;
       sum[id] += 4.0/(1.0+x*x);
     }
  }
  for(i=0, pi=0.0;i<nthreads;i++)pi += sum[i] * step;

  gettimeofday(&time_end, NULL);

  double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                     (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;
  printf("pi with %ld steps is %.9lf in %lf seconds in %d processors\n", num_steps, pi, exec_time,omp_get_max_threads());

  return 0;
}
