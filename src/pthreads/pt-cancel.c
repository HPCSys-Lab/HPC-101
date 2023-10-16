/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: 
** Objetivo: 
*/

// Ref: Exemplo Linux / man pthread_cleanup_push

#include <pthread.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>


static int done = 0;
static int cleanup_pop_arg = 0;
static int cnt = 0;

static void
cleanup_handler(void *arg)
{
	printf("Executou clean-up handler\n");
	cnt = 0;
}

static void *
thread_start(void *arg)
{
	time_t start, curr;

	printf("Nova thread iniciada\n");

	// instala rotina de cleanup
	pthread_cleanup_push(cleanup_handler, NULL);

	curr = start = time(NULL);

	while (!done) {
		pthread_testcancel();		// atua como um ponto de cancelamento
		if (curr < time(NULL)) {
			curr = time(NULL);
			printf("cnt = %d\n", cnt);  // Função write é ponto de cancelamento
			cnt++;
		}
	}

	// desinstala a função de cleanup. 
	// Valor de cleanup_pop_arg indica se rotina será executada
	pthread_cleanup_pop(cleanup_pop_arg);

	return NULL;
}

int
main(int argc, char *argv[])
{
	pthread_t thr;
	int s;
	void *res;

	s = pthread_create(&thr, NULL, thread_start, NULL);
	if (s != 0) {
		perror("pthread_create");
		exit(EXIT_FAILURE);
	}

	// permite que thread execute um tempo
	srandom(getpid());
	sleep(random()%10);

	// sem argumentos, cancela thread
	// com 1 argumento, deixa que thread termine por conta própria
	// com 2 argumentos, segundo indica se função de cleanup será executada no pop
	if (argc > 1) {
		if (argc > 2)
			// atribui valor à variável usada para indicar se função de cleanup será executada em sua remoção
			cleanup_pop_arg = atoi(argv[2]);
		done = 1;
	} else {
		printf("Cancelando thread\n");
		s = pthread_cancel(thr);
		if (s != 0) {
			perror("Erro em pthread_cancel");
			exit(EXIT_FAILURE);
		}
	}

	s = pthread_join(thr, &res);
	if (s != 0) {
		perror("Erro em pthread_join");
		exit(EXIT_FAILURE);
	}

	if (res == PTHREAD_CANCELED)
		printf("Thread foi cancelada; cnt = %d\n", cnt);
	else
		printf("Thread terminou normalmente; cnt = %d\n", cnt);

	exit(EXIT_SUCCESS);
}



/*
void pthread_cleanup_push_defer_np (void (*routine)(void *), void *arg);
void pthread_cleanup_pop_restore_np (int execute);

These   functions   are   the   same   as   pthread_cleanup_push(3) and pthread_cleanup_pop(3), except for the differences noted on this page.
Like  pthread_cleanup_push(3), pthread_cleanup_push_defer_np() pushes routine onto the thread's stack of cancellation clean-up handlers.  In  addition,  it also saves the thread's current cancelability type, and sets the cancelability type  to  "deferred"  (see  pthread_setcanceltype(3));  this ensures  that cancellation clean-up will occur even if the thread's cancelability type was "asynchronous" before the the call.
Like  pthread_cleanup_pop(3),  pthread_cleanup_pop_restore_np()  pops  the top-most clean-up handler from the thread's stack of cancellation clean-up handlers.  In addition, it restores the thread's cancelability type to its value at the time of the matching pthread_cleanup_push_defer_np().
 The caller must ensure that calls to these functions are paired within the same function, and at the same lexical nesting level.  Other  restrictions apply, as described in pthread_cleanup_push(3).

 pthread_cleanup_push_defer_np(routine, arg);
 pthread_cleanup_pop_restore_np(execute);

 É equivalente a:

 int oldtype;
 pthread_cleanup_push(routine, arg);
 pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, &oldtype);
 ...
 pthread_setcanceltype(oldtype, NULL);
 pthread_cleanup_pop(execute);
*/


