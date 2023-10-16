/* 
** Universidade Federal de S�o Carlos
** Departamento de Computa��o
** Prof. H�lio Crestana Guardia
** Programa��o Paralela e Distribu�da
*/

/*
** Programa : 
** Comando: 
** Objetivo: 
*/



#include <unistd.h>
#include <stdio.h>

#define TAM 256

int
main()
{
	int nbytes;
	char buf[TAM];

	printf("Eco: pid=%d, ppid=%d\n\n",(int)getpid(),(int)getppid());
	fflush(stdout);

	do {
		nbytes=read(0,buf,TAM);
		if(nbytes>0) {
			buf[nbytes]='\0';
			printf("eco: %s",buf); 
			fflush(stdout);
		}
	} while (nbytes>0);

	printf("EOF\n");

	return(0);
}
