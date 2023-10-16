/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: wait()
** Objetivo: usar sinal sigchild para tratar fim do processo filho, 
**           evitando que este vire zombie...
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

// typedef void (*sighandler_t)(int);
// sighandler_t signal(int signum, sighandler_t handler);

// pid_t wait(int *status);
// pid_t waitpid(pid_t pid, int *status, int options);

int _fim=0;

void
sighand(int signum)
{
	pid_t pid;
	int status;

	pid=wait(&status);
	printf("Child %d terminated: %d %d\n",pid,signum,status);

	_fim=1;
} 

int
main()
{
	pid_t pid;

	signal(SIGCHLD, sighand);	
	pid=fork();
	if(!pid) {
		sleep(10);
		printf("Filho terminando...\n"); fflush(stdout);
		exit(0);
	}

	do { } while (!_fim);

	printf("Pai terminando...\n"); fflush(stdout);

	return(0);
}

