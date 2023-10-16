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
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define NUM 20

int 
main(int argc, char **argv) 
{
	int i, vet[NUM];
	srand((int)getpid()); 
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
	// printf("Thread %d\n",omp_get_thread_num());

	#pragma omp for 

	for (i=0; i < NUM; i++) {
		vet[i] = 2 * i;
		printf("th %d tratando itera��o %d\n",omp_get_thread_num(),i);
		sleep(rand()%5);
	}
} // fim da se��o paralela

/* 
	// forma compacta de declara��o do paralell for
	// usada quando o for � o �nico comando na regi�o paralela
	#pragma omp parallel for
	for (i=0; i < NUM; i++) {
		vet[i] = 2 * i;
		sleep(rand()%5);
		printf("th %d tratando itera��o %d\n",omp_get_thread_num(),i);
	} // fim da se��o paralela
*/
	return(0);
}
