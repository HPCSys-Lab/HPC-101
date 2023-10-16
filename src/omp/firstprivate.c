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

	// valor do pid e obtido antes da criacao das threads
	pid=getpid();

	// firstprivate permite que valor atual da variavel seja copiado para as 
	// instancias das threads
	#pragma omp parallel private(tnum,tid) firstprivate(pid) 
	{
		// obt�m informa��es sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		// obt�m informa��es sobre as threads usando API Posix
		tid=pthread_self();

		printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
			
		sleep(10);
	}  

	printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);

	return(0);
}
