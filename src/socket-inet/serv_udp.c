/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : processo servidor que se comunica atraves de socket com UDP
** Comandos: socket(), recvfrom(), sendto()
** Objetivo: ilustrar a comunicacao clienteXservidor usando socket.
             Observar o registro de endereco do servidor usando bind().
             Ilustrar a recepcao de pedidos e e o envio de dados para enderecos
             variados.
*/

#include <sys/time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#define S_PORT		8001
#define MIN_PORT	1024
#define MAX_PORT	32767
#define TAM_MSG	1024
#define LEN			128

/*
// Structure describing an Internet (IP) socket address. 
#define __SOCK_SIZE__   16    // sizeof(struct sockaddr) 
struct sockaddr_in {
  sa_family_t     sin_family;   // Address family    
  unsigned short int sin_port;  // Port number  
  struct in_addr  sin_addr;     // Internet address

  // Pad to size of `struct sockaddr'.
  unsigned char      __pad[__SOCK_SIZE__ - sizeof(short int) -
         sizeof(unsigned short int) - sizeof(struct in_addr)];
};
*/

int 
main(int argc, char **argv)
{
	int sockfd, porta, addr_len, n_bytes;
	struct sockaddr_in cli_addr, bind_addr;
	char msg[TAM_MSG];
	char server_name[LEN];

	if(argc>1) {
		porta=atoi(argv[1]);
		if(porta<=MIN_PORT || porta>MAX_PORT)
			porta=S_PORT;
	}else
		porta=S_PORT;

	// cria socket UDP 
	if((sockfd=socket(PF_INET,SOCK_DGRAM,0)) <0) {
		perror("Erro na criacao do socket");
		exit(0);
	}
	// determina nome do servidor local 
	if(gethostname(server_name,LEN)<0) {
		perror("Erro obtendo nome do servidor local");
		exit(0);
	}
	// ajusta endereco local 
	memset((char *) &bind_addr,0, sizeof(bind_addr));
	bind_addr.sin_family      = PF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY); // escuta em qquer interface
	bind_addr.sin_port        = htons(porta);

	// registra porta local
	if(bind(sockfd, (struct sockaddr *)&bind_addr,sizeof(bind_addr)) < 0) {
		perror("Erro na execucao do bind");
		exit(0);
	}

	printf("Servidor %s escutando na porta %d\n",server_name,porta); fflush(stdout);

	do {
		addr_len=sizeof(struct sockaddr);
		n_bytes=recvfrom(sockfd,msg,TAM_MSG,0,(struct sockaddr *)&cli_addr,(socklen_t*)&addr_len);
		if(n_bytes<0){
		      perror("Servidor: erro recebendo dados");
		}else {
			msg[n_bytes]='\0'; 
			printf("%s recebeu(%d): %s\n",server_name,n_bytes,msg);

			addr_len=sizeof(struct sockaddr);
			// if((n_bytes=sendto(sockfd,server_name,strlen(server_name),0,
			if((n_bytes=sendto(sockfd,msg,strlen(msg),0,
					(struct sockaddr *)&cli_addr,addr_len))<0)
				perror("servidor: erro enviando dados para cliente");
		} 
	} while(strncmp(msg,"fim",3));

	printf("Servidor %s terminando...\n",server_name); fflush(stdout);

	return 0;
}

