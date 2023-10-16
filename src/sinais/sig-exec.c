/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : tratamento de sinais
** Comando: sigaction(), kill()
** Objetivo: Tratamento de sinais. Observar que sinal diferente do esperado
             pelo filho, faz com que ele seja encerrado, ou parado (ação default).
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>




void 
sig_handler(int signo)
{
  printf("Signal %d\n",signo); fflush(stdout);
}


int 
main(int argc, char **argv)
{
	int signum;
	pid_t pid;
	struct sigaction act;

	if(argc<2) {
		printf("Uso: %s SIGNUM\n",argv[0]);
		exit(0);
	}
	signum=atoi(argv[1]); // falta verificar se sinal é válido
 
	// Ideia: instalar rotina de tratamento no pai para ver se filho 
	// herda o tratamento
	act.sa_handler = sig_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = SA_RESTART;
	if(sigaction(signum, &act, NULL) == -1) {
		perror("Erro na captura do signal");
      exit(0);
    }

	if((pid=fork())<0) {
		printf("Erro na execucao do fork\n");
		exit(0);
	}

	if(pid==0) { // filho 
		// sobrepoe o processo atual com um novo processo 
		execlp("./sig-show","sig-show",NULL);
		// processo nao continua depois do exec!
		perror("Filho: erro na execucao do execl");

	}else { // pai 
		do {
			printf("Num: ");
			scanf("%d",&signum);
			kill(pid,signum);
		}while(signum!=-1);

		// forca fim do filho, caso nao tenha sido terminado 
		kill(pid,SIGKILL);
	}

	return(0);
}
