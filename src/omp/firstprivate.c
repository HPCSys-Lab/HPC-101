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

	// valor do pid e obtido antes da criacao das threads
	pid=getpid();

	// firstprivate permite que valor atual da variavel seja copiado para as 
	// instancias das threads
	#pragma omp parallel private(tnum,tid) firstprivate(pid) 
	{
		// obtém informações sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		// obtém informações sobre as threads usando API Posix
		tid=pthread_self();

		printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
			
		sleep(10);
	}  

	printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);

	return(0);
}
