/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
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

// #include "f.h"

int
main()
{
	/* printf("Hello, world! This is %d\n",(int)getpid()); */
	// printf("Alo^, mundo! Aqui e' %d\n",(int)getpid());
	printf("Alo^, mundo! Aqui e' %d, filho de %d\n",(int)getpid(),(int)getppid());

	// sleep(10);
	// f1();

	return(0);
}
