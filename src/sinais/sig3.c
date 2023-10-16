/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : tratamento de sinais (signals).
** Comando: sigaction()
** Objetivo: instalar rotina de tratamento e verificar o comportamento do 
             programa. Testar SIGINT e SIGTSTOP e matar o programa via console
             (kill -SIG... pid).
             20 SIGTSTOP: inibe ctrl Z 
             2  SIGINT: inibe ctrl C 
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


struct sigaction _act, _oact;


void 
sig_handler(int signo)
{
  printf("Signal %d\n",signo);
}


int 
main(int argc, char **argv)
{
  int signum;

  if(argc<2) {
    printf("Uso: %s SIGNUM\n",argv[0]);
    exit(0);
  }
  signum=atoi(argv[1]);
  
  _act.sa_handler = sig_handler;
  sigemptyset(&_act.sa_mask);
  _act.sa_flags = SA_RESTART;
  // if(sigaction(signum, &_act, &_oact) == -1)
  if(sigaction(signum, &_act, 0) == -1) {
    perror("Erro na captura do signal");
	 exit(0);
  }

  /* espera ociosa: usar kill -INTNUM do console para testar rotina */
  for( ; ; ) {
    sleep(1);
    printf(".");
    fflush(stdout);
  }

  return(0);
}

