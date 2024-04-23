#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {

    if(argc != 2){
        printf("Usage: ./%s N\n",argv[0]);
        printf("N: Compute prime numbers from 1..N\n");
        exit(-1);
    }

    // variables to measure execution time
    struct timeval time_start;
    struct timeval time_end;

    // get the start time
    gettimeofday(&time_start, NULL);

    long long n = atoll(argv[1]);
    exit(0);
    double x, sum = 0.0;
    double step = 1.0 / (double) num_steps;
printf("sizeof(int)=%d\n",sizeof(int));
    for(int i = 1; i <= num_steps; i++){
        x = (i - 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
//        printf("  i=%d   x=%6.2f sum=%6.2f  \n",i,x,sum);
    }

    double pi = step * sum;

    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("pi with %d steps is %.9lf in %lf seconds\n", num_steps, pi, exec_time);

    return 0;
}
