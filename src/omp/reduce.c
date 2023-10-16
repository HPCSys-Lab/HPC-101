/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: 
** Objetivo: 
*/


#include <stdio.h>
#include <omp.h>

/*
The next piece of code is a common usage of the "reduction" clause to calculate reduced sums. Here, we add up all the elements of an array "a" with an "i" dependent weight using a for-loop which we parallelize using OpenMP directives and reduction clause. The scheduling is kept static.
*/


#define N 10


int main()
{
	int i;
	long w;
	int a[N]={1,1,1,1,1,1,1,1,1,1};
	long sum = 0;

	// forks off the threads and starts the work-sharing construct
	#pragma omp parallel for private(w) reduction(+:sum) schedule(static,1)
	for(i = 0; i < N; i++) {
		w = i*i;
		sum = sum + w*a[i];
	}
	printf("\nSoma: %li\n",sum);

/*
An equivalent, less elegant, implementation of the above code is to create a local sum variable for each thread ("loc_sum"), and make a protected update of the global variable "sum" at the end of the process, through the directive "critical". Note that this protection is crucial, as explained elsewhere.
*/
/*
 ...
	long sum = 0, loc_sum = 0;
	// forks off the threads and starts the work-sharing construct
	#pragma omp parallel for private(w,loc_sum) schedule(static,1) 
	{
		for(i = 0; i < N; i++) {
			w = i*i;
			loc_sum = loc_sum + w*a[i];
		}
	#pragma omp critical
	sum = sum + loc_sum;
	}

	printf("\n %li",sum);
*/

	return(0);
}
