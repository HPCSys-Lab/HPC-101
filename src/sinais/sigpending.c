/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : tratamento de sinais (signals).
** Comando: sigpending()
** Objetivo: usar o mascaramento do recebimento de sinais (sigprocmask)
**           usar sigpending para pesquisar sinais pendentes
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


int
main()
{
	sigset_t sig_mask;
	int i;
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
*/

	// zera a máscara de sinais
	// int sigemptyset(sigset_t *set);
	if(sigemptyset(&sig_mask)==-1) {
		perror("Erro em sigemptyset");
		exit(0);
	}

	// adiciona sinais que se quer bloquear na máscara
	// int sigaddset(sigset_t *set, int signum);
	if(sigaddset(&sig_mask,SIGINT)==-1) {
		perror("Erro em sigaddset"); exit(0);
	}
	if(sigaddset(&sig_mask,SIGTSTP)==-1) {
		perror("Erro em sigaddset"); exit(0);
	}
	if(sigaddset(&sig_mask,SIGQUIT)==-1) {
		perror("Erro em sigaddset"); exit(0);
	}
	if(sigaddset(&sig_mask,SIGTERM)==-1) {
		perror("Erro em sigaddset"); exit(0);
	}

	// ativa máscara de sinais com os sinais cujo recebimento será bloqueado (pendente)
	// int sigprocmask(int how, const sigset_t *set, sigset_t *oldset);
	if(sigprocmask(SIG_BLOCK, &sig_mask, 0)==-1) {
		perror("Erro em sigprocmask");
		exit(0);
	}

/*
	int sigpending(sigset_t *set);

	sigpending()  returns the set of signals that are pending for delivery to the
	calling thread (i.e., the signals which have been raised while blocked).  The
	mask of pending signals is returned in set.
*/

	/* espera ociosa: usar kill -INTNUM do console para testar rotina */
	for( ; ; ) {
		sleep(2);

		// verifica se há sinais pendentes

		// int sigpending(sigset_t *set);
		if(sigpending(&sig_mask)==-1) {
			perror("Erro em sigpending"); exit(0);
		}
		// imprime lista de sinais pendentes
		for (i=1;i<32;i++) {
			// int sigismember(const sigset_t *set, int signum);
			if(sigismember(&sig_mask,i))
				printf("Sinal %d pendente.\n",i);
		}	
	}

	return(0);
}



