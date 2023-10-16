/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/
/*
** Programa : tratamento de sinais (signals).
** Comando: 
** Objetivo: instalar rotina de tratamento e verificar o comportamento
**           Testar com <ctrl>c e <ctrl>z !!!
**           Para ver as threads: ps -L, -T, -m, m, H
**           Para ver tratamento de sinais nas threads: ps Hs
**           Observar que não há diferença entre instalar o tratamento antes
**           ou depois da criação das threads, na main thread ou em qualquer outra
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <errno.h>


#define TTL 10
#define N_THREADS 5
#define LEN 128



// int pthread_kill(pthread_t thread, int sig);

// The pthread_kill() function shall request that a signal be delivered to the
// specified thread.
// As in kill(), if sig is zero, error checking shall be performed but no sig-
// nal shall actually be sent.
// Upon  successful  completion,  the  function  shall return a value of zero.
// Otherwise, the function shall return an error number. If the pthread_kill()
// function fails, no signal shall be sent.

// The pthread_kill() function shall fail if:
// ESRCH: no thread could be found corresponding to the specified thread ID.
// EINVAL: The value of the sig argument is an invalid or unsupported sig number.
//
// The pthread_kill() function shall not return an error code of [EINTR].
// The pthread_kill() function provides a mechanism for asynchronously direct-
// ing a signal at a thread in the calling process. This could  be  used,  for
// example, by one thread to affect broadcast delivery of a signal to a set of
// threads.

// Note that pthread_kill() only causes the signal to be handled in  the  con-
// text  of  the  given  thread;  the  signal action (termination or stopping)
// affects the process as a whole.

void 
sig_hand(int signo)
{
	printf("Thread %lu received %d\n",(unsigned long)pthread_self(),signo);
}


void *
funcao(void * arg)
{
	int i;
	char err_msg[LEN];
	struct sigaction ssa;

	printf("Thread %d: self: %lu up and running...\n",(int)arg,(long)pthread_self());

	if(arg==0) {
		printf("%d instalando SIGTSTP\n",(int)arg);

		// Instala rotina de tratamento SIGTSTP
		// Tratamento deve valer para todas
		ssa.sa_handler = sig_hand;
		sigemptyset(&ssa.sa_mask);
		ssa.sa_flags = SA_RESTART;

		if(sigaction(SIGTSTP, &ssa, NULL) == -1) {
			strerror_r(errno,err_msg,LEN);
			printf("Erro na captura do signal: %s\n",err_msg);
		}
	}

	for(i=0;i<TTL;i++) {
		printf("%d ",(int)arg); fflush(stdout);
		sleep(1);
	}
	pthread_exit(NULL);
} 


int 
main(int argc, char **argv)
{
	struct sigaction ssa;
	int i;
	pthread_t th_vec[N_THREADS];
	char err_msg[LEN];

	// Instala rotina de tratamento do sinal SIGINT
	// Tratamento é o mesmo para todas as threads
	ssa.sa_handler = sig_hand;
	sigemptyset(&ssa.sa_mask);
	ssa.sa_flags = SA_RESTART;

	if(sigaction(SIGINT, &ssa, NULL) == -1) {
		strerror_r(errno,err_msg,LEN);
		printf("Erro na captura do signal: %s\n",err_msg);
	}

	for(i=0;i<N_THREADS;i++) {
		if(pthread_create(&th_vec[i], NULL, funcao,(void *)i)) {
			perror("Erro em ptread_create");
			exit(0);
		}
		// printf("Pai criou thread %d\n",i);
	}

	printf("Main thread: %lu\n",pthread_self());

	// Instala rotina de tratamento SIGTERM
	// Mesmo instalando depois da criação dos outros threads, 
	// tratamento vale para todos
	ssa.sa_handler = sig_hand;
	sigemptyset(&ssa.sa_mask);
	ssa.sa_flags = SA_RESTART;

	if(sigaction(SIGTERM, &ssa, NULL) == -1) {
		strerror_r(errno,err_msg,LEN);
		printf("Erro na captura do signal: %s\n",err_msg);
	}

	for(i=0;i<TTL;i++) {

		sleep(1);

		// envia sinal para threads aleatoriamente

		// int pthread_kill(pthread_t thread, int sig);
		if(pthread_kill(th_vec[(int)random()%N_THREADS],SIGINT)==-1) {
			perror("Erro no envio de sinal (pthread_kill)");
		}
		// raise envia sinal para o processo. 
		// Teoricamente, qualquer thread poderia tratá-lo.
		// if(raise(SIGINT)==-1)
			// perror("Erro no envio de sinal (kill)");
	}

	for(i=0;i<N_THREADS;i++) {
		if(pthread_join(th_vec[i], NULL)==-1) {
			perror("erro em pthread_join");
		} else {
			printf("Thread %lu joined\n",th_vec[i]);
		}
	}

	return 0;
}


