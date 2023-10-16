/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: 
** Objetivo: produtor x consumidor usando threads
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>

#define N_TIMES	5
#define MAX 128


// semáforos
sem_t s_prod, s_cons; 

// buffer compartilhado
int *val;


void *
produz(void * n_times)
{
	int i;

	for (i=0; i<*n_times; i++) {
		sem_wait(&s_prod);
		// deveria usar outro semáforo (ou mutex) para garantir exclusão mútua no acesso a val
		(*val)++; 
		printf("Produziu %d\n",*val); fflush(stdout);
		sem_post(&s_cons);
	}
	pthread_exit(NULL);
}       

void *
consome(void *n_times)
{
	int i;

	for (i=0; i<*n_times; i++) {
		sem_wait(&s_cons);
		// deveria usar outro semáforo (ou mutex) para garantir exclusão mútua no acesso a val
		printf("Consumiu: %d\n",*val);
		sleep(random()%5);
		sem_post(&s_prod);
	}
	pthread_exit(NULL);
}


int
main()
{
	pthread_t prod, cons;
	pid_t pid;
	char err_msg[MAX];
	int status;
	int vezes=N_TIMES;

	*val = 0;

	// int sem_init (sem_t *sem, int pshared, unsigned int value);
	if (sem_init(&s_cons, 0, 0) == -1) {
		strerror_r(errno,err_msg,MAX);
		printf("Erro em sem_init: %s\n",err_msg);
		exit(1);
	}

	if (sem_init(&s_prod, 0, 1) == -1) {
		strerror_r(errno,err_msg,MAX);
		printf("Erro em sem_init: %s\n",err_msg);
		exit(1);
	}

	if(pthread_create(&prod, NULL, produz, (void *)vezes)) {
		strerror_r(status,err_msg,LEN);
		printf("Erro na criação da thread: %s\n",err_msg);
		exit(0);
	}
	if(pthread_create(&cons, NULL, consome, (void *)vezes)) {
		strerror_r(status,err_msg,LEN);
		printf("Erro na criação da thread: %s\n",err_msg);
		exit(0);
	}

	// espera pelas outras threads
	// variável de retorno do join deve ser ponteiro

	// OBS: declarando retval como inteiro, nao *, usa-se (void *)&retval
	// status = pthread_join(thread[t], (void *)&retval);
	if(pthread_join(prod, (void **)&status)) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em pthread_join: %s\n",err_msg);
		exit(0);
	}

	sem_destroy(&s_prod);
	sem_destroy(&s_cons);

	pthread_exit(NULL);
}



