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



#include<stdio.h>
#include<omp.h>

#define MAX 5

int
main()
{
	int i,a,a_shared;

	// last em lastprivate corresponde ao �ltimo bloco paralelo executado: �ltima itera��o ou �ltima section	
	// valor da vari�vel definida com cl�usula lastprivate � copiado para vari�vel da master thread, que prossegue depois do trecho paralelo

	// #pragma omp parallel for private (i) private(a)
	#pragma omp parallel for private (i) lastprivate(a)
		for(i=0;i<MAX;i++) {
			a=i+1;
			printf("Thread %d has a value of a = %d for i = %d\n",omp_get_thread_num(),a,i);
		}	
	printf("Value of a after parallel for: a = %d\n",a);

	// esquema alternativo de implementa��o, N�O recomendado...

	#pragma omp parallel for private (i) shared(a_shared)
		for(i=0;i<MAX;i++) {
			a=i+1;
			printf("Thread %d has a value of a = %d for i = %d\n",omp_get_thread_num(),a,i);
			if(i==1)
				a_shared=a;
		}	
	printf("Value of a after parallel for: a = %d\n",a);

	return(0);
}
