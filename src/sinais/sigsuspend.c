/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : 
** Comando: 
** Objetivo: Bloquear processo até recebimento de um sinal, que deve estar
             capturado para evitar que ação default seja executada.
				 Bloqueio evita "busy wait" à espera do sinal.
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
	sigset_t sinais,old_mask;

	// instala rotina de tratamento do sinal SIGINT
	if(signal(SIGINT, int_hand)==SIG_ERR) {
		perror("Erro capturando tratamento do sinal");
		return(0);
	}

/*
	int sigsuspend(const sigset_t *mask);

	sigsuspend()  temporarily  replaces  the  signal  mask  of  the calling
	process with the mask given by mask and then suspends the process until
	delivery  of  a signal whose action is to invoke a signal handler or to
	terminate a process.

	If the signal  terminates  the  process,  then  sigsuspend()  does  not
	return.   If  the signal is caught, then sigsuspend() returns after the
	signal handler returns, and the signal mask is restored  to  the  state
	before the call to sigsuspend().

	It  is  not possible to block SIGKILL or SIGSTOP; specifying these sig-
	nals in mask, has no effect on the process's signal mask.

	sigsuspend() always returns -1, normally with the error EINTR.

	Normally,  sigsuspend()  is  used  in conjunction with sigprocmask() in
	order to prevent delivery of a signal during the execution of a  criti-
	cal  code  section.   The caller first blocks the signals with sigproc-
	mask().  When the critical code has completed, the  caller  then  waits
	for  the  signals by calling sigsuspend() with the signal mask that was
	returned by sigprocmask() (in the oldset argument).
*/

	// Questao: codigo abaixo deve ser executado sem sujeitar-se a INT

	// int sigemptyset(sigset_t *set);
	if(sigemptyset(&sinais)==-1) {
		perror("Erro em sigemptyset"); exit(0);
	}
	// int sigaddset(sigset_t *set, int signum);
	if(sigaddset(&sinais,SIGINT)==-1) {
		perror("Erro em sigaddset"); exit(0);
	}
	// int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
	if(sigprocmask(SIG_BLOCK, &sinais, &old_mask)==-1) {
		perror("Erro em sigprocmask");
		exit(0);
	}

	// dorme para dar tempo de receber um sinal bloqueado (crtl <c>)
	sleep(10);
	
	// int sigsuspend(const sigset_t *mask);
	if(sigsuspend(&old_mask)==-1)
		perror("Erro em sisguspend");

	printf("Fim\n");

	return(0);
}

