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

int
main()
{
	int np=omp_get_num_procs();

	printf("Ha %d processadores...\n",np);

	return(0);
}
