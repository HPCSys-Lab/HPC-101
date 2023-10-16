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



#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int
main()
{
	int nt, tnum;
	pthread_t tid;

	printf("\n");

	// omp_set_nested(1);
	omp_set_nested(0);

	// #pragma omp parallel private(tnum,tid) num_threads(8)
	#pragma omp parallel private(tnum,tid) 
	{
		// obtém informações sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		tid=pthread_self();

		if (tnum == 0)
			printf("Master thread: %d/%d\n",tnum,nt); fflush(stdout);

		printf("Thread_num: %d/%d. Id: %lu\n",tnum,nt,tid); fflush(stdout);

		// #pragma omp parallel private(tnum,tid) num_threads(2)
		#pragma omp parallel private(tnum,tid) 
		{
			// obtém informações sobre as threads omp
			nt = omp_get_num_threads();
			tnum = omp_get_thread_num();

			tid=pthread_self();

			if (tnum == 0)
				printf("Nested master thread: %d/%d. Id: %lu\n",tnum,nt,tid); fflush(stdout);
	
			printf("Nested thread_num: %d/%d. Id: %lu\n",tnum,nt,tid); fflush(stdout);

			// omp_get_ancestor_thread_num: ainda não implementado (24/11/2009)
			// printf("Nested thread_num: %d/%d. Ancestor: %d. Id: %lu\n",
			//	 tnum,nt,omp_get_ancestor_thread_num,tid); fflush(stdout);

			sleep(30);
		}  
	}  
	printf("\n");

	return(0);
}
