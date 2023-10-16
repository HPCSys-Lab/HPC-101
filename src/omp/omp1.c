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
main(int argc, char **argv)
{
/*
	The _OPENMP macro is defined by OpenMP-compliante implementations as the decimal constant
	yyyymm, which will be the year and month of the approved specification.
*/
	#ifdef _OPENMP
	printf("Compiled by an OpenMP-compliant implementation: %d\n",_OPENMP);
	#endif

	return(0);
}

