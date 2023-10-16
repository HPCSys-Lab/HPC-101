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

	// Cria um conjunto de threads, cada uma com uma cópia privada das variáveis tid,tnum,pid 
	// Demais variáveis, não explicitamente privadas, são compartilhadas

	#pragma omp parallel private(tnum,tid,pid)
	{
		// obtém informações sobre as threads omp
		nt = omp_get_num_threads();
		tnum = omp_get_thread_num();

		// obtém informações sobre as threads usando API Posix
		// normalmente, essas funções não são usadas com openmp...
		tid=pthread_self();
		pid=getpid();

		printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
			
		// Exemplo de seleção de código feito apenas pela thread master
		// if (tnum == 0) 
		#pragma omp master
			printf("Master thread: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);
		
		sleep(20);

		// Todas as threads são sincronizadas no fim da seção paralela. 
	}  

	// Apenas thread master prossegue, demais terminam.
	// Observar: variáveis ajustadas dentro do corpo da seção paralela não 
	// preservam valor ao sair...
	printf("thread_num: %d/%d. pid: %d tid:%lu\n",tnum,nt,pid,tid); fflush(stdout);

	return(0);
}
