/* 
** Universidade Federal de S�o Carlos
** Departamento de Computa��o
** Prof. H�lio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: 
** Objetivo: 
*/

#include <stdio.h> 
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

// #include "f.h"

int
main()
{
	/* printf("Hello, world! This is %d\n",(int)getpid()); */
	// printf("Alo^, mundo! Aqui e' %d\n",(int)getpid());
	// printf("Alo^, mundo! Aqui e' %d, filho de %d\n",(int)getpid(),(int)getppid());
	printf("Alo^, mundo! Aqui e' %d, filho de %d tid:%lu\n",(int)getpid(),(int)getppid(),(int)pthread_self());

	// sleep(10);
	// f1();

	return(0);
}