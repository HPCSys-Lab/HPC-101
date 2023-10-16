/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa 9: tratamento de sinais (signals).
** Comando: sigaction()
** Objetivo: instalar rotina de tratamento e verificar o comportamento do 
             programa. Rotina e' executada duas vezes, depois restaura
             rotina original do sistema.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


struct sigaction _act, _oact;
int _num=0;


void 
int_hand(int signo)
{
  printf("%d %d\n",signo,_num++);

  /* atua 2 vezes depois restaura a rotina original: ação default */
  if(_num>2) {
    sigaction(SIGINT, &_oact, NULL);
    /* signal(SIGINT,SIG_DFL); */
    /* exit(0); */
  }
}

int 
main(int argc, char **argv)
{
  /* instala rotina de tratamento do sinal de interrupcao */
  _act.sa_handler = int_hand;
  sigemptyset(&_act.sa_mask);  // "zera" máscara de sinais que serão 
                               // bloqueados durante a execução do int_hand
  _act.sa_flags = SA_RESTART;

  // if(sigaction(SIGINT, &_act, 0 ) == -1)
  if(sigaction(SIGINT, &_act, &_oact) == -1)
    /* printf("Erro na captura do signal\n"); */
    perror("Erro na captura do signal");

  /* espera fim do programa */
  for( ; ; ) {
    sleep(1);
    printf("."); fflush(stdout);
  }

  return(0);
}


