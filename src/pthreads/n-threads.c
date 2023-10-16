/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : criacao de threads
** Comando: pthread_create()
** Objetivo: teste do numero maximo permitido - sujeito a restricoes do "ulimit -u" (?)
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <pthread.h>



#define MAX 32767
#define LEN 128


#define toSeconds(t) (t.tv_sec + (t.tv_usec/1000000.))



void *funcao(void * arg)
{
	// int id=*(int *) arg;

	// printf("Starting thread %d: self: %lu\n",id,(unsigned long)pthread_self());

	// sleep((int)random()%5); 
	sleep(5); 

	// printf("Finishing thread %d: self: %lu\n",id,(long)pthread_self());
	// printf("Finishing thread %d: self: %lu\n",*(int *)arg,(long)pthread_self());

	// pthread_exit(NULL);
	return(arg);
}

int
main(int argc, char **argv)
{
	int i,result,nt;
	// pthread_t th_vec[MAX];
	pthread_t *th_vec;
	int *retval;
	struct timeval inic,fim;
	struct rusage r1, r2;
	char err_msg[LEN];
 
	if(argc>1) {
		nt=atoi(argv[1]);
	}
	if(nt<=0 || nt>MAX) {
		printf("Uso: %s num_threads (<=%d)\n",argv[0],MAX);
		exit(0);
	}

	// alocacao dinamica do vetor de pthread_t
	th_vec=(pthread_t *)malloc(nt*sizeof(pthread_t));

	// int gettimeofday(struct timeval *tv, struct timezone *tz);
	gettimeofday(&inic,0);
	getrusage(RUSAGE_SELF, &r1);

	for(i=0;i<nt;i++) {
		// printf("Criando thread %i\n",i); fflush(stdout);

		result = pthread_create(&th_vec[i], NULL, funcao,(void *)&i);
		if (result != 0) {
			// lembrar que errno é único para cada thread
			strerror_r(result,err_msg,LEN);
			fprintf(stderr,"Erro criando thread %d: %s\n",i,err_msg); fflush(stderr);
			exit(0);
		}
		// printf("Pai criou thread %d\n",i); fflush(stdout);
	}

	gettimeofday(&fim,0);
	getrusage(RUSAGE_SELF, &r2);

	printf("\n");
	printf("Início: %f\nFim: %f\n",toSeconds(fim),toSeconds(inic));
	printf("Elapsed time:%f sec\nUser time:%f sec\nSystem time:%f sec\n\n",
		toSeconds(fim)-toSeconds(inic),
		toSeconds(r2.ru_utime)-toSeconds(r1.ru_utime),
		toSeconds(r2.ru_stime)-toSeconds(r1.ru_stime));


	// OBS: adequar tipo da variável de retorno com o retorno da funcao da thread
	for(i=0;i<nt;i++) {
		result = pthread_join(th_vec[i], (void *)&retval);
		if (result != 0) {
			strerror_r(result,err_msg,LEN);
			fprintf(stderr,"Erro em pthread_join: %s\n",err_msg);
		}else {
			// printf("Thread: %lu %d\n",th_vec[i],*retval);
		}
	}

	return(0);
}

