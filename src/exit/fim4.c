/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : terminando um programa
** Comandos: _exit() e exit()
** Objetivo: verificar que processo que fica "órfão" é "adotado" pelo init (pid=1)
*/

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


// variáveis globais
pid_t _my_pid, _p_pid;


/*
#include <stdlib.h>

void exit(int status);

The  exit() function causes normal program termination and the the value of status & 0377
is returned to the parent (see wait(2)).  All  functions  registered  with  atexit()  and
on_exit() are called in the reverse order of their registration, and all open streams are
flushed and closed.  Files created by tmpfile() are removed.

The C standard specifies two defines EXIT_SUCCESS and EXIT_FAILURE that may be passed  to
exit() to indicate successful or unsuccessful termination, respectively.

After exit(), the exit status must be transmitted to the parent process. There are  three
cases. If the parent has set SA_NOCLDWAIT, or has set the SIGCHLD handler to SIG_IGN, the
status is discarded. If the parent was waiting on the child it is notified  of  the  exit
status.  In  both cases the exiting process dies immediately. If the parent has not indi-
cated that it is not interested in the exit status,  but  is  not  waiting,  the  exiting
process  turns  into  a "zombie" process (which is nothing but a container for the single
byte representing the exit status) so that the parent can learn the exit status  when  it
later calls one of the wait() functions.

If  the implementation supports the SIGCHLD signal, this signal is sent to the parent. If
the parent has set SA_NOCLDWAIT, it is undefined whether a SIGCHLD signal is sent.

If the process is a session leader and its controlling terminal the controlling  terminal
of  the  session,  then  each process in the foreground process group of this controlling
terminal is sent a SIGHUP signal, and the terminal is disassociated  from  this  session,
allowing it to be acquired by a new controlling process.

If  the  exit of the process causes a process group to become orphaned, and if any member
of the newly-orphaned process group is stopped, then a SIGHUP signal followed by  a  SIG-
CONT signal will be sent to each process in this process group.
*/

/*
#include <unistd.h>

void _exit(int status);

The  function  _exit terminates the calling process "immediately". Any open file descrip-
tors belonging to the process are closed; any children of the process  are  inherited  by
process 1, init, and the process's parent is sent a SIGCHLD signal.

The  value status is returned to the parent process as the process's exit status, and can
be collected using one of the wait family of calls.
*/


void 
sig_handler(int signo)
{
	printf("%d, filho de %d, recebeu sinal %d\n",(int)_my_pid,(int)_p_pid,signo); 
	fflush(stdout);
}


int 
main(int argc, char **argv)
{
	int i, status;
	pid_t result;
  
	// cria processo filho
	if((result=fork())==-1) {
		perror("Erro na execucao do fork");
		exit(EXIT_SUCCESS);
	}

	if(result) {	// pai
		
		// instala rotina tratamento SIGCHLD
		if(signal(SIGCHLD, sig_handler)==SIG_ERR) {
			perror("Erro capturando tratamento do sinal");
			exit(EXIT_SUCCESS);
		}
		// obtém PID e PPID
		_my_pid=getpid();
		_p_pid=getppid();

		// Pai espera o fim do processo filho
      /* wait retorna o pid do processo que terminou */
      result=wait(&status);
      printf("%d, filho de %d terminou com status %d\n",(int)result,(int)_my_pid,status);

      printf("%d terminou\n",(int)_my_pid);

		exit(EXIT_SUCCESS);
	}

	if(!result) {	 // filho

		// obtem PID e PPID
		_my_pid=getpid();
		_p_pid=getppid();

		// cria neto
		if((result=fork())==-1) {
			perror("Erro na execucao do fork");
			exit(EXIT_SUCCESS);
		}
		if(!result) { // neto
			
			// obtém PID e PPID
			_my_pid=getpid();
			_p_pid=getppid();

			// instala rotina tratamento SIGHUP
			if(signal(SIGHUP, sig_handler)==SIG_ERR) {
				perror("Erro capturando tratamento do sinal");
				exit(EXIT_SUCCESS);
			}
			printf("\n%d, filho de %d dormindo\n",(int)_my_pid,(int)_p_pid); fflush(stdout);

			// Neto dorme à espera do sinal ou 10 s
			for(i=0;i<10;i++)
				sleep(1);

			// obtém PPID  novamente
			_p_pid=getppid();

			printf("%d, filho de %d terminando\n",(int)_my_pid,(int)_p_pid); fflush(stdout);
			exit(EXIT_SUCCESS);

    	} else { // filho
			printf("%d, filho de %d dormindo\n",(int)_my_pid,(int)_p_pid); fflush(stdout);
			sleep(5);
			_exit(EXIT_SUCCESS);	
		}
	}

	return EXIT_SUCCESS;
}


