/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : tratamento de sinais (signals).
** Comando: 
** Objetivo: bloquear o recebimento de sinais
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

/*
	int sigemptyset(sigset_t *set);
	int sigfillset(sigset_t *set);
	int sigaddset(sigset_t *set, int signum);
	int sigdelset(sigset_t *set, int signum);
	int sigismember(const sigset_t *set, int signum);

	sigemptyset() initializes the signal set given by set to empty, with
		all signals excluded from the set.
	sigfillset() initializes set to full, including all signals.
	sigaddset() and sigdelset() add and delete respectively  signal  signum from set.
	sigismember() tests whether signum is a member of set.
*/

/*
	int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);

	sigprocmask() is used to change the signal mask, the set of currently
	blocked signals. The behaviour of the call is dependent on the value of how, 
	as follows.

	SIG_BLOCK: The set of blocked signals is the union of the current
		set and the set argument.
	SIG_UNBLOCK: The signals in set are removed from the  current  set  of
		blocked signals. It is legal to attempt to unblock a signal which is not blocked.
	SIG_SETMASK: The set of blocked signals is set to the argument set.

	If oldset is non-null, the previous value of the signal mask is stored in oldset.
	If  set  is  NULL,  then  the  signal  mask  is unchanged (i.e., how is
	ignored), but the current value of  the  signal  mask  is  nevertheless
	returned in oldset (it it is not NULL).

	The use of sigprocmask() is unspecified in a multithreaded process; see
	pthread_sigmask(3).
*/


int _num=0;

void
hand(int signo)
{
	printf("Num: %d\n",_num++);	
}


int 
main(int argc, char **argv)
{
	int i,signum;
	sigset_t sig_mask;

	if(argc<2) {
		printf("Uso: %s SIGNUM (0 < signum < 32)\n",argv[0]);
		exit(0);
	}
	signum=atoi(argv[1]);
	if(signum<=0 || signum>31) {
		printf("Uso: %s SIGNUM (0 < signum < 32)\n",argv[0]);
		exit(0);
	}

	if(signal(signum, hand)==SIG_ERR) {
		perror("Erro capturando tratamento do sinal");
		return(0);
	}

	// zera máscara de sinais
	// int sigemptyset(sigset_t *set);
	if(sigemptyset(&sig_mask)==-1) {
		perror("Erro em sigemptyset");
		exit(0);
	}
	// adiciona sinal desejado na máscara
	// int sigaddset(sigset_t *set, int signum);
	if(sigaddset(&sig_mask,signum)==-1) {
		perror("Erro em sigaddset");
		exit(0);
	}

	// bloqueia recebimento dos sinais mascarados
	// int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
	if(sigprocmask(SIG_BLOCK, &sig_mask, 0)==-1) {
		perror("Erro em sigprocmask");
		exit(0);
	}

	/* espera ociosa: usar kill -INTNUM do console para testar rotina */
	for(i=0;i<10;i++) {
		sleep(1);
		printf("."); fflush(stdout);
	}

	// int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
	if(sigprocmask(SIG_UNBLOCK, &sig_mask, 0)==-1) {
		perror("Erro em sigprocmask");
		exit(0);
	}

	// entra em loop; <ctrl>C não termina, mas faz com que rotina de tratamento seja executada
	for(;;);

	return(0);
}


