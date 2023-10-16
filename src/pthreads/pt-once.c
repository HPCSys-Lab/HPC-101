
/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: pthread_once( ): 
** Objetivo:
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>


#define NUM_THREADS	4
#define LEN 128


/* Yield the processor to another thread or process.
   This function is similar to the POSIX `sched_yield' function but might be 
	differently implemented in the case of a m-on-n thread implementation.  */

int pthread_yield (void);


/* Guarantee that the initialization function INIT_ROUTINE will be called
   only once, even if pthread_once is executed several times with the
   same ONCE_CONTROL argument. ONCE_CONTROL must point to a static or
   extern variable initialized to PTHREAD_ONCE_INIT.
*/
// int pthread_once (pthread_once_t *once_control, void (*init_routine)(void));


pthread_once_t pt_once = PTHREAD_ONCE_INIT;


int _init_val=0;

void normal_init()
{
	_init_val+=1;
	printf("_init_val: %d\n",_init_val);
}

void init_routine()
{
	printf("Doing that \"do once\" thing...\n");
}


void *hello_w(void *thread_num)
{
	sleep((int)random()%5);
	int result;
	char err_msg[LEN];

	pthread_once(&pt_once, init_routine);

	// dorme tempo aleatório
	srandom(getpid());
	sleep(random()%5);

	// por que não usar variável global? "Race condition": if...time-slice...sched _init=1;
	if(!_init_val) {
		if((result=pthread_yield())) {
			strerror_r(result,err_msg,LEN);
			printf("Erro em pthread_yield: %s\n",err_msg);
		}
		normal_init();
	}

	printf("Hello World, this is thread %d!\n",(int)thread_num);

	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int t, status;

	for(t=0; t<NUM_THREADS; t++){
		// printf("Criando thread %d\n", t);
		if((status=pthread_create(&threads[t], NULL, hello_w, (void *)t))) {
			printf("Falha da criacao da thread %d: %d\n",t,status);
			exit(EXIT_FAILURE);
		}
	}
	pthread_exit(NULL);
}




