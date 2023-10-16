/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : processo local_addr que se comunica atraves de socket com UDP
** Comandos: socket(),bind(),gethostname(),gethostbyname(),sendto(),recvfrom()
** Objetivo: ilustrar a comunicacao local_addrXserv_addr usando socket.
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
	int sockfd, n_bytes, tam, porta;
	struct sockaddr_in serv_addr, local_addr;
	struct hostent *phe;
	char *serv_name; 
	char host_name[LEN], msg[TAM_MSG], rsp[TAM_MSG];

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

	// ajusta endereco do local_addr local 
	memset((char *)&local_addr,0,sizeof(local_addr));
	local_addr.sin_family = PF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY); // escuta em todos ends locais
	local_addr.sin_port = htons(0);                 // port atribuido pelo SO 

/*
	// bind é recomendado mas código funciona sem. SO escolhe porta
	// registra porta, habilitando recebimento de dados 
	if( bind(sockfd, (struct sockaddr *)&local_addr, sizeof(local_addr)) < 0) {
		perror("erro 'binding' socket do local_addr");
		exit(0);
	}
*/
	// ajusta endereco do servidor remoto
	memset((char *)&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family	= PF_INET;
	serv_addr.sin_port   = htons(porta); // endereco da porta serv_addr 

	// obtem endereco ip do serv_addr a partir do nome de dominio 
	if((phe = gethostbyname(serv_name))==NULL) {
		perror("erro na consulta do serv_addr");
		exit(0);
	}
	// copia endereco para a estrutura 
	memcpy((char *)&serv_addr.sin_addr,phe->h_addr,phe->h_length);

	do {
		printf("Mensagem: "); fflush(stdout);
		fgets(msg,TAM_MSG,stdin);
		tam=strlen(msg);

	/*
	MSG_DONTWAIT
	Enables  non-blocking  operation;  if  the  operation would block, EAGAIN is
	returned (this can also be enabled using O_NONBLOCK with the F_SETFL fcntl(2)).
	*/
		// envia msg para o serv_addr 
		// if((n_bytes=sendto(sockfd,msg,tam,0,
		if((n_bytes=sendto(sockfd,msg,tam,MSG_DONTWAIT,
                       (struct sockaddr *)&serv_addr,sizeof(serv_addr))) != tam) {
			fprintf(stderr,"Erro em sendto: enviou %d bytes de %d!\n",n_bytes,tam);
			exit(0);
		}
		// envia msg para o serv_addr 
		if((n_bytes=sendto(sockfd,"fim",3,MSG_DONTWAIT,
                       (struct sockaddr *)&serv_addr,sizeof(serv_addr))) != tam) {
			fprintf(stderr,"Erro em sendto: enviou %d bytes de %d!\n",n_bytes,tam);
			exit(0);
		}

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

