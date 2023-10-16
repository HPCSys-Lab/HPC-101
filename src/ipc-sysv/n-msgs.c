/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/
/*
** Programa : comunicacao entre processos usando fila de mensagem
** Comandos: msgget(), msgrcv(), msgsnd(), msgctl()
** Objetivo: 
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


int
main(int argc, char **argv)
{
	int i, j, max, erro, status;
	int *msqid;
	struct msqid_ds buf;

	if(argc<2) {
		printf("Uso: %s num_filas (>0)\n",argv[0]);
		exit(0);
	}
	if((max=atoi(argv[1]))>0) {
		msqid=(int *)malloc(max*sizeof(int));	
	}
/*
	// sysconf - Get configuration information at runtime
	// long sysconf(int name);
	// fs.mqueue.msgsize_max = 8192
	// fs.mqueue.msg_max = 10
	// fs.mqueue.queues_max = 256
	max=sysconf(_SC_MSG_MAX);
*/
	// for(i=0;i<max;i++)
	for(i=0,erro=0;i<max && !erro;i++)
		if((msqid[i]=msgget(IPC_PRIVATE,0744)) == -1) {
			printf("Erro na alocacao da fila de mensagens (%i): %s\n",i,strerror(errno));
			erro=1;
		}else
			printf("Criou fila %d: %d\n",i,msqid[i]);

	for(j=0;j<i;j++) {
		// libera fila de mensagem 
		status=msgctl(msqid[j],IPC_RMID,&buf); 
		// printf("Removeu fila %d: %d\n",j,msqid[j]);
	}
	
	return 0;
}

