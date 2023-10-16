/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: waitpid()
** Objetivo: detectar a condição de saída do processo filho
*/

/* 
 * teste: enviar um sinal para o filho e ver o resultado no wait */
/* kill -SIGTSTP ...
 * kill ...
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);

	The  wait  function  suspends  execution  of  the current process until a child has
	exited, or until a signal is delivered whose action is  to  terminate  the  current
	process  or  to  call a signal handling function.  If a child has already exited by
	the time of the call (a so-called "zombie" process), the function  returns  immedi-
	ately.  Any system resources used by the child are freed.

	The  waitpid  function  suspends  execution of the current process until a child as
	specified by the pid argument has exited, or until  a  signal  is  delivered  whose
	action  is  to terminate the current process or to call a signal handling function.
	If a child as requested by pid has already exited  by  the  time  of  the  call  (a
	so-called   "zombie"  process),  the  function  returns  immediately.   Any  system
	resources used by the child are freed.

	The value of pid can be one of:

	< -1  which means to wait for any child process whose process group ID is equal to
         the absolute value of pid.

	-1    which  means to wait for any child process; this is the same behaviour which
         wait exhibits.

	0     which means to wait for any child process whose process group ID is equal to
         that of the calling process.

	> 0   which  means to wait for the child whose process ID is equal to the value of pid.

	The value of options is an OR of zero or more of the following constants:

	WNOHANG: which means to return immediately if no child has exited.
	WUNTRACED: which means to also return for children which are stopped (but not  traced),
              and  whose  status  has not been reported.  Status for traced children which
              are stopped is provided also without this option.

	(For Linux-only options, see below.)

	If status is not NULL, wait or waitpid store status  information  in  the  location
	pointed to by status.

	This  status can be evaluated with the following macros (these macros take the stat
	buffer (an int) as an argument -- not a pointer to the buffer!):

	WIFEXITED(status): returns true if the child terminated normally, that is, by calling 
		exit() or _exit(), or by returning from main().
	WEXITSTATUS(status): evaluates  to  the  least  significant  eight bits of the return 
		code of the child which terminated, which may have been set as the argument to a 
		call to exit() or _exit() or as the argument for a return statement in the main pro-
		gram.  This macro can only be evaluated if WIFEXITED returned true.
	WIFSIGNALED(status): returns true if the child process terminated because of a signal  
		which  was not caught.
	WTERMSIG(status): returns the number of the signal that caused the child process to 
		terminate. This macro can only be evaluated if WIFSIGNALED returned non-zero.
	WIFSTOPPED(status): returns true if the child process  which  caused  the  return  
		is currently stopped;  this is only possible if the call was done using WUNTRACED 
		or when the child is being traced (see ptrace(2)).
	WSTOPSIG(status): returns the number of the signal which caused the child to stop.  
		This macro can only be evaluated if WIFSTOPPED returned non-zero.

RETURN VALUE
	The  process ID of the child which exited, or zero if WNOHANG was used and no child
	was available, or -1 on error (in which case errno is set to an appropriate value).
*/

#define ESPERA 10


int
main(int argc, char **argv)
{
	int espera,status;
	pid_t pid,result;

	if(argc>1) {
		espera=atoi(argv[1]);
		if(espera<0) {
			printf("Uso: %s tempo_sleep # >= 0\n", argv[0]);
			exit(EXIT_SUCCESS);
		}
	} else 
		espera=ESPERA;

	if((pid=fork())==-1) {
		perror("Erro no fork");
		exit(EXIT_SUCCESS);
	}

	if(!pid) {
		sleep(espera);
		// exit(EXIT_SUCCESS);
		// exit(EXIT_FAILURE);
		// return(1);
		return(0);
	}
	printf("%d waiting for %d\n",(int)getpid(),(int)pid); fflush(stdout);

	// pid_t wait(int *status);
	// result=wait(&status);
	// pid_t waitpid(pid_t pid, int *status, int options);
	result=waitpid(pid, &status, 0);

	printf("waitpid() return: %d\n",result); fflush(stdout);

	if(WIFEXITED(status)) {
		printf("%d terminated normally. Exit status: %d\n",pid,WEXITSTATUS(status));
		return(0);
	}
	if(WIFSIGNALED(status))
		printf("%d terminated because of a signal which was not caught: %d\n",pid,WTERMSIG(status));
	if(WIFSTOPPED(status))
		printf("%d is currently stopped: %d\n",pid,WSTOPSIG(status));
		
	return(0);
}
