/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: 
** Objetivo: estudar espera pelo fim de thread e tratamento do valor de retorno
             join e normalmente feito na main thread apenas porque ela e que tem
             os identificadores dos threads. Sendo variaveis globais, qualquer
             thread pode fazer o join por qualquer outra do mesmo processo.
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int _num;

#define N_THREADS	3
#define LEN 128


void *soma(void *id)
{
	int i;

	// dorme um tempo aleatório
	sleep(random()%N_THREADS);

	// for (i=0; i<1000; i++)
	//		_num += (int)(random()%N_THREADS);

	_num=(int)id;

	printf("Soma (%d) = %d\n",(int)id, (int)_num);

	// void pthread_exit(void *retval);

	// pthread_exit terminates the execution of the calling thread. All cleanup handlers 
	// that have been set for the calling thread with !pthread_cleanup_push!(3) are
	// executed in reverse order (the most recently pushed handler is executed first).
	// Finalization functions for thread-specific data are then called for all keys that
	// have non-!NULL! values associated with them in the calling thread (see
	// !pthread_key_create!(3)). Finally, execution of the calling thread is stopped.
	// The |retval| argument is the return value of the thread. It can be consulted from
	// another thread using !pthread_join!(3).

	// pthread_exit(NULL);
	pthread_exit((void *)_num);
}

int 
main (int argc, char *argv[])
{
	pthread_t thread[N_THREADS];
	int result, t, final;
	char err_msg[LEN];
 
	// void srandom(unsigned int seed);
	srandom((unsigned int)getpid());


	for(t=0; t<N_THREADS; t++) {
		printf("Criando thread %d\n", t);
		result = pthread_create(&thread[t], NULL, soma, (void *)t); 
		if (result) {
			strerror_r(result,err_msg,LEN);
			printf("Erro criando thread %d: %s\n",t,err_msg);
			exit(0);
		}
	}

	// int pthread_join(pthread_t thread, void **value_ptr);

	// The pthread_join() function shall suspend execution of the calling thread until the
	// target thread terminates, unless the target thread has already terminated. On
	// return from a successful pthread_join() call with a non-NULL value_ptr argument,
	// the value passed to pthread_exit() by the terminating thread shall be made avail-
	// able in the location referenced by value_ptr. When a pthread_join() returns suc-
	// cessfully, the target thread has been terminated. 
	// If successful, the pthread_join() function shall return zero; otherwise, an error
	// number shall be returned to indicate the error.

	// espera pelas outras threads
	for(t=0; t<N_THREADS; t++) {

		// result = pthread_join(thread[t], NULL);
		// para ter resultado aleatorio, precisaria fazer o join em sequencia aleatoria tambem...
		result = pthread_join(thread[t], (void *)&final);
		if (result) {
			strerror_r(result,err_msg,LEN);
			fprintf(stderr,"Erro em pthread_join: %s\n",err_msg);
			exit(0);
		}else
			printf("Thread %d joined\n",t);
	}

	printf("Final: %d\n",final);

	pthread_exit(NULL);
}



