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



/* Algoritmo de Monte Carlo para cálculo do valor de pi */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

int 
main(int argc, char **argv) 
{
	int count;
	int i;
	int local_count;
	int samples;
	unsigned short xi[3];
	int t;
	int tid;
	double x,y;

	// numb of points and number of threads are command line arguments
	samples=atoi(argv[1]);
	omp_set_num_threads(atoi(argv[2]));

	count=0;

#pragma omp parallel private (xi,t,i,x,y,local_count)
{
	local_count=0;
	xi[0]=atoi(argv[3]);
	xi[1]=atoi(argv[4]);
	xi[2]=tid=omp_get_thread_num();
	t=omp_get_num_threads();

	for (i=tid;i< samples; i+=t) {
		x=erand48(xi);
		y=erand48(xi);
		if(x*x+y*y <= 1.0)
			local_count++;
	}	
#pragma omp critical
	count += local_count;
} // fim da seção paralela

	printf("Estimate of pi: %7.5f\n", 4.0*count/samples);

	return(0);
}
