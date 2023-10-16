/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/
/*
** Programa : comunicacao entre processos usando fila de mensagem
** Comandos: msgget(), msgrcv(), msgsnd(), msgctl()
** Objetivo: Verificar a sintaxe dos comandos e o significado dos flags de 
             transmissao e recepcao.
             Executar o programa em "background" e usar o comando ipcs para 
             verificar os recursos alocados. Ensinar o comando ipcrm.
             Verificar o efeito de um signal, quando o processo suspenso volta
             para "foreground".
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define MSGKEY 75
#define MSGSIZE 80


int
main()
{
	int msqid,status,size,type;
	pid_t pid;
	struct msqid_ds buf;

	// redefinição do tipo em função do tamanho das mensagens que serão enviadas
	struct msgbuf {
		long mtype;           // message type, must be > 0 
		char mtext[MSGSIZE];  // message data
	}msg;

	// outra forma de alocação da estrutura (?)
	// struct msgbuf *msg;
	// msg=(struct msgbuf *)malloc(sizeof(struct msgbuf)+MSGSIZE-1);

	// if((msqid=msgget(IPC_PRIVATE,0700)) == -1) { 
	if((msqid=msgget(MSGKEY,0777|IPC_CREAT)) == -1) {
		printf("Erro na alocacao da fila de mensagens\n");
		exit(0);
	}
	printf("msqid: %d\n",msqid);

	if((pid=fork())<0) {
		printf("Erro na criacao do processo filho\n");
		exit(0);
	}
	if(!pid) { /* filho */
		pid=getpid();
		do{
			// sleep(10);
			sleep(1);

// msg.mtype:
//  0, primeira msg disponivel;
// <0, primeira mensagem com mtype menor ou igual módulo do valor informado
// >0, msg deste tipo; na inexistencia, vai dormir 
//     ate' que a msg chegue. Caso o flag IPC_NOWAIT
//     seja usado, retorna com valor de erro.

			// if((size=msgrcv(msqid,&msg,256,0,0)) < 0 ) { // tipo = 0 : recebe qualquer msg
			// if((size=msgrcv(msqid,&msg,256,pid,IPC_NOWAIT)) < 0 ) { // pid usado como tipo
			// if((size=msgrcv(msqid,&msg,MSGSIZE,pid,0)) < 0 ) { // pid usado como tipo
			if((size=msgrcv(msqid,&msg,MSGSIZE,0,0)) < 0 ) { // pid usado como tipo
				perror("Filho: erro recebendo mensagem"); 
				exit(0);
			}else {
				msg.mtext[size]='\0'; 
				printf("Filho recebeu: (%d) %s\n",(int)msg.mtype,msg.mtext); fflush(stdout);
			}
		} while(strncmp(msg.mtext,"fim",3));

	exit(0);

	}else { /* pai */
		type=pid;
		msg.mtype=type; 
		do {
			printf("Msg: "); fflush(stdout);
			fgets(msg.mtext,MSGSIZE,stdin);
			// forca fim de string
			// msg.mtext[strlen(msg.mtext)-1]='\0';
			// strlen -1 para retirar o fim de linha do fgets
			if(msgsnd(msqid,&msg,strlen(msg.mtext)-1,IPC_NOWAIT)<0) {
				perror("Erro no envio da mensagem"); 
				strcpy(msg.mtext,"fim");
			}
		} while(strncmp(msg.mtext,"fim",3));

		/* espera filho terminar */
		wait(&status);

		/* libera fila de mensagem */
		status=msgctl(msqid,IPC_RMID,&buf); 
	}

	return 0;
}

