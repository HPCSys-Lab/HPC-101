/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 3: criacao de processos
** Comandos: fork() waitpid()
** Objetivo: pai cria N processos e espera cada um individualmente. Variacoes
             na duracao do sleep podem causar zombies.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N_PROC 5

int
main(int argc, char **argv)
{
	int i,n_proc;
	pid_t vet_pid[N_PROC];

	for(i=0;i<N_PROC;i++)
		vet_pid[i]=0;

	if(argc>1) {
		n_proc=atoi(argv[1]);
		if((n_proc < 0) || (n_proc > N_PROC)) {
			printf("Num proc deve ser > 0 e < %d. Usando %d\n",N_PROC,N_PROC);
			n_proc=N_PROC;
		}
	}else
		n_proc=N_PROC;

	for(i=0;i<n_proc;i++) {
		printf("%d criando proc %d...\n",getpid(),i); fflush(stdout);

		if((vet_pid[i]=fork())<0) {
			printf("Erro na criacao do processo: %d\n",i);
			exit(0);
		}
		/* todos os processos prosseguem */
  }

	/* todos dormem */
	sleep(20);
	/* Como determinar quantos filhos cada processo tem que esperar? */

	// printf("%d pronto\n",getpid());

	return(0);
}

