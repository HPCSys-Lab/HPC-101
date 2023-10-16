/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : processo bind_addr que se comunica atraves de socket com UDP
** Comandos: socket(),bind(),gethostname(),gethostbyname(),sendto(),recvfrom()
** Objetivo: ilustrar a comunicacao bind_addrXserv_addr usando socket.
             Aprender o uso de primitivas de envio para enderecos variados
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


#define S_PORT		8001 
// #define TAM_MSG	65535
#define TAM_MSG	256000
#define LEN			128

int 
main(int argc, char **argv)
{
	int sockfd, n_bytes, tam, porta, enviados, restantes;
	struct sockaddr_in serv_addr, bind_addr;
	struct hostent *phe;
	char *serv_name; 
	char host_name[LEN], msg[TAM_MSG], rsp[TAM_MSG];
	char *buf;

	if(argc<3) {
		printf("Uso: %s serv_addr Num_Porta\n",argv[0]);
		exit(0);
	}
	serv_name = argv[1];
	porta = atoi(argv[2]);
	if(porta<=1024 || porta>65535)
		porta=S_PORT;

	// determina nome da maquina local
	if(gethostname(host_name,128)<0) {
		perror("erro obtendo nome da maquina local");
		exit(0);
	}

	// criacao do socket UDP 
	if( (sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("erro na abertura do socket");
		exit(0);
	}

	// ajusta endereco do bind_addr local 
	memset((char *)&bind_addr,0,sizeof(bind_addr));
	bind_addr.sin_family = PF_INET;
	bind_addr.sin_addr.s_addr = htonl(INADDR_ANY); // aceita conexao de qquer maq 
	bind_addr.sin_port = htons(0);                 // port atribuido pelo SO 
/*
	// bind é recomendado para cliente UDP, mas código funciona sem. SO escolhe porta
	// registra porta, habilitando recebimento de dados 
	if( bind(sockfd, (struct sockaddr *)&bind_addr, sizeof(bind_addr)) < 0) {
		perror("erro 'binding' socket do bind_addr");
		exit(0);
	}
*/
	// ajusta endereco do serv_addr 
	memset((char *)&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family	= PF_INET;
	serv_addr.sin_port		= htons(porta); // endereco da porta serv_addr 

	// obtem endereco ip do serv_addr a partir do nome de dominio 
	if((phe = gethostbyname(serv_name))==NULL) {
		perror("erro na consulta do serv_addr");
		exit(0);
	}
	// copia endereco para a estrutura 
	memcpy((char *)&serv_addr.sin_addr,phe->h_addr,phe->h_length);

	do {
		// printf("Tamanho da mensagem (0<tam<65535): "); fflush(stdout);
		printf("Tamanho da mensagem (0<tam<256000): "); fflush(stdout);
		fgets(msg,TAM_MSG,stdin);
		tam=atoi(msg);

	/*
		MSG_DONTWAIT
		Enables  non-blocking  operation;  if  the  operation would block, EAGAIN is
		returned (this can also be enabled using the O_NONBLOCK with the F_SETFL fcntl(2)).
	*/
		// envia pedido para o serv_addr 
		enviados=0;
		restantes=tam;
		buf=msg;
		do {
			if((enviados=sendto(sockfd,buf,restantes,MSG_DONTWAIT,
                       	(struct sockaddr *)&serv_addr,sizeof(serv_addr))) == -1) {
				perror("Erro enviando dados com sendto");
				exit(0);
			}
			printf("Cliente enviou %d bytes de %d!\n",enviados,restantes);
			buf+=enviados;
			restantes-=enviados;
		} while(restantes>0);

		// espera resposta do serv_addr
		tam=sizeof(struct sockaddr);
		if((n_bytes=recvfrom(sockfd,rsp,TAM_MSG,0,
                         (struct sockaddr *)&serv_addr,(socklen_t*)&tam))<0){
			perror("erro em recvfrom");
		}else{
			rsp[n_bytes]='\0';
			printf("Cliente recebeu: %s\n",rsp); fflush(stdout);
		}

	} while (strncmp(msg,"fim",3));

	close(sockfd);

	return 0;
}


