/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : comunicacao entre processos
** Comando: pipe()
** Objetivo: ilustrar o ajuste de stdin e stdout usando pipes
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>


#define LEN 256

int
main()
{
	pid_t pid;
	int result;
	char msg[LEN];
	int pipefd[2]; // pipe[0]: read, pipe[1]: write 

	printf("Redirecionamento de stdout e stdin.\nDigite \"fim\" para terminar\n");
	
	// cria pipe
	if(pipe(pipefd)==-1) {
		perror("abertura do pipe");
		exit(0);
	}
	// cripa processo filho
	if((pid=fork())==-1) {
		perror("fork");
		exit(0);
	}

	if(pid) {  // pai: ajusta stdout (1) 

		result=dup2(pipefd[1],STDOUT_FILENO);
		// nesse caso, uma vez ajustado o stdout, pode fechar o pipe
		close(pipefd[0]); // fecha leitura do pipe 
		close(pipefd[1]); // fecha escrita no pipe 

	} else {   // filho: ajusta stdin (0) 

		result=dup2(pipefd[0],STDIN_FILENO);
		// nesse caso, uma vez ajustado o stdin, pode fechar o pipe
		close(pipefd[1]); // fecha escrita do pipe 
		close(pipefd[0]); // fecha leitura do pipe 

	}

	// Pai e filho leem de STDIN e escrevem em STDOUT 
	do {
		// char *fgets(char *s, int size, FILE *stream);
		fgets(msg,LEN,stdin);
		msg[strlen(msg)-1]='\0';
		// printf("%s\n",msg); fflush(stdout);
		fprintf(stdout,"%s\n",msg); fflush(stdout);
	} while (strncmp(msg,"fim",3));

	// precisa ajustar a condicao de saida
	// pai deveria esperar filho...

	return 0;
}

