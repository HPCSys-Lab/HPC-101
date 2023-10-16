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
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>

#define N	1000
#define NUM	3


void
func(int tnum)
{
	int i;
	for(i=0;i<NUM;i++) {
		printf("tnum: %d\n",tnum);
		sleep(rand()%5);
	}
}

int
main ()
{
	int i,tnum;
	float a[N], b[N];

	// Inicializações
	for (i=0; i < N; i++) {
		a[i] = i * 1.5;
		b[i] = i + 3.1415;
	}
/*
	#pragma omp sections [clause ...]  newline 
		private (list) 
		firstprivate (list) 
		lastprivate (list) 
		reduction (operator: list) 
		nowait
*/

	// Numero de secoes pode diferir do numero de threads / processadores disponiveis
	// Nesse caso, vale a politica de escalonamento para atribuicoes...

	#pragma omp parallel shared(a,b) private(i,tnum)
	{
		// #pragma omp sections nowait
		#pragma omp sections nowait
		{
		// pode haver uma secao entre a diretiva e a primeira especificacao de secao
		// #pragma omp section
		// {
			tnum=omp_get_thread_num();
			func(tnum);
		// }
		#pragma omp section
		{
			tnum=omp_get_thread_num();
			func(tnum);
		}	
		#pragma omp section
		{
			tnum=omp_get_thread_num();
			func(tnum);
		}
		#pragma omp section
		{
			tnum=omp_get_thread_num();
			func(tnum);
		}
		}  // fim das seções
	}  // fim da regiao paralela 
	
	return(0);
}

