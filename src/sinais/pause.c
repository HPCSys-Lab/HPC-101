/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : 
** Comando: pause()
** Objetivo: testar a espera pelo recebimento de um sinal.
**           Para testar, iniciar programa em foreground e digitar <ctrl>C
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void
int_hand(int signo)
{
  printf("Signal: %d\n",signo);
}


int 
main()
{

/*
	int pause(void);

	The pause() library function causes the invoking process (or thread) to
	sleep until a signal is received that either terminates it or causes it
	to call a signal-catching function.

	The pause() function only returns when a signal was caught and the sig-
	nal-catching function returned. In this case pause()  returns  -1,  and
	errno is set to EINTR.
*/

	// if(signal(SIGUSR1, int_hand)==SIG_ERR) {
	if(signal(SIGINT, int_hand)==SIG_ERR) {
		perror("Erro capturando tratamento do sinal");
		return(0);
	}

	// int pause(void);
	if(pause()==-1) {
		perror("Erro em pause");
	}

	printf("Tchau!\n");

	return(0);
}


