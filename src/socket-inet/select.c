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
**    Observar que processo filho vira zombie. Usar wait/waitpid para encerrá-lo.
**    Como encerrar processo pai? 
*/

#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

/* According to POSIX 1003.1-2001 */
#include <sys/select.h>

#define S_PORT		8000
#define MIN_PORT	1024
#define MAX_PORT	65537

#define TAM_MSG	128
#define LEN			128

#define TIMEOUT	10
#define MAXCON 	5
#define MAXREQ		5

/*
	int select(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, 
           struct timeval *timeout);

	int pselect(int n, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, 
            const struct timespec *timeout, const sigset_t *sigmask);

	FD_CLR(int fd, fd_set *set);
	FD_ISSET(int fd, fd_set *set);
	FD_SET(int fd, fd_set *set);
	FD_ZERO(fd_set *set);
*/

int
main(int argc, char **argv)
{
	int sockfd, newsock, addr_len, n_bytes, porta, n_sock;
	struct sockaddr_in bind_addr, remote_addr;
	struct timeval tv;
	char serv_name[LEN];
	char msg[TAM_MSG];
	pid_t pid;
	fd_set read_set;
	int maxcon=0; 
	int maxreq;

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
	// monta mensagem de resposta
	sprintf(msg,"Hello from %s!\n",serv_name);

	// cria socket TCP
	if((sockfd=socket(PF_INET,SOCK_STREAM,0)) <0) {
		perror("erro na criacao do socket");
		exit(0);
	}

	// ajusta estrutura para bind: de quem aceita conexões
	memset((char *) &bind_addr,0, sizeof(bind_addr));
	bind_addr.sin_family      = PF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY); // qquer end local
	bind_addr.sin_port        = htons(porta);

	// registra recebimento atraves do socket
 	// int bind(int sockfd, const struct sockaddr *my_addr, socklen_t addrlen);
	//
	// bind()  gives  the  socket  sockfd  the  local  address my_addr.  my_addr is
	// addrlen bytes long.  Traditionally, this is called "assigning a  name  to  a
	// socket."  When a socket is created with socket(2), it exists in a name space
	// (address family) but has no name assigned.
	// It is normally necessary to assign a local address  using  bind()  before  a
	// SOCK_STREAM socket may receive connections (see accept(2)).
	if(bind(sockfd, (struct sockaddr *) &bind_addr, sizeof(bind_addr)) < 0) {
		perror("erro na execucao do bind");
		exit(0);
	}

	// habilita buffer para pedidos de conexoes pendentes
	listen(sockfd, 5);

	printf("Servidor %s esperando na porta %d\n",serv_name, porta);
	fflush(stdout);

	do {
		// espera pedido de conexao
		addr_len=sizeof(struct sockaddr_in);

		// int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
		if((newsock=accept(sockfd,(struct sockaddr *)&remote_addr,(socklen_t *)&addr_len))<0) {
			perror("Falha na conexão");

		} else {
			// incrementa número de conexões
			maxcon++;

			printf("Servidor conectado - sockfd: %d, newsock: %d PID: %d\n",sockfd,newsock,getpid());

			if((pid=fork())<0) {
				perror("Erro no fork");
			   exit(0);
			}

			if(pid==0) { //filho
				maxreq=0;
				do {
					FD_ZERO(&read_set);
					FD_SET(newsock,&read_set);

					tv.tv_sec=TIMEOUT;
					tv.tv_usec=0;

					if((n_sock=select(FD_SETSIZE,&read_set,NULL,NULL,&tv))<0) {
						perror("Erro na Funcao Select\n");
						// interrompe o loop
						break;
					}
					if(!FD_ISSET(newsock,&read_set)) {
						printf("Estouro do timeout. Fechando conexão...\n");
						// interrompe o loop
						break;
					}
					addr_len=sizeof(addr_len);
					if((n_bytes=read(newsock, msg, TAM_MSG))>=0) {
						msg[n_bytes]='\0';
						printf("Servidor recebeu (%d): %s\n",n_bytes,msg);
						maxreq++;
					}
					// envia msg para cliente
					if((n_bytes=write(newsock,msg,strlen(msg)))==-1) {
						perror("Servidor: erro escrevendo no socket");
						// interrompe o loop
						break;
					}
				} while (strncmp(msg,"fim",3) && (maxreq<MAXREQ));

				close(newsock);
				if(maxreq>=MAXREQ)
					printf("Terminando: número máximo de requisições atingido...\n");
				printf("Filho encerrando...\n");
				exit(0);
	
			} else {	 // pai
				// fecha socket
				close(newsock);

				// waitpid(...);
				if(maxcon>=MAXCON) {
					printf("Terminando: número máximo de processos atingido...\n");
					exit(0);
				}
			}
		}

	} while (1);

	return(0);
}
