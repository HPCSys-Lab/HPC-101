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


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define CHUNKSIZE 100
#define NUM 20

int 
main(int argc, char **argv) 
{
	int i, vet[NUM];
	int chunk;
 
/*
	#pragma omp for [clause ...]  newline 
		schedule (type [,chunk]) 
		ordered
		private (list) 
		firstprivate (list) 
		lastprivate (list) 
		shared (list) 
		reduction (operator: list) 
		collapse (n) 
		nowait 

   for_loop
*/
/*
schedule:

-> STATIC: Loop iterations are divided into pieces of size chunk and then 
statically assigned to threads. If chunk is not specified, the iterations 
are evenly (if possible) divided contiguously among the threads.

-> DYNAMIC: Loop iterations are divided into pieces of size chunk, and 
dynamically scheduled among the threads; when a thread finishes one chunk, 
it is dynamically assigned another. The default chunk size is 1.

-> GUIDED: For a chunk size of 1, the size of each chunk is proportional 
to the number of unassigned iterations divided by the number of threads, 
decreasing to 1. For a chunk size with value k (greater than 1), the size 
of each chunk is determined in the same way with the restriction that the 
chunks do not contain fewer than k iterations (except for the last chunk 
to be assigned, which may have fewer than k iterations). 
The default chunk size is 1.

-> RUNTIME: The scheduling decision is deferred until runtime by the 
environment variable OMP_SCHEDULE. It is illegal to specify a chunk size 
for this clause.

-> AUTO: The scheduling decision is delegated to the compiler and/or runtime system. 
*/
 
	chunk = CHUNKSIZE;

	// Observar a atribuição das iterações em função da política de 
	// escalonamento selecionada

#pragma omp parallel 
{
	// #pragma omp for schedule (static)
	// #pragma omp for schedule(dynamic,chunk) nowait
	#pragma omp for schedule (dynamic)
	// #pragma omp for 

	for (i=0; i < NUM; i++) {
		vet[i] = 2 * i;
		sleep(rand()%5);
		printf("th %d tratando iteração %d\n",omp_get_thread_num(),i);
	}
} // fim da seção paralela

	return(0);
}
