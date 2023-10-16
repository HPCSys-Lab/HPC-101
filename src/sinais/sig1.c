/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa 8: tratamento de sinais (signals).
** Comando: signal()
** Objetivo: instalar rotina de tratamento e verificar o comportamento do 
             programa. Observar que a rotina e' executada apenas uma vez,
             retornando automaticamente `a rotina default.
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
	/* typedef void (*sighandler_t)(int); */
	/* sighandler_t signal(int signum, sighandler_t handler); */

	if(signal(SIGTERM, int_hand)==SIG_ERR) {
		perror("Erro capturando tratamento do sinal");
		return(0);
	}
	if(signal(SIGINT, int_hand)==SIG_ERR) {
		perror("Erro capturando tratamento do sinal");
		return(0);
	}

/*
	int raise(int sig);
	The  raise()  function  sends  a  signal to the current process.  
	It is equivalent to:  kill(getpid(), sig);
*/

	// void srand(unsigned int seed);
	srand((int)getpid());

	for( ; ; ) {
		sleep(1);
		printf("."); fflush(stdout);

		// int rand(void);
		// The  rand()  function  returns  a  pseudo-random  integer between 0 and RAND_MAX.
		if(rand()%2) {
			if(rand()%2) {
				if(raise	(SIGTERM)==-1) {
					perror("Erro em raise"); exit(0);
				}
			} else {
				if(raise	(SIGINT)==-1) {
					perror("Erro em raise"); exit(0);
				} 
			}
		}
	}

	return(0);
}


