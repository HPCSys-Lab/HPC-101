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


struct sigaction _act, _oact;


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

  if(argc<2) {
    printf("Uso: %s SIGNUM\n",argv[0]);
    exit(0);
  }
  signum=atoi(argv[1]); // falta verificar se sinal é válido
 
  // Ideia: instalar rotina de tratamento no pai para ver se filho 
  // herda o tratamento
	_act.sa_handler = sig_handler;
	sigemptyset(&_act.sa_mask);
	_act.sa_flags = SA_RESTART;
	if(sigaction(signum, &_act, NULL) == -1) {
		perror("Erro na captura do signal");
      exit(0);
    }

  if((pid=fork())<0) {
    printf("Erro na execucao do fork\n");
    exit(0);
  }

  if(pid==0) { // filho 
 /*
    _act.sa_handler = sig_handler;
    sigemptyset(&_act.sa_mask);
    _act.sa_flags = SA_RESTART;

    if(sigaction(signum, &_act, &_oact) == -1) {
      perror("Erro na captura do signal");
      exit(0);
    }
 */
    while(1); // espera infinita

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
