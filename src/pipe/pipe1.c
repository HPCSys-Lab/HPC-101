/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : comunicacao entre processos
** Comando: pipe()
** Objetivo: testar a comunicacao unidirecional utilizando pipes
**           Observar, com o uso do sleep, que dados são "bufferizados" no pipe.
**           e não é possível distringuir limites entre as mensagens.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define DATA "MSG PAI PARA FILHO"
#define N_MSG 5
#define LEN 1024

int
main()
{
	pid_t pid,i;
	int n_bytes, status;
	int pipefd[2]; // pipe[0]: read, pipe[1]: write
	char buf[LEN];

	if(pipe(pipefd)==-1) {
		perror("abertura do pipe");
		exit(0);
	}

	// imprime descritores
	printf("Pipe[0]: %d, pipe[1]:%d\n",pipefd[0],pipefd[1]);

	// cria processo filho, que herda acesso ao pipe pela 
	// cópia do vetor de arquivos abertos
	if((pid=fork())==-1) {
		perror("fork");
		exit(0);
	}
	if(pid) {	// pai: envia msg para filho
		// como pai vai enviar mensagem para filho pelo pipe, 
		// pode fechar o descritor aberto para leitura
		close(pipefd[0]);

		for(i=0;i<N_MSG;i++) {	
			sleep(1);
			// sprintf(buf,"%s %d\n",DATA,i);
			sprintf(buf,"%s %d",DATA,i);
			// if(write(pipefd[1],buf,strlen(buf)+1) ==-1)
			if(write(pipefd[1],buf,strlen(buf)) ==-1)
				perror("escrevendo mensagem...");	
		} 
		// fecha pipe
		close(pipefd[1]);
		// pai espera filho terminar
		status=wait(&pid);
		printf("Pai terminou\n"); fflush(stdout);

	}else { // filho: recebe msg do pai
		// como filho vai apenas receber mensagem do pai pelo pipe, 
		// pode fechar o descritor aberto para escrita
		close(pipefd[1]);

		for(i=0;i<N_MSG;i++) { 
			// sleep(1);
			if((n_bytes=read(pipefd[0],buf,LEN)) ==-1)
				perror("lendo mensagem...");
			buf[n_bytes]='\0';
			printf("Filho leu (%d): %s\n",n_bytes,buf); fflush(stdout);
		} 
		close(pipefd[0]);
	}

	return 0;
}


