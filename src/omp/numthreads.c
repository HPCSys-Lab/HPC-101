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
#include <unistd.h>
#include <pthread.h>


int
main()
{
	int nt, tnum, pid;
	pthread_t tid;


	#pragma omp parallel private(tnum,tid,pid) num_threads(16)
	{
		// obtém informações sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		// obtém informações sobre as threads usando API Posix
		tid=pthread_self();
		pid=getpid();

		printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
			
		sleep(10);

		// Todas as threads são sincronizadas no fim da seção paralela. 
	}  

	printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);

	return(0);
}
