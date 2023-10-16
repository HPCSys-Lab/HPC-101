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


#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include <omp.h>

#define SIZE 10

int 
main()
{
	int i,a;
	int vet[SIZE];

#pragma omp parallel shared(a,vet) private(i)
{
	#pragma omp single
	{
	srand((int)getpid());
	a=(int)random()%SIZE;

	printf("Single construct executed by thread %d\n",omp_get_thread_num());
	}
	// uma barreira e inserida automaticamente aqui

	#pragma omp for
	for(i=0;i<SIZE;i++)
		vet[i]=a;

} // fim da regiao paralela

	printf("Depois da regiao paralela\n");
	for(i=0;i<SIZE;i++)
		printf("vet[%d] = %d\n",i,vet[i]);

	return(0);
}
