/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : processo cliente que se comunica atraves de socket com UDP
** Comandos: socket(),bind(),gethostname(),gethostbyname(),sendto(),recvfrom()
** Objetivo: ilustrar a comunicacao clienteXservidor usando socket.
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
#define TAM_MSG	128
#define LEN			128


int 
main(int argc, char **argv)
{
	int sockfd, n_bytes, tam, porta, op;
	struct sockaddr_in bcast_addr, serv_addr;
	char host_name[LEN], msg[TAM_MSG], rsp[TAM_MSG];

	if(argc<2) {
		printf("Uso: %s num_porta\n",argv[0]);
		exit(0);
	}
	porta = atoi(argv[1]);
	if(porta<=1024 || porta>65535)
		porta=S_PORT;

	// determina nome da maquina local
	if(gethostname(host_name,128)<0) {
		perror("erro obtendo nome da maquina local");
		exit(0);
	}

	// criacao do socket UDP para transmissão em broadcast
	if( (sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("erro na abertura do socket");
		exit(0);
	}

	// ajuste do envio com broadcast
	op=1;
	if(setsockopt(sockfd,SOL_SOCKET,SO_BROADCAST,(char *)&op,sizeof(op))==-1) {
		perror("erro ajustando socket para broadcast"); 
	}
	// ajusta informações do socket para envio
	memset((char *)&bcast_addr,0, sizeof(bcast_addr));
	bcast_addr.sin_family      =PF_INET;
	bcast_addr.sin_addr.s_addr =htonl(INADDR_BROADCAST); // 255.255.255.255
	// bcast_addr.sin_addr.s_addr =htonl(0xc81261ff);	// 200.18.97.255 
	bcast_addr.sin_port        =htons(porta);

/*
	// registra porta, habilitando recebimento de dados 
	if( bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
		perror("erro 'binding' socket do cliente");
		exit(0);
	}
*/

	do {
		printf("Mensagem: "); fflush(stdout);
		fgets(msg,TAM_MSG,stdin);

		// envia mensagem para servidores
		tam=strlen(msg);
		if((n_bytes=sendto(sockfd,msg,tam,0,
                       (struct sockaddr *)&bcast_addr,sizeof(bcast_addr))) != tam) {
			fprintf(stderr,"Erro em sendto: enviou %d bytes de %d!\n",n_bytes,tam);
			exit(0);
		}

		// espera resposta dos servidores: como saber quantos?
		// usar gethostbyaddr a partir do serv_addr para saber de quem recebeu...
		tam=sizeof(struct sockaddr);
		do {
			if((n_bytes=recvfrom(sockfd,rsp,TAM_MSG,MSG_DONTWAIT,
                         	(struct sockaddr *)&serv_addr,(socklen_t*)&tam))>=0) {
				rsp[n_bytes]='\0'; printf("Cliente recebeu: %s\n",rsp); fflush(stdout);
			}
		} while(n_bytes>0);

	} while (strncmp(msg,"fim",3));

	close(sockfd);

	return 0;
}


