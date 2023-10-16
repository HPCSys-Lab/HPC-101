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

/*
	FILE *popen(const char *command, const char *type);
	int pclose(FILE *stream);

	The  popen()  function  opens a process by creating a pipe, forking, and invoking the
	shell.  Since a pipe is by definition unidirectional, the type argument  may  specify
	only  reading or writing, not both; the resulting stream is correspondingly read-only
	or write-only.

	The command argument is a pointer to a null-terminated string containing a shell com-
	mand  line.   This command is passed to /bin/sh using the -c flag; interpretation, if
	any, is performed by the shell.  The type argument is a pointer to a  null-terminated
	string which must be either `r' for reading or `w' for writing.

	The  return  value  from popen() is a normal standard I/O stream in all respects save
	that it must be closed with pclose() rather than fclose().  Writing to such a  stream
	writes  to  the  standard  input of the command; the command's standard output is the
	same as that of the process that called popen(), unless this is altered by  the  com-
	mand itself.  Conversely, reading from a ``popened'' stream reads the command's stan-
	dard output, and the command's standard input is the same as that of the process that
	called popen().

	Note that output popen() streams are fully buffered by default.
	
	The  pclose()  function waits for the associated process to terminate and returns the
	exit status of the command as returned by wait4().
*/


int
main()
{
	FILE *fp;
	char msg[LEN];

	printf("popen1: pid=%d\n\n",(int)getpid()); fflush(stdout);

	// Cria pipe para escrita e inicia "eco" com stdin redirecionado para o pipe
	if((fp=popen("eco","w"))==NULL) {
		perror("Execucao do popen");
		exit(0);
	}

	printf("Executou popen(\"eco\",\"w\").\nEnviando echo para stdout.\n\n");

	/* le de STDIN e escreve em STDOUT */
	do {
		// char *fgets(char *s, int size, FILE *stream);
		fgets(msg,LEN,stdin);
		msg[strlen(msg)-1]='\0';
		fprintf(fp,"%s\n",msg); fflush(fp);
	} while (strcmp(msg,"fim"));

	// espera processo acabar e fechar pipe
	if(pclose(fp)==-1)
		perror("Erro em pclose");

	return 0;
}
