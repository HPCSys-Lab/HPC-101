/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 3: criacao de processos
** Comandos: fork() em loop
** Objetivo: observar a criação de processos dentro do loop: 2^N processos
             Executar em bg e usar "ps jf" 
*/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N_PROC 5

int
main(int argc, char **argv)
{
	int i,n_proc;
	pid_t result;

	if(argc>1) {
		n_proc=atoi(argv[1]);
		if((n_proc < 0) || (n_proc > N_PROC)) {
			printf("Num proc deve ser > 0 e < %d. Usando %d\n",N_PROC,N_PROC);
			n_proc=N_PROC;
		}
	}else
		n_proc=N_PROC;
/*
		assembler simplificado ...
		mov ebx, 5
		mov eax, 2	// Codigo do fork
ret:	int 80h
		... tratar valor de retorno...
		sub ebx, 1
		jnz ret
*/
	for(i=0;i<n_proc;i++) {
		printf("%d criando proc %d...\n",getpid(),i); fflush(stdout);

		if((result=fork())==-1) {
		  	perror("Erro na execucao do fork"); 
			printf("Erro na criacao do processo %d: %s\n",i,strerror(errno));
			exit(0);
		}
		/* todos os processos continuam no loop */
  }

	/* todos dormem */
	sleep(10);
	/* Como determinar quantos filhos cada processo tem que esperar? */

	// printf("%d pronto\n",getpid());

	return(0);
}

