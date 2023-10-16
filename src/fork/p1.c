/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 1: criacao de processos
** Comando: fork()
** Objetivo: observar que o mesmo codigo e' executado pelos processos pai e
             filho, que diferem apenas pelo valor de retorno do comando fork()  
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int
main()
{
	pid_t result;
	int a=5;

	printf("Este eh o pai (pid=%d). Filho n�o foi criado ainda.\n",(int)getpid());

	result=fork();

	/* Se fork funcionou, s�o 2 processos executando o mesmo codigo a partir daqui! */
	/* Caso contr�rio, apenas pai existe e vai verificar a condi��o de erro a seguir */

	if(result==-1) {
		// printf("Erro na execucao do fork\n"); 
		perror("Erro na execucao do fork"); 
		exit(0);
	}

	/* Se c�digo chegou aqui, h� 2 processos em execu��o. */
	/* result cont�m o resultado da chamada fork. Para o pai, que fez a chamada, 
	* result cont�m o pid do filho criado. Para o filho, valor de result � 0 */

	if(result==0) { /* filho */
		a+=10;
		printf("Filho: %d\n",(int)getpid()); fflush(stdout);
	}else { /* pai */
		a+=20;
		printf("Pai: %d\n",(int)getpid()); fflush(stdout);
	}

	printf("%d terminou. a=%d\n",(int)getpid(),a); fflush(stdout);

	return(0);
}

