/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : tratamento de sinais (signals).
** Comando: signal()
** Objetivo: ignorar o recebimento de alguns sinais 
*/

#include <stdio.h>
#include <signal.h>
#include <unistd.h>


int 
main()
{
/*
	//Tenta ignorar todos os sinais 
	int nsig;
	for(nsig=1;nsig<=32;nsig++)
		if(signal(nsig,SIG_IGN)==SIG_ERR)
			printf("Erro ignorando sinal %d\n",nsig);
*/

  /* ignora os sinais SIGHUP(1), SINGINT(2), SIGQUIT(3), SIGTERM(15) e SIGTSTP(20) */

  if(signal(SIGHUP,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGHUP");

  if(signal(SIGINT,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGINT");

  if(signal(SIGQUIT,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGQUIT");

  if(signal(SIGTERM,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGTERM");

  if(signal(SIGTSTP,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGTSTP");

  if(signal(SIGCONT,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGCONT");

  // man 7 signal: "The signals SIGKILL and SIGSTOP cannot be caught or ignored."
  if(signal(SIGKILL,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGKILL");

  if(signal(SIGSTOP,SIG_IGN)==SIG_ERR)
    perror("Erro ignorando SIGSTOP");

  for( ; ; ) {
    sleep(1);
    printf(".");
    fflush(stdout);
  }
  
  return(0);
}

