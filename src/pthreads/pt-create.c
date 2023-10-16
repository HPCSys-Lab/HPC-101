/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: pthread_create( ): atenção para a passagem de parâmetros para a função
** Objetivo:
**           para testar, executar: hello &; ls /proc/pid_hello/task 
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS	4
#define LEN 128


void *hello_w(void *thread_num)
{
	pthread_t id;

	// pthread_self return the thread identifier for the calling thread.
	// pthread_t pthread_self(void);
	id=pthread_self();

	printf("Hello World, this is thread %d! tid=%lu pid=%d\n",(int)thread_num,id,(int)getpid());

	if(rand()%2)
		sleep(20);

	printf("thread %d terminando...\n",(int)thread_num);

	pthread_exit(NULL);
}

int main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	int t, status;
	char err_msg[LEN];

/*
	!pthread_create!  creates a new thread of control that executes concurrently with the calling thread. 
	The new thread applies the function |start_routine| passing it |arg| as first argument. The new thread 
	terminates either explicitly, by calling !pthread_exit!(3), or implicitly, by returning from the 
	|start_routine| function. The latter case is equivalent to calling !pthread_exit!(3) with the result
	returned by |start_routine| as exit code.

	The |attr| argument specifies thread attributes to be applied to the new thread. See !pthread_attr_init!(3) 
	for  a  complete  list  of thread  attributes.  The |attr| argument can also be !NULL!, in which case 
	default attributes are used: the created thread is joinable (not detached) and has default (non real-time) 
	scheduling policy.

	On success, the identifier of the newly created thread is stored in the location  pointed  by  the  |thread|  
	argument,  and  a  0  is returned. On error, a non-zero error code is returned.

	EAGAIN: not enough system resources to create a process for the new thread.
	EAGAIN: more than PTHREAD_THREADS_MAX threads are already active.
*/

	for(t=0; t<NUM_THREADS; t++){
		printf("Criando thread %d\n", t);

		// int pthread_create(pthread_t * thread, pthread_attr_t * attr, void * (*start_routine)(void *), void * arg);
		status = pthread_create(&threads[t], NULL, hello_w, (void *)t);
		if (status) {
			strerror_r(status,err_msg,LEN);
			printf("Falha da criacao da thread %d (%d): %s\n",t,status,err_msg);
			exit(EXIT_FAILURE);
		} else {
			printf("%lu criou thread %lu\n",pthread_self(),threads[t]);
		}
	}

	sleep(20);
	// terminar thread com ptread_exit() permite que outras threads 
	// do processo continuem em execução
	pthread_exit(NULL);

	// apenas main thread gera zombie se terminar antes das demais...

	// terminar com exit, ou return da função main, 
	// faz com que o processo e todas suas threads terminem
	// exit(0);
	// return(0);
}



