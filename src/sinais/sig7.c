/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: 
** Objetivo: Tratar o sinal SIGHUP
             Observar que, ao ficar "órfão", processo é "adotado" pelo init (pid = 1)
*/
/* Executar: "sig8 > saída" e fechar a janela do terminal. Abrir nova janela e examinar
   o arquivo saída */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// typedef void (*sighandler_t)(int);
// sighandler_t signal(int signum, sighandler_t handler);



void
hup_handler(int signum)
{
	pid_t ppid=getppid();
	printf("SIGHUP: pai = %d\n",ppid); fflush(stdout);
} 


int
main()
{
	int i;

	// Trata SIGHUP: enviado quando terminal ou processo controlador termina 
	signal(SIGHUP, hup_handler);	

	for (i=0;i<20;i++) { 
		sleep(1);
		printf("."); fflush(stdout);
	}
	printf("Processo terminando...\n"); fflush(stdout);

	return(0);
}

