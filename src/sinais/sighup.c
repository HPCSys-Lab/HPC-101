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
				 controlador do grupo (normalmente, o shell ou o programa da janela) termina.

				 Para funcionar, seria preciso ajustar o processo atual como líder do processgroup
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


/* 
man 2 setsid

A  child created  via fork(2) inherits its parent¿s session ID.  
The session ID is preserved across an execve(2).
A process group leader is a process with process group ID equal to its PID. 
In order to be sure that setsid() will succeed, fork() and exit(), and 
have the child do setsid().
*/									 

int
main()
{
	pid_t pid, lider;

	if((pid=fork()==-1)) {
		perror("Falha no fork");
		exit(0);
	}
	// pai termina para criar novo session leader
	if(pid)
		exit(0);

	// só filho prossegue

	// pid_t setsid(void);
	lider=setsid();
	if(lider==-1) {
		perror("Falha no setsid");
		exit(0);
	}
 
	// ajuste do process group do pai para ser líder de seu process group
	if(setpgid(0, getpid())==-1) {
		perror("Falha no setpgid do pai");
		exit(0);
	}

	if((pid=fork())==-1) {
		perror("Erro no fork");
		exit(0);
	}
	// tentativa de ajuste do process group do filho para pertencer ao 
	// process group do pai; portanto, deveria receber SIGHUP no término 
	// do processo pai
	if(!pid){
		// int setpgid(pid_t pid, pid_t pgid);
		if(setpgid(0, getppid())==-1) {
			perror("Falha no setpgid");
			exit(0);
		}
		// filho: trata SIGHUP: enviado quando terminal ou processo 
		// controlador termina 
		signal(SIGHUP, hup_handler);	
		do { 
			sleep(1);
			printf("."); fflush(stdout);
		} while (!_fim);

		printf("Filho terminando...\n"); fflush(stdout);
	} else { // pai
		sleep(5);
		exit(0);
	}

	return(0);
}

