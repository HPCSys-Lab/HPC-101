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


// Exemplo de compila��o condicional do suporte �s fun��es da API OpenMP 

#ifdef _OPENMP
	#include <omp.h>
#else
	#define omp_get_thread_num() 0
	#define omp_get_num_threads() 1
#endif


int
main(int argc, char **argv)
{

// The _OPENMP macro is defined by OpenMP-compliante implementations as the decimal 
// constant yyyymm, which will be the year and month of the approved specification.

	#ifdef _OPENMP
	printf("Compiled by an OpenMP-compliant implementation: %d\n",_OPENMP);
	#endif

	return(0);
}

