/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 3: criacao de processos
** Comandos: fork() waitpid()
** Objetivo: pai cria N processos e espera cada um individualmente. Variacoes
             na duracao do sleep podem causar zombies.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define N_PROC 5

/*
	pid_t wait(int *status);
	pid_t waitpid(pid_t pid, int *status, int options);

	The  wait  function  suspends  execution of the current process until a child has exited, or
	until a signal is delivered whose action is to terminate the current process or  to  call  a
	signal  handling  function.   If  a  child  has  already  exited  by the time of the call (a
	so-called "zombie" process), the function returns immediately.  Any system resources used by
	the child are freed.

	The waitpid function suspends execution of the current process until a child as specified by
	the pid argument has exited, or until a signal is delivered whose action is to terminate the
	current  process  or to call a signal handling function.  If a child as requested by pid has
	already exited by the time of the call (a so-called "zombie" process), the function  returns
	immediately.  Any system resources used by the child are freed.

   The call wait(&status) is equivalent to:

   waitpid(-1, &status, 0);

   The waitpid() system call suspends execution of the current process until a child speci-
   fied by pid argument has changed state.  By default, waitpid() waits only for terminated
   children, but this behaviour is modifiable via the options argument, as described below.

	The value of pid can be:

	< -1  meaning wait for any child process whose process group ID is equal to the absolute value of pid.

	-1    meaning wait for any child process.

	0     meaning wait for any child process whose process group ID is equal to that of the
         calling process.

	> 0   meaning wait for the child whose process ID is equal to the value of pid.

	The value of options is an OR of zero or more of the following constants:

	WNOHANG: return immediately if no child has exited.
	WUNTRACED: also return if a child has stopped (but not traced via ptrace(2)). Status for
        traced children which have stopped is provided even if this option is not specified.
	WCONTINUED: (Since Linux 2.6.10) also return if a stopped child has been resumed by delivery
        of SIGCONT.

	If status is not NULL, wait() and waitpid() store status information in the int to which
	it  points.   This  integer  can  be inspected with the following macros (which take the
	integer itself as an argument, not a pointer to it, as is done in wait() and waitpid()!):

	WIFEXITED(status): returns true if the child terminated normally, that is, by calling exit(3) or
		_exit(2), or by returning from main().
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


int
main(int argc, char **argv)
{
	int i,n_proc,status;
	pid_t vet_pid[N_PROC], pid;

	if(argc>1) {
		n_proc=atoi(argv[1]);
		if((n_proc < 0) || (n_proc > N_PROC)) {
			printf("Num proc deve ser > 0 e < %d. Usando %d\n",N_PROC,N_PROC);
			n_proc=N_PROC;
		}
	}else
		n_proc=N_PROC;

	for(i=n_proc-1;i>=0;i--) {
		printf("Criando proc %d...\n",i); 

		if((vet_pid[i]=fork())==-1) {
			// Deveria encerrar processos jah criados antes de sair...
		  	perror("Erro na execucao do fork");
			printf("Erro na criacao do processo: %d\n",i);
			exit(0);
		}
		/* filho dorme e termina, sem continuar o loop */
		if(vet_pid[i]==0) {
			sleep(n_proc-i); 
			exit(0);
		} 
	}

	/* so o pai prossegue */
	sleep(10); /* dorme um pouco antes de esperar pelos filhos */
	for(i=0;i<n_proc;i++) {
		printf("Esperando proc %d (%d)...",i,(int)vet_pid[i]); 
		pid=waitpid(vet_pid[i],&status,0);
		// espera qualquer filho: pid=-1
		// pid=waitpid(-1,&status,0);
		printf("%d\n",(int)pid);
	}
	printf("Pronto\n");

	return(0);
}

