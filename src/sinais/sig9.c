/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: 
** Objetivo: Verificar que o término do processo pai não gera necessariamente
             o envio do sinal SIGHUP para o filho. Isso ocorre apenas se o processo
				 controlador do grupo (normalmente, o shell) termina.
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// typedef void (*sighandler_t)(int);
// sighandler_t signal(int signum, sighandler_t handler);


int _fim=0;


void
hup_handler(int signum)
{
	pid_t pid=getppid();
	printf("SIGHUP: pai = %d\n",pid); fflush(stdout);
	_fim=1;
} 


int
main()
{
	pid_t pid;

	if((pid=fork())==-1) {
		perror("Erro no fork");
		exit(0);
	}
	// pai dorme 10s e termina
	if(pid) {
		sleep(10);
		if(raise(9)==-1)	// suicídio!
			perror("Falha no raise");
		exit(0);
	}

	// filho: trata SIGHUP: enviado quando terminal ou processo controlador termina 
	signal(SIGHUP, hup_handler);	

	// Para testar: executar redirecionando saida para arquivo. Terminar shell
	do { 
		sleep(1);
		printf("."); fflush(stdout);
	} while (!_fim);

	printf("Filho terminando...\n"); fflush(stdout);

	return(0);
}

