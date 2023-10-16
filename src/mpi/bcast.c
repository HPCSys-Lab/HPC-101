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
#include <string.h>
#include "mpi.h"

#define LEN 128

#define root 0


int
main( int argc, char *argv[])
{
	int i, rank, result, numtasks, namelen, msgtag, pid, pid_0;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	char buf[LEN];

	result = MPI_Init(&argc,&argv);

	if (result != MPI_SUCCESS) {
		printf ("Erro iniciando programa MPI.\n");
		MPI_Abort(MPI_COMM_WORLD, result);
	}

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(processor_name,&namelen);

	if(rank==root)
		sprintf(buf,"%s",processor_name);

	// Mesmo codigo é executado em todos os nós. Rank e root do bcast definem papeis.
	// Como todos usam o mesmo códo e só root sabe o tamanho da string, não dá para usar strlen...

	// int MPI_Bcast (void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
	// MPI_Bcast( buf, strlen(buf)+1, MPI_CHAR, root, MPI_COMM_WORLD); 
	MPI_Bcast( buf, LEN, MPI_CHAR, root, MPI_COMM_WORLD); 

	if(rank==root) {
		printf("%s enviou: %s\n",processor_name,buf);
	}else {
		// todos recebem de rank 0
		printf("%s (%d) recebeu: %s\n",processor_name,rank,buf);
	}

	MPI_Finalize();

	return(0);
}

