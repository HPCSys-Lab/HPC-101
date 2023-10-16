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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>


#define N_THREADS 3
#define TTL			2
#define LEN			128

int pthread_yield (void);

int pthread_mutex_timedlock(pthread_mutex_t *mutex, const struct timespec *abs_timeout);

/*
The pthread_mutex_timedlock() function shall lock the mutex object ref-
erenced by mutex. If the mutex is already locked,  the  calling  thread
shall   block   until   the   mutex   becomes   available   as  in  the
pthread_mutex_lock() function. If the mutex cannot  be  locked  without
waiting for another thread to unlock the mutex, this wait shall be ter-
minated when the specified timeout expires.

The timeout shall expire when the absolute time specified by  abs_time-
out  passes, as measured by the clock on which timeouts are based (that
is, when the value of that clock equals or exceeds abs_timeout), or  if
the  absolute  time specified by abs_timeout has already been passed at
the time of the call.

If the Timers option is supported, the timeout shall be  based  on  the
CLOCK_REALTIME clock; if the Timers option is not supported, the timeout 
shall be based on the system clock as returned by the time() function.

The  resolution  of the timeout shall be the resolution of the clock on
which it is based. The timespec data type is defined  in  the  <time.h>
header.

Under  no  circumstance  shall  the function fail with a timeout if the
mutex can be locked immediately. The validity of the abs_timeout param-
eter need not be checked if the mutex can be locked immediately.

As  a  consequence  of the priority inheritance rules (for mutexes ini-
tialized with the PRIO_INHERIT protocol), if a timed mutex wait is ter-
minated  because  its timeout expires, the priority of the owner of the
mutex shall be adjusted as necessary to  reflect  the  fact  that  this
thread is no longer among the threads waiting for the mutex.

RETURN VALUE
If successful, the pthread_mutex_timedlock() function shall return zero; 
otherwise, an error number shall be returned to indicate the error.

ERRORS
The pthread_mutex_timedlock() function shall fail if:

EINVAL The  mutex  was  created  with the protocol attribute having the
	value PTHREAD_PRIO_PROTECT and the calling thread's priority  is
	higher than the mutex' current priority ceiling.
EINVAL The  process  or  thread would have blocked, and the abs_timeout
	parameter specified a nanoseconds field value less than zero  or
	greater than or equal to 1000 million.
ETIMEDOUT
	The mutex could not be locked before the specified timeout expired.
EINVAL The value specified by mutex does not refer  to  an  initialized
	mutex object.
EAGAIN The  mutex  could  not be acquired because the maximum number of
	recursive locks for mutex has been exceeded.
EDEADLK
	The current thread already owns the mutex.

APPLICATION USAGE
	The pthread_mutex_timedlock() function is part of the Threads and Time-
	outs options and need not be provided on all implementations.
*/


time_t time(time_t *t);
/*
time()  returns  the  time  since  the  Epoch (00:00:00 UTC, January 1,
1970), measured in seconds.
If t is non-NULL, the return value is also stored in the memory pointed to by t.
On success, the value of time in seconds since the Epoch is returned.
On error, ((time_t)-1) is returned, and errno is set appropriately.
*/



// declaracao do mutex deve ser global

pthread_mutex_t _mutex = PTHREAD_MUTEX_INITIALIZER; // criacao e inicializacao estatica


int _result = 0;

/*
struct timespec {
	__time_t tv_sec;    // Seconds
	long int tv_nsec;   // Nanoseconds
};
*/

void *soma(void *th_num)
{
	int i, status;
	char err_msg[LEN];
	struct timespec tspec;

	printf("Thread %d pronta.\n",(int)th_num);
	// sleep(1);

	for (i=0; i<TTL; i++) {

		// int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, 
		// 		const struct timespec *restrict abs_timeout);

		do {
			// determina instante atual + 4 s: timeout
			tspec.tv_sec=time(NULL)+4;
			tspec.tv_nsec=0;

			if((status=pthread_mutex_timedlock(&_mutex, &tspec))) {
				strerror_r(status,err_msg,LEN);
				printf("Thread %d - timeout em mutex_timedlock: %s\n",(int)th_num,err_msg);
			}else {
				// imprimir o valor de time para saber se foi acordada antes de expirar o timeout
			}
		} while(status);

		printf("%d bloqueou mutex.\n",(int)th_num);

		printf("Th: %d+%d\n",_result,(int)th_num); fflush(stdout);
		_result = _result + (int)th_num;

		sleep(5);

		pthread_mutex_unlock(&_mutex);

		printf("%d liberou mutex.\n",(int)th_num);
	}

	printf("Soma (%d)= %d\n",(int)th_num,_result);

	pthread_exit(NULL);
}


int
main (int argc, char *argv[])
{
	pthread_t thread[N_THREADS];
	int t, result;
	char err_msg[LEN];

	// cria threads
	for(t=0; t<N_THREADS; t++) {
		// printf("Criando thread %d\n", t);
		if((result=pthread_create(&thread[t], NULL, soma, (void *)t))) {
			strerror_r(result,err_msg,LEN);
			printf("Erro criando thread: %s\n",err_msg);
			exit(0);
		}
	}
	printf("\n");

	// espera threads concluírem
	for(t=0; t<N_THREADS; t++) {
		if((result=pthread_join(thread[t], NULL))) {
			strerror_r(result,err_msg,LEN);
			printf("Erro em pthread_join: %s\n",err_msg);
			exit(0);
		}
	}

	printf("Soma final: %d\n",_result);

	// libera mutex
	pthread_mutex_destroy (&_mutex);

	pthread_exit(NULL);
}


