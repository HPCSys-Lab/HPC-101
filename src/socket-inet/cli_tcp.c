/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : processo cliente que se comunica atraves de socket com tcp
** Comandos: socket(), connect(), gethostname(), gethostbyname(), read(),write()
** Objetivo: ilustrar a comunicacao cliente X serv_addr usando TCP.
             Aprender a determinacao do endereco de um serv_addr atraves do nome,
             consultando o serv_addr DNS.
*/


#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>


#define LEN			128
#define S_PORT		8000
#define TAM_MSG	65537
#define MIN_PORT	1024
#define MAX_PORT	32767


int
main(int argc, char **argv)
{
	struct sockaddr_in serv_addr;
	struct hostent *phe;
	char *nome_serv;
	char nome_cliente[LEN];
	int sockfd, n_bytes, porta;
	char msg[TAM_MSG];
	char **apelidos;
	int i;
	int tam,win_size;

	if(argc<3) {
		printf("Uso: %s serv_addr num_porta\n",argv[0]);
		exit(0);
	}
	nome_serv = argv[1];
	porta = atoi(argv[2]);

	if (porta<=MIN_PORT || porta>MAX_PORT) {
		printf("Porta deve estar entre %d e %d\n",MIN_PORT,MAX_PORT);
		exit(0);
	}

	// cria socket TCP 
	if( (sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Erro na criacao do socket");
		exit(0);
	}
	// determina nome do serv_addr local
	if(gethostname(nome_cliente,128)<0) {
		perror("Erro em gethostname");
		exit(0);
	}

	// ajusta endereco do servidor
	memset((char *)&serv_addr,0,sizeof(serv_addr));  // preenche estrutura com 0's
	serv_addr.sin_family = PF_INET;         // ajusta familia protocolo
	serv_addr.sin_port   = htons(porta);    // porta de servico

	// obtem endereco ip do serv_addr a partir do nome logico
	if((phe = gethostbyname(nome_serv))==NULL) {
		perror("Erro em gethostbyname");
		exit(0);
	}
	// desnecessario: apenas para ilustrar outras informaçoes obtidas do DNS
	apelidos = phe->h_aliases;
	for(i=0;apelidos[i]!=0;i++)
		if(apelidos[i]) 
			printf("Alias: %s\n",(char *)apelidos[i]);
	
	// copia endereco para a estrutura 
	memcpy((char *)&serv_addr.sin_addr, phe->h_addr, phe->h_length);

	// estabelece conexão com servidor
	if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Erro conectando com serv_addr");
		exit(0);
	}

	tam=sizeof(win_size);
	getsockopt(sockfd,SOL_SOCKET,SO_SNDBUF, &win_size, (socklen_t *)&tam);
	printf("SNDBUF: %d\n", win_size);

	do {
		printf("Mensagem para servidor: "); fflush(stdout);
		fgets(msg,TAM_MSG,stdin);
		// envia msg para servidor 
		if((n_bytes=write(sockfd,msg,strlen(msg)))<0) {
			perror("erro escrevendo no socket");
			close(sockfd);
			exit(0);
		}
		// espera dados do serv_addr
		if((n_bytes=read(sockfd, msg, TAM_MSG))<0) {
			perror("erro recebendo dados");
		}else {
			msg[n_bytes]='\0';
			printf("Cliente recebeu (%d): %s\n",n_bytes,msg);
		}

	} while (strncmp(msg,"fim",3) && strncmp(msg,"quit",4));

	close(sockfd);

	return 0;
}
