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
	int nt, tnum, pid;
	pthread_t tid;

	#pragma omp parallel private(tnum,tid,pid)
	{
		// obtém informações sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		tid=pthread_self();

		// se 1 thread termina, demais também terminam...
		if (tnum == 0) {
			printf("Master thread: %d/%d. tid:%lu\n",tnum,nt,tid); fflush(stdout);
			exit(0);
		}
		sleep(20);

		printf("thread_num: %d/%d. tid:%lu\n",tnum,nt,tid); fflush(stdout);

		// Todas as threads são sincronizadas no fim da seção paralela. 
	}  

	printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);

	return(0);
}
