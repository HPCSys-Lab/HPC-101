/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : tratamento de sinais
** Comando: sigaction(), alarm()
** Objetivo: Tratamento de sinais, uso da chamada alarm()
**           Ajuste de evento (sinal) 
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>


int _fim;


void 
sig_handler(int signo)
{
  printf("(%d) Hora de acabar!\n",signo); fflush(stdout);
  _fim=1;
}


int 
main(int argc, char **argv)
{
  int result,espera;

  if(argc<2) {
    printf("Uso: %s tempo_de_espera (0<t<=100)\n",argv[0]);
    exit(0);
  }
  
  espera=atoi(argv[1]);
  if(espera<1 || espera>100) {
    printf("Uso: %s tempo_de_espera (0<t<=100)\n",argv[0]);
    exit(0);
  }

  if((signal(SIGALRM,sig_handler))==SIG_ERR) {
    perror("Erro tratanto SIGALRM");
    exit (0);
  }

/* 
   DESCRIPTION
    alarm arranges for a SIGALRM signal to be delivered to the
    process in seconds seconds.
*/
  result=alarm(espera);
 
  _fim=0;
  do {
    sleep(1); 
    printf("."); fflush(stdout);
  }while(!_fim);

  printf("Acabou\n");

  return(0);
}



