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



#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

/*
The application of some OpenMP clauses are illustrated in the simple examples in this section. 
The piece of code below updates the elements of an array "b" by performing a simple operation 
on the elements of an array "a". The parallelization is done by the OpenMP directive "#pragma". 
The scheduling of tasks is dynamic. Notice how the iteration counters "j" and "k" have to be 
made private, whereas the primary iteration counter "i" is private by default. 
The task of running through "i" is divided among multiple threads, and each thread creates 
its own versions of "j" and "k" in its execution stack, thus doing the full task allocated 
to it and updating the allocated part of the array "b" at the same time as the other threads.
*/

#define CHUNKSIZE 1 /*defines the chunk size as 1 contiguous iteration*/

#define N 4

int
main()
{
	int i,j;
	int id, nt;
	int a[N][N], b[N][N];

	// forks off the threads
	#pragma omp parallel private(j,id) 
	{
	// nt = omp_get_num_threads();
	id = omp_get_thread_num();

	// Starts the work sharing construct
	#pragma omp for schedule(dynamic, CHUNKSIZE)
		for(i=0;i<N;i++)
			for(j=0;j<N;j++) {
				printf("Thread %d calculando %d %d\n",id,i,j);
				b[i][j] = a[i][j]*rand();
			}
	}
	return(0);
}

