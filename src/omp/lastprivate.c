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



#include<stdio.h>
#include<omp.h>

#define MAX 5

int
main()
{
	int i,a,a_shared;

	// last em lastprivate corresponde ao último bloco paralelo executado: última iteração ou última section	
	// valor da variável definida com cláusula lastprivate é copiado para variável da master thread, que prossegue depois do trecho paralelo

	// #pragma omp parallel for private (i) private(a)
	#pragma omp parallel for private (i) lastprivate(a)
		for(i=0;i<MAX;i++) {
			a=i+1;
			printf("Thread %d has a value of a = %d for i = %d\n",omp_get_thread_num(),a,i);
		}	
	printf("Value of a after parallel for: a = %d\n",a);

	// esquema alternativo de implementação, NÃO recomendado...

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
