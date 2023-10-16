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
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LEN 64

typedef struct par {
	int num_int;
	char num_char[LEN];
} st_par;


void *
f_int(void *num_int)
{
	printf("Hello World, this is %d\n",(int)num_int);

	// pthread_exit(NULL);
	pthread_exit((int *)1);
}

void *
f_char(void *num_char)
{
	printf("Hello World, this is %s\n",(char *)num_char);

	// pthread_exit(NULL);
	pthread_exit((int *)2);
}

void *
f_struct(void *num_struct)
{
	st_par *param;
	param=(st_par*)num_struct;

	printf("Hello World, this is %d %s\n",param->num_int,param->num_char);

	// pthread_exit(NULL);
	pthread_exit((int *)34);
}

int main (int argc, char *argv[])
{
	pthread_t th_int, th_char, th_struct;
	int status, *retval;
	int num_int=1;
	char *num_char="dois";
	st_par num_struct;
	char err_msg[LEN];

	num_struct.num_int=3;
	strcpy(num_struct.num_char,"quatro");

	if((status=pthread_create(&th_int, NULL, f_int, (void *)num_int))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro criando th_int: %s\n",err_msg);
		exit(EXIT_FAILURE);
	}
	if((status=pthread_create(&th_char, NULL, f_char, (void *)num_char))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro criando th_cahr: %s\n",err_msg);
		exit(EXIT_FAILURE);
	}
	if((status=pthread_create(&th_struct, NULL, f_struct, (void *)&num_struct))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro criando th_struct: %s\n",err_msg);
		exit(EXIT_FAILURE);
	}

	if((status = pthread_join(th_int, (void **)&retval))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em pthread_join th_int: %s\n",err_msg);
	}else
		printf("th_int joined: %d\n",(int)retval);

	if((status = pthread_join(th_char, (void **)&retval))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em pthread_join th_char: %s\n",err_msg);
	} else
		printf("th_char joined: %d\n",(int)retval);

	if((status = pthread_join(th_struct, (void **)&retval))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em pthread_join th_struct: %s\n",err_msg);
	} else
		printf("th_struct joined: %d\n",(int)retval);

	pthread_exit(NULL);
}




