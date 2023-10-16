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
#include <unistd.h>
#include <pthread.h>
#include <errno.h>


#define N_THREADS 2
#define TTL			5
#define LEN			128

/* Yield the processor to another thread or process.
   This function is similar to the POSIX `sched_yield' function but might be 
	differently implemented in the case of a m-on-n thread implementation.  */

int pthread_yield (void);


// declaracao do mutex deve ser global

pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER; // criacao e inicializacao estatica

int _result = 0;


void *
soma(void *th_num)
{
	int i, st1, st2;
	char err_msg[LEN];

	printf("Thread %d pronta\n",(int)th_num);

/*
	The pthread_mutex_trylock() function shall be equivalent to
	pthread_mutex_lock(), except that if the mutex object referenced by
	mutex is currently locked (by any thread, including the current
	thread), the call shall return immediately. If the mutex type is
	PTHREAD_MUTEX_RECURSIVE and the mutex is currently owned by the calling
	thread, the mutex lock count shall be incremented by one and the
	pthread_mutex_trylock() function shall immediately return success.

	The pthread_mutex_trylock() function shall return zero if a lock on the
	mutex  object referenced by mutex is acquired. Otherwise, an error num-
	ber is returned to indicate the error.

	EINVAL The  mutex  was  created  with the protocol attribute having the
		value PTHREAD_PRIO_PROTECT and the calling thread's priority  is
		higher than the mutex's current priority ceiling.
	EBUSY  The mutex could not be acquired because it was already locked.
	EINVAL The value specified by mutex does not refer  to  an  initialized
		mutex object.
	EAGAIN The  mutex  could  not be acquired because the maximum number of
		recursive locks for mutex has been exceeded.
	EDEADLK The current thread already owns the mutex.
	EPERM  The current thread does not own the mutex.
*/
	for (i=0; i<TTL; i++) {

		do {
			// int pthread_mutex_trylock(pthread_mutex_t *mutex);
			// if((st1=pthread_mutex_trylock(&_mutex))!=0) {
			if((st1=pthread_mutex_trylock(&_mutex))) {
				strerror_r(st1,err_msg,LEN);
				printf("Falha em pthread_mutex_trylock (%d): %s\n",st1,err_msg);

				// aqui, deveria fazer outra coisa para aproveitar o tempo

				if((st2=pthread_yield())) {
					strerror_r(st2,err_msg,LEN);
					printf("Erro em pthread_yield (%d): %s\n",st2,err_msg);
				}
				sleep(1);
			}
		} while (st1);

		printf("Thread %d bloqueou mutex\n",(int)th_num);

		printf("Th: %d+%d\n",_result,(int)th_num); fflush(stdout);
		_result = _result + (int)th_num;

		sleep(3);

		pthread_mutex_unlock(&_mutex);

		printf("Thread %d liberou mutex\n",(int)th_num);

		// força troca de contexto. Isso, contudo, não é suficiente para fazer 
		// com que a outra thread assuma a cpu e bloqueie o mutex
		if((st2=pthread_yield())) {
			strerror_r(st2,err_msg,LEN);
			printf("Erro em pthread_yield (%d): %s\n",st2,err_msg);
		}
		sleep(1);
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
		if(pthread_join(thread[t], NULL)) {
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


