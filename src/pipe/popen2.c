
/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa :
** Comando: 
** Objetivo: 
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>


#define LEN 256

int
main(int argc, char *argv[])
{
	FILE *pipe_in, *pipe_out;
	char buf[LEN], cmd[LEN];

	if(argc!=2) {
		printf("Uso: %s diretório\n",argv[0]);
		exit(1);
	}

	// monta comando de 'ls' em diretório especificado

	// char *strcpy(char *dest, const char *src);
	strcpy(cmd,"ls -la ");
	// char *strcat(char *dest, const char *src);
	strcat(cmd,argv[1]);
	
	// abre pipe de entrada (r), associando stdout do cmd a ele
	if ((pipe_in = popen(cmd, "r")) == NULL) {
		perror("Erro popen");
		exit(1);
	}

	// abre pipe de saida (w), associando stdout do sort... a ele
	if ((pipe_out = popen("sort -nr +4 | more", "w")) == NULL) {
		perror("Erro popen");
		exit(1);
	}

	// lê de stdin e escreve em stdout
	do { 
		fgets(buf, LEN, pipe_in);

		if(feof(pipe_in))
			break;

		// int fputc(int c, FILE *stream);
		// int fputs(const char *s, FILE *stream);
		fputs(buf, pipe_out);
	} while(!feof(pipe_in));


	// fecha pipes e espera processos terminarem
	pclose(pipe_in);
	pclose(pipe_out);

	return(0);
}

