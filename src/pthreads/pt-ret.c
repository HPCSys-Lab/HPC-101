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
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int _num;

#define N_THREADS	3
#define LEN 128


void *soma(void *id)
{
	int i;

	for (i=0; i<1000; i++) {
		_num = _num + (int)(random()%N_THREADS);
	}
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
	pthread_attr_t attr;
	int result, t, *retval;
	char err_msg[LEN];
 
	// void srandom(unsigned int seed);
	srandom((unsigned int)getpid());

	// Cria pthread_attr e ajusta atributo dettached como joinable (default)
	pthread_attr_init(&attr);

	// opções: PTHREAD_CREATE_JOINABLE e PTHREAD_CREATE_DETACHED
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	for(t=0; t<N_THREADS; t++) {
		printf("Criando thread %d\n", t);
		result = pthread_create(&thread[t], &attr, soma, (void *)t); 
		if (result) {
			strerror_r(result,err_msg,LEN);
			printf("Erro criando thread %d: %s\n",t,err_msg);
			exit(0);
		}
	}

	// Libera estrutura dos atributos 
	pthread_attr_destroy(&attr);


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

		// variável de retorno do join deve ser ponteiro

		// OBS: declarando retval como inteiro, nao *, usa-se (void *)&retval
		// result = pthread_join(thread[t], (void *)&retval);
		result = pthread_join(thread[t], (void **)&retval);
		if (result) {
			strerror_r(result,err_msg,LEN);
			printf("Erro em pthread_join: %s\n",err_msg);
			exit(0);
		}else
			printf("Thread %d joined: %d\n",t,(int)retval);
	}

	pthread_exit(NULL);
}



