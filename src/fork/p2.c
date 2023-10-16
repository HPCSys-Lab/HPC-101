/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 2: criacao de processos
** Comandos: fork(), exit(), sleep(), wait()
** Objetivo: verificar o efeito da espera pelo termino de um processo criado.
             Usando sleep() no filho, pai fica parado esperando; no pai, filho
             termina e fica "zombie" ate' que pai espere (wait) por ele.
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int
main()
{
	pid_t result, pid_filho;
	int status;

	result=fork();

	if(result==-1) {
		printf("Erro na execucao do fork\n"); 
		exit(0);
	}

	if(result==0) { /* filho */
		printf("Filho: %d\n",(int)getpid()); fflush(stdout);
		// sleep(5);
		exit(0);
	}else { /* pai */
		sleep(30); 
		printf("Pai esperando %d\n",(int)result); fflush(stdout);
		/* wait retorna o pid do processo que terminou */
		pid_filho=wait(&status);

/*
WIFEXITED(status): returns true if the child terminated normally, that is, by calling 
	exit(3) or _exit(2), or by returning from main().
WEXITSTATUS(status): returns the exit status of the child. This consists of the least
  	significant 8 bits  of  the  status  argument  that  the child specified in a call
  	to exit() or _exit() or as the argument for a return statement in main(). This macro
	should only be employed if WIFEXITED returned true.
WIFSIGNALED(status): returns true if the child process was terminated by a signal.
WTERMSIG(status): returns the number of the signal that caused the child process to
	terminate. This macro should only be employed if WIFSIGNALED returned true.
WCOREDUMP(status): returns true if the child produced a core dump. This macro should
	only be employed if WIFSIGNALED returned true. This macro is not specified in
	POSIX.1-2001 and is not  available  on  some  Unix  implementations  (e.g.,  AIX,
	SunOS).  Only use this enclosed in #ifdef WCOREDUMP ... #endif.
WIFSTOPPED(status): returns true if the child process was stopped by delivery of a
	signal; this is only possible if the call was done using WUNTRACED or when the
	child is being traced (see ptrace(2)).
*/
		if(WIFEXITED(status))
			printf("%d terminou com status %d\n",(int)pid_filho,WEXITSTATUS(status));
		else
			printf("Filho não erminou normalmente\n");
	}

	printf("Fim\n"); fflush(stdout);

	return(0);
}



