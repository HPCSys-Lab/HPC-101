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

int
main( int argc, char *argv[])
{
	int i, rank, result, numtasks, namelen, msgtag, pid, pid_0;
	char processor_name[MPI_MAX_PROCESSOR_NAME];
	char tx_buf[LEN], rx_buf[LEN];
	MPI_Status status;

	result = MPI_Init(&argc,&argv);

	if (result != MPI_SUCCESS) {
		printf ("Erro iniciando programa MPI.\n");
		MPI_Abort(MPI_COMM_WORLD, result);
	}

	MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Get_processor_name(processor_name,&namelen);

	pid=getpid();

	if(rank==0) {
		msgtag=1;
		// rank 0 envia para todos os demais
		for(i=1;i<numtasks;i++) {
			// int MPI_Send(void *buf, int count, MPI_Datatype dtype, int dest, int tag, MPI_Comm comm)
			MPI_Send(&pid,1,MPI_INT,i,msgtag,MPI_COMM_WORLD);
			// printf("%s enviou: %d\n",processor_name,pid);
		}
	} else {
		// todos recebem de rank 0
		msgtag=1;
		// int MPI_Recv(void* buf,int count,MPI_Datatype datatype,
		//              int soustatuse,int tag,MPI_Comm comm,MPI_Status *status);
		MPI_Recv(&pid_0,1,MPI_INT,0,msgtag,MPI_COMM_WORLD,&status);
		// printf("%s recebeu: %d\n",processor_name,pid_0);
	}

	// Ranks != 0 enviam msg para rank 0
	if(rank!=0) {
		sprintf(tx_buf,"%s: rank=%d, pid 0=%d",processor_name,rank,pid_0);

		// int MPI_Send(void *buf, int count, MPI_Datatype dtype, int dest,
		//              int tag, MPI_Comm comm)
		msgtag=pid;
		MPI_Send(tx_buf,strlen(tx_buf)+1,MPI_CHAR,0,msgtag,MPI_COMM_WORLD);

	} else {
		for(i=0;i<numtasks-1;i++) {
			// rank 0 recebe dos demais (MPI_Comm_size -1)
			// Uso de MPI_ANY_SOURCE e MPI_ANY_TAG: não se sabe a orde de envio
			// int MPI_Recv(void *buf, int count, MPI_Datatype dtype, 
			//              int src, int tag, MPI_Comm comm, MPI_Status *stat)
			MPI_Recv(rx_buf,LEN,MPI_CHAR,MPI_ANY_SOURCE,MPI_ANY_TAG,MPI_COMM_WORLD,&status);

			// Campos de MPI_Status. NÃO deveriam ser usados diretamente...
			// MPI_Status {
			//    int MPI_SOURCE;
			//    int MPI_TAG;
			//    int MPI_ERROR;
			//    int st_length;  /* message length */
			// };
			printf("%s: source=%d, tag=%d, mensagem=%s\n",
			       processor_name,status.MPI_SOURCE,status.MPI_TAG,rx_buf);
		}
	}

	MPI_Finalize();

	return(0);
}

