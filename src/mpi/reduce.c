
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


int main(int argc, char *argv[])
{
	int rank, sendbuf, recvbuf, numtasks;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	sendbuf = rank;

	// collective communications 

	// int MPI_Reduce ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, int root, MPI_Comm comm )
	MPI_Reduce(&sendbuf, &recvbuf, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

	printf("rank= %d recvbuf= %d\n",rank,recvbuf);

	// int MPI_Allreduce ( void *sendbuf, void *recvbuf, int count, MPI_Datatype datatype, MPI_Op op, MPI_Comm comm )
	MPI_Allreduce(&sendbuf, &recvbuf, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

	printf("rank= %d recvbuf= %d\n",rank,recvbuf);

	MPI_Finalize();

	return(0);
}

