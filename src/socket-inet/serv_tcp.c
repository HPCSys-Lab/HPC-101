/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : processo servidor que se comunica atraves de socket
** Comandos: socket(), bind(), listen(), accept(), read(), write()
** Objetivo: ilustrar a comunicacao clienteXservidor usando socket.
             Observar o registro de endereco do servidor usando bind().
*/


#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sched.h>


#define S_PORT		8000
#define MIN_PORT	1024
#define MAX_PORT	65537

#define TAM_MSG	65537
#define LEN			128


int 
main(int argc, char **argv)
{
	int sockfd, newsock, cli_len, n_bytes, porta;
	struct sockaddr_in cli_addr, bind_addr;
	pid_t pid;
	char serv_name[LEN];
	char msg[TAM_MSG];
 
	if(argc>1) {
		porta=atoi(argv[1]);
		if(porta<=MIN_PORT || porta>MAX_PORT)
			porta=S_PORT;
	}else
		porta=S_PORT;

	// determina nome do servidor local
	if(gethostname(serv_name,LEN)<0) {
		perror("erro obtendo nome do servidor local");
		exit(0);
	}

	// cria socket TCP 
	if((sockfd=socket(PF_INET,SOCK_STREAM,0)) <0) {
		perror("erro na criacao do socket");
		exit(0);
	}
	// ajusta endereco local 
	memset((char *) &bind_addr,0, sizeof(bind_addr));
	bind_addr.sin_family      = PF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY); // espera conexao qquer end local
	bind_addr.sin_port        = htons(porta);

	// int listen(int sockfd, int backlog);
	// 
	// To  accept  connections, a socket is first created with socket(2), a willingness to
	// accept incoming connections and a queue limit for incoming connections  are  speci-
	// fied with listen(), and then the connections are accepted with accept(2).  The lis-
	// ten() call applies only to sockets of type SOCK_STREAM or SOCK_SEQPACKET.
	// 
	// The backlog parameter defines the maximum length the queue of  pending  connections
	// may  grow  to.   If a connection request arrives with the queue full the client may
	// receive an error with an indication of ECONNREFUSED or, if the underlying  protocol
	// supports retransmission, the request may be ignored so that retries succeed.

	// indica ao SO que aceitará conexões nesse socket, das origens especificadas e na porta indicada
	if(bind(sockfd, (struct sockaddr *) &bind_addr, sizeof(bind_addr)) < 0) {
		perror("erro na execucao do bind");
		exit(0);
	}

	// habilita buffer para pedidos de conexoes pendentes 
	// listen - listen for connections on a socket
	// To  accept connections, a socket is first created with socket(2), a willing-
	// ness to accept incoming connections and a queue limit for  incoming  connec-
	// tions  are  specified  with  listen(), and then the connections are accepted
	// with  accept(2).   The  listen()  call  applies  only  to  sockets  of  type
	// SOCK_STREAM or SOCK_SEQPACKET.
	// 
	// The  backlog  parameter defines the maximum length the queue of pending con-
	// nections may grow to.  If a connection request arrives with the  queue  full
	// the  client  may  receive an error with an indication of ECONNREFUSED or, if
	// the underlying protocol supports retransmission, the request may be  ignored
	// so that retries succeed.
	//
	// int listen(int sockfd, int backlog);
printf("Antes do listen...\n"); fflush(stdout);
sleep(10);
	listen(sockfd, 5);
printf("Depois do listen...\n"); fflush(stdout);

	printf("Servidor %s esperando na porta %d\n",serv_name, porta); fflush(stdout);

	do {
		// espera pedido de conexao 
		cli_len=sizeof(struct sockaddr_in);
		newsock=accept(sockfd,(struct sockaddr *)&cli_addr, (socklen_t*)&cli_len);

		if(newsock==-1) {
			perror("erro no accept");

		}else {
			if((pid=fork())==-1) {
				perror("Erro no fork");
				close(sockfd);
				exit(0);
			}
			if(!pid) {
				// filho fecha socket original
				close(sockfd);

				printf("Servidor recebeu conexão - sockfd: %d, newsock: %d\n",sockfd,newsock); 
				fflush(stdout);

				do {
					// espera dados do cliente
					// Leitura e escrita so podem ocorrer no novo socket
					// if((n_bytes=read(sockfd, msg, TAM_MSG))<0) {
					if((n_bytes=read(newsock, msg, TAM_MSG))<0) {
						perror("Servidor: erro recebendo dados");
					}else {
						msg[n_bytes]='\0';
						// printf("Servidor recebeu (%d): %s\n",n_bytes,msg);
						printf("Servidor recebeu (%d)\n",n_bytes); fflush(stdout);
						// teste para verificar envio de ack
						// sleep(1);	
						// sched_yield();
					}
					// envia msg para cliente
					if((n_bytes=write(newsock,msg,strlen(msg)))==-1)
						perror("Servidor: erro escrevendo no socket");

				} while (strncmp(msg,"fim",3) && strncmp(msg,"quit",4));

				// fecha socket
				close(newsock);
				exit(0);

			} // filho	
		}
		// pai fecha newsock
		close(newsock);

	// rever logica do processo pai, que nao esta lendo dados do socket
	} while (strncmp(msg,"quit",4));

	printf("Servidor encerrando...\n");

	return 0;
}

