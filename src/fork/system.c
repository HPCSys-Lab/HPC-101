/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : criacao e execucao de processos
** Comandos: fork(), system()
** Objetivo: disparar a execucao de um comando passando como parametro. 
             Observer quantas vezes a palavra "Fim" e' impressa.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


int
main()
{
	int ret;

	printf("Proc: %d  ppid: %d\n",(int)getpid(),(int)getppid());
	fflush(stdout);

	/* system dispara shell para executar o comando passando como parametro */

	// ret=system("ls"); 
	ret=system("./hello"); 
	// ret=system("sleep 10"); 
	// ret=system("sleep 10 &"); 

	// ret=system("/bin/ps -ef | grep $USER");

	printf("System retornou %d\n",ret);

	/* processo continua normalmente depois do system */

	printf("Fim\n");

	return(0);
}

