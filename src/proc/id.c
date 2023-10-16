/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa: identificacao de processo
** Comando: 
** Objetivo: 
*/

#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

// prototipo da funcao
pid_t getpgid(pid_t pid);

int
main()
{

	printf("\n");
	printf("Process ID: %ld\n",(long)getpid());
	printf("Parent process ID: %ld\n",(long)getppid());
	printf("Owner user ID: %ld\n",(long)getuid());
	printf("Process GID: %ld\n",(long)getgid());

/*
	Process  groups  are used for distribution of signals, and by terminals
	to arbitrate requests for their input: Processes  that  have  the  same
	process group as the terminal are foreground and may read, while others
	will block with a signal if they attempt to read.  These calls are thus
	used  by programs such as csh(1) to create process groups in implement-
	ing job control.   The  TIOCGPGRP  and  TIOCSPGRP  calls  described  in
	termios(3)  are used to get/set the process group of the control terminal.

	If a session has a controlling terminal, CLOCAL is not set and a hangup
	occurs,  then  the  session  leader  is  sent a SIGHUP.  If the session
	leader exits, the SIGHUP signal will be sent to  each  process  in  the
	foreground process group of the controlling terminal.

	If  the  exit of the process causes a process group to become orphaned,
	and if any member of the newly-orphaned process group is stopped,  then
	a  SIGHUP  signal  followed  by  a  SIGCONT signal will be sent to each
	process in the newly-orphaned process group.
*/
/*
	int setpgid(pid_t pid, pid_t pgid);
	int setpgrp(void);
	pid_t getpgid(pid_t pid);
	pid_t getpgrp(void);
*/

	printf("\n");
	printf("Process Group ID (%d): %ld\n",(int)getpid(),(long)getpgid(getpid()));
	printf("Process Group GROUP: %ld\n",(long)getpgrp());
	printf("\n");

	return(0);
}

