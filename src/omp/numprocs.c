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



#include <stdio.h>
#include <omp.h>

int
main()
{
	int np=omp_get_num_procs();

	printf("Ha %d processadores...\n",np);

	return(0);
}
