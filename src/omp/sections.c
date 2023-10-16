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

	// Inicializa��es
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
		}  // fim das se��es
	}  // fim da regiao paralela 
	
	return(0);
}

