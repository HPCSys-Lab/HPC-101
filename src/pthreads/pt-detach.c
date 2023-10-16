/* 
** Universidade Federal de S�o Carlos
** Departamento de Computa��o
** Prof. H�lio Crestana Guardia
** Programa��o Paralela e Distribu�da
*/

/*
** Programa : 
** Comando: pthread_create( ): aten��o para a passagem de par�metros para a fun��o
** Objetivo:
**           para testar, executar: pt-dettach &; ls /proc/pid_hello/task 
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NUM_THREADS	4
#define LEN	128


#define __USE_GNU

// __USE_GNU deveria evitar ter que redefinir o prototipo aqui...
int pthread_getattr_np (pthread_t __th, pthread_attr_t *__attr);


void *hello_w(void *thread_num)
{
	pthread_t id;
	pthread_attr_t attr;
	int state;

	id=pthread_self();

	// obt�m atributos desta thread
	// int pthread_getattr_np (pthread_t th, pthread_attr_t *attr);
	pthread_getattr_np (id, &attr);

	// obt�m detachstate	
	// int pthread_attr_getdetachstate (const pthread_attr_t *attr, int *detachstate);
	pthread_attr_getdetachstate(&attr, &state);

	// libera attr
	pthread_attr_destroy(&attr);

	// sleep(20);

	printf("Hello World, this is thread %d! tid=%lu pid=%d state=%d\n",
	       (int)thread_num,id,getpid(),state);

	pthread_exit(NULL);
}

int 
main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	int t, status, detach_state;
	char err_msg[LEN];
	
	if(argc<2) {
		printf("Uso: %s -j (joinable) ou -d (detached)\n",argv[0]);
		exit(0);
	}
	if(!strcmp(argv[1],"-j"))
		detach_state=PTHREAD_CREATE_JOINABLE;
	else 
		if(!strcmp(argv[1],"-d"))
			detach_state=PTHREAD_CREATE_DETACHED;
		else {
			printf("Uso: %s -j (joinable) ou -d (detached)\n",argv[0]);
			exit(0);
		}
	
	// Cria pthread_attr e ajusta atributo dettached
	pthread_attr_init(&attr);

	// op��es: PTHREAD_CREATE_JOINABLE e PTHREAD_CREATE_DETACHED
	pthread_attr_setdetachstate(&attr, detach_state);

	for(t=0; t<NUM_THREADS; t++) {
		printf("Criando thread %d\n", t);
		status = pthread_create(&threads[t], &attr, hello_w, (void *)t);
		if (status) {
			strerror_r(status,err_msg,LEN);
			printf("Erro criando thread %d: %s\n",t,err_msg);
			exit(0);
		}
	}

	// Libera estrutura dos atributos 
	pthread_attr_destroy(&attr);

	// faz detach da main thread
	// int pthread_detach (pthread_t th);
	pthread_detach(pthread_self());

	// Espera antes de terminar. Examinar a exist�ncia de "zombies".
	// Apenas 1 zombie � gerado, e apenas para a thread da fun��o main.
	sleep(20);

	pthread_exit(NULL);
}


