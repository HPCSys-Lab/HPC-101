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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

// protótipo da função...
int pthread_yield (void);

/*
A  mutex  is  a  MUTual  EXclusion device, and is useful for protecting
shared data structures from concurrent modifications, and  implementing
critical sections and monitors.

A  mutex  has  two possible states: unlocked (not owned by any thread),
and locked (owned by one thread). A mutex can never  be  owned  by  two
different  threads  simultaneously. A thread attempting to lock a mutex
that is already locked by another thread is suspended until the  owning
thread unlocks the mutex first.

!pthread_mutex_init! initializes the mutex object pointed to by |mutex|
according to the mutex attributes specified in |mutexattr|.  If |mutex-
attr| is !NULL!, default attributes are used instead.
*/

#define N_THREADS 3
#define TTL			3
#define LEN 128


// declaracao do mutex deve ser global

pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER; // criacao e inicializacao estatica
// ou
// pthread_mutex_t _mutex;                          // declaracao sem iniciacao
// pthread_mutex_init(&_mutex, NULL);               // inicializar na funçao main


int _result = 0;


void *soma(void *th_num)
{
	int i;

	printf("Thread %d pronta\n",(int)th_num);
	// sleep(1);

	for (i=0; i<TTL; i++) {
		printf("Th %d esperando...\n",(int)th_num); fflush(stdout);

		pthread_mutex_lock(&_mutex);

		// printf("Th: %d+%d\n",_result,(int)th_num); fflush(stdout);
		printf("Th: %d na secao critica\n",(int)th_num); fflush(stdout);

		_result = _result + (int)th_num;

		sleep(1);

		printf("Th: %d liberou secao critica\n",(int)th_num); fflush(stdout);

		pthread_mutex_unlock(&_mutex);

		pthread_yield();
		// sleep(1);
	}
	printf("Soma = %d\n",_result);

	pthread_exit(NULL);
}


int
main (int argc, char *argv[])
{
	pthread_t thread[N_THREADS];
	int t, result;
	char err_msg[LEN];

	// cria threads
	for(t=0; t<N_THREADS; t++) {
		// printf("Criando thread %d\n", t);
		if((result=pthread_create(&thread[t], NULL, soma, (void *)t))) {
			strerror_r(result,err_msg,LEN);
			printf("Erro criando thread: %s\n",err_msg);
			exit(0);
		}
	}

	// espera threads concluírem
	for(t=0; t<N_THREADS; t++) {
		if((result=pthread_join(thread[t], NULL))) {
			strerror_r(result,err_msg,LEN);
			printf("Erro em pthread_join: %s\n",err_msg);
			exit(0);
		}
	}

	printf("Soma final: %d\n",_result);

	// libera mutex
	pthread_mutex_destroy (&_mutex);

	pthread_exit(NULL);
}


