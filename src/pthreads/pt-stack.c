/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: 
** Objetivo: estudar o efeito de alocacao de estruturas no escopo da funcao da thread.
             Como alocacao e feita na pilha, fica-se sujeito 'a limitacao existente.
             Por outro lado, estruturas na funcao main, ou no escopo do programa,
             sao naturalmente compartilhada, o que pode nao ser desejado.
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define N_THREADS 4
#define N 1000
#define FOLGA 1000000
#define LEN 128 

pthread_attr_t _attr;


void *func(void *threadid)
{
	double A[N][N];
	int i,j;
	size_t stacksize;
 
	pthread_attr_getstacksize (&_attr, &stacksize);
	printf("Thread %d: tam. pilha = %d bytes \n",(int) threadid, stacksize);

	for (i=0; i<N; i++)
		for (j=0; j<N; j++)
			A[i][j] = i*j;

	pthread_exit(NULL);
}
 
int 
main(int argc, char *argv[])
{
	pthread_t threads[N_THREADS];
	size_t stacksize;
	int t, status;
	char err_msg[LEN];

	// ulimit -s: stack size
 
	pthread_attr_init(&_attr);

	// obtém tamanho da pilha
	pthread_attr_getstacksize (&_attr, &stacksize);
	printf("Tamanho default da pilha: %d\n", stacksize);

	// stacksize = sizeof(double)*N*N+FOLGA;
	stacksize = 1000;
	printf("Tamanho da pilha necessária (com folga de %d): %d\n",FOLGA,stacksize);
	pthread_attr_setstacksize (&_attr, stacksize);

	for(t=0; t<N_THREADS; t++){
		if ((status=pthread_create(&threads[t], &_attr, func, (void *)t))) {
			strerror_r(status,err_msg,LEN);
			printf("Erro em pthread_create: %s\n", err_msg);
			exit(-1);
		}
	}

	pthread_exit(NULL);
}
