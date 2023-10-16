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

	// Cria um conjunto de threads, cada uma com uma c�pia privada das vari�veis tid,tnum,pid 
	// Demais vari�veis, n�o explicitamente privadas, s�o compartilhadas

	#pragma omp parallel private(tnum,tid,pid)
	{
		// obt�m informa��es sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		// obt�m informa��es sobre as threads usando API Posix
		// normalmente, essas fun��es n�o s�o usadas com openmp...
		tid=pthread_self();
		pid=getpid();

		printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
			
		// Exemplo de sele��o de c�digo feito apenas pela thread master
		// if (tnum == 0) 
		#pragma omp master
			printf("Master thread: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
		
		sleep(20);

		// Todas as threads s�o sincronizadas no fim da se��o paralela. 
	}  

	// Apenas thread master prossegue, demais terminam.
	// Observar: vari�veis ajustadas dentro do corpo da se��o paralela n�o 
	// preservam valor ao sair...
	printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);

	return(0);
}
