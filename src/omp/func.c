/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
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
