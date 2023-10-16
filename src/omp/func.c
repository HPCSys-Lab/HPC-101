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

void
func()
{
	#pragma omp parallel
	printf("Thread %d dentro da funcao\n",omp_get_thread_num());
}

int
main()
{

	func();
	
	return(0);
}
