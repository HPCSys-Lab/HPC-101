/* 
** Universidade Federal de S�o Carlos
** Departamento de Computa��o
** Prof. H�lio Crestana Guardia
** Programa��o Paralela e Distribu�da
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
		// obt�m informa��es sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		// obt�m informa��es sobre as threads usando API Posix
		tid=pthread_self();
		pid=getpid();

		printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
			
		sleep(10);

		// Todas as threads s�o sincronizadas no fim da se��o paralela. 
	}  

	printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);

	return(0);
}
