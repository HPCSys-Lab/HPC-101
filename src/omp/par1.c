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
/*
	#pragma omp parallel [clause[[,]clause]...] new-line
		structured-block
	clause is one of the following:
		if(scalar-expression)
		private(variable-list)
		firstprivate(variable-list)
		default(shared | none)
		copying(variable-list)
		reduction(operator: variable-list)
		num_threads(integer-expression)

	When a thread encounters a parallel construct, a team of threads is created if
	one of the following cases is true:
		- No if clause is present
		- The if expression evaluates to a nonzero value
	
	The thread becomes the master thread of the team, with a thread number of 0, 
	and all threads in the team, including the master thread, execute the region in
	parallel. If the value of the if expression is zero, the region is serialized.

	To determine the number of threads thar are requested, the following rules will be 
	considered in order. The first rule whose condition is met will be applied.
	- If the num_threads clause is present, then the value of the integer expression is 
	  the number of threads requested
	- If the omp_set_num_threads library function has been called, then the value of 
	  the argument in the most recently executed call is thenumber of threads requested
	- If the environment variable OMP_NUM_THREADS is defined, then the value of this
	  environment variable is the number of threads requested
	- If none of the methods above were used, then the number of threads requested 
	  is implementation-defined

	The number of threads that execute the paralel region also depends upon whether
	or not dynamic adjustments of the number of threads is enabled. If dynamic 
	adjustments is enabled then the requested number of threads is the maximum
	number of threads that may execute the parallel region.

	The omp_set_dynamic library function and the OMP_DYNAMIC environment variable
	can be used do enable and disable dynamic adjustments of the number of threads.

	
	There is an implied barried at the end of a parallel region. Only the master 
	thread of the team continues execution at the end of a parallel region.

 	If a thread in a team executing a parallel region encounters another parallel 
	construct, it creates a new team, and it becomes the master of that new team. 
	Nested parallel regions are serialized by default. As a result, a nested
	parallel region is executed by a team composed of one thread. The default
	behaviour may be changed by using either the runtime library funcion 
	omp_set_nested or the environment variable OMP_NESTED. 
*/

	#pragma omp parallel 
	printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
	
	printf("Terminando...\n");

	return(0);
}

