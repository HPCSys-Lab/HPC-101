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


#include "mpi.h"
#include <stdio.h>

int
main( int argc, char *argv[])
{
	int numtasks, rank, status, namelen;
	double startwtime, endwtime;
	char processor_name[MPI_MAX_PROCESSOR_NAME];

/*
	int i;
		for(i=0;i<argc;i++)
	printf("%d: %s\n",i,argv[i]);
*/

	status = MPI_Init(&argc,&argv);

	// startwtime = MPI_Wtime();

	if (status != MPI_SUCCESS) {
		printf ("Erro em MPI_Init. Terminando...\n");
		MPI_Abort(MPI_COMM_WORLD, status);
	}

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(processor_name,&namelen);

	fprintf(stderr,"Processo %d de %d em %s\n", rank,numtasks,processor_name);

	// endwtime = MPI_Wtime();
	// printf("Wall clock time: %f\n", endwtime-startwtime);

	MPI_Finalize();

	return(0);
}

