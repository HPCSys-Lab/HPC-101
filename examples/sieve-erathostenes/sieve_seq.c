#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

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

    long long n = atol(argv[1]);

    printf(" Valor de N =%lld \n",n);
    unsigned long long int i,j;
    int *primes;  
    int z = 1;

    primes = malloc(sizeof(int) * n);
    
    for (i = 2;i < n; i++)  // In the begin, all numbers are prime
        primes[i] = 1;
    long long squareRoot = sqrt(n);
//    for (i = 2;i < squareRoot; i++) // Multiples of a prime are not prime
    for (i = 2;i < n; i++) // Multiples of a prime are not prime
        if (primes[i])
            for (j = i;i * j < n; j++)
                primes[i * j] = 0;

    printf("\nPrime numbers in range 1 to %lld are: \n",n);
    for (i = 2;i < n; i++)
        if (primes[i])
            printf("%lld\n", i);


    // get the end time
    gettimeofday(&time_end, NULL);

    double exec_time = (double) (time_end.tv_sec - time_start.tv_sec) +
                       (double) (time_end.tv_usec - time_start.tv_usec) / 1000000.0;

    printf("Executed  in %lf seconds\n", exec_time);

    return 0;
}
