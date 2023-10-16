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



#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define NUM 8

int 
main(int argc, char **argv) 
{
	int i, vet[NUM];
	srand((int) getpid()); 

/*
	#pragma omp for [clause ...]  newline 
		schedule (type [,chunk]) 
		ordered
		private (list) 
		firstprivate (list) 
		lastprivate (list) 
		shared (list) 
		reduction (operator: list) 
		collapse (n) 
		nowait 

   for_loop
*/

#pragma omp parallel 
{
	#pragma omp for nowait
	// #pragma omp for 

	for (i=0; i < NUM; i++) {
		vet[i] = 2 * i;
		printf("th %d tratando iteração %d\n",omp_get_thread_num(),i);
		sleep(rand()%5);
	}
	printf("th %d concluiu o for\n",omp_get_thread_num());

} // fim da seção paralela

	return(0);
}
