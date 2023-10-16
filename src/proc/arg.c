/* 
** Universidade Federal de S�o Carlos
** Departamento de Computa��o
** Prof. H�lio Crestana Guardia
*/

/*
** Programa: arg
** Comando: 
** Objetivo: verificar o uso de argumentos no programa atrav�s da fun��o main
*/


#include <stdio.h>

// int main (int argc, char** argv)
int main (int argc, char* argv[])
{
	int i;
	printf("\nPrograma: %s, chamado com %d argumentos\n", argv[0], argc-1);

	/* lembrar da conversao de formatos: atoi() atof() */

	if (argc > 1) {
		printf ("\nArgumentos:\n");
		for (i = 1; i < argc; ++i)
			printf ("  %s\n", argv[i]);
	}
	printf ("\n");

	return(0);
}
