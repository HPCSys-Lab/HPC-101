/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: 
** Objetivo: uso de barreiras para sincronização de threads
*/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>


/*
// Functions to handle barriers

// Initialize BARRIER with the attributes in ATTR.  The barrier is
// opened when COUNT waiters arrived.  
int pthread_barrier_init(pthread_barrier_t *barrier,
                         const pthread_barrierattr_t *attr, unsigned count);

// Destroy a previously dynamically initialized barrier BARRIER.  
int pthread_barrier_destroy(pthread_barrier_t *barrier);

// Wait on barrier BARRIER
int pthread_barrier_wait(pthread_barrier_t *barrier);

// Initialize barrier attribute ATTR 
int pthread_barrierattr_init(pthread_barrierattr_t *attr);

// Destroy previously dynamically initialized barrier attribute ATTR. 
int pthread_barrierattr_destroy(pthread_barrierattr_t *attr);

// Get the process-shared flag of the barrier attribute ATTR
int pthread_barrierattr_getpshared(const pthread_barrierattr_t *attr, int *pshared);

// Set the process-shared flag of the barrier attribute ATTR.  
int pthread_barrierattr_setpshared(pthread_barrierattr_t *attr, int pshared);
*/


#define NUM_THREADS	4
#define LEN 128


pthread_barrier_t _barrier;

/*
	int pthread_barrier_wait(pthread_barrier_t *barrier);

	The  pthread_barrier_wait()  function shall synchronize participating threads at the barrier refer-
	enced by barrier.  The calling thread shall block until the required number of threads have  called
	pthread_barrier_wait() specifying the barrier.

	When  the required number of threads have called pthread_barrier_wait() specifying the barrier, the
	constant PTHREAD_BARRIER_SERIAL_THREAD shall be returned to one unspecified thread and  zero  shall
	be  returned  to  each  of  the remaining threads. At this point, the barrier shall be reset to the
	state it had as a result of the most recent pthread_barrier_init() function that referenced it.

	The constant PTHREAD_BARRIER_SERIAL_THREAD is defined in <pthread.h> and its value  shall  be  dis-
	tinct from any other value returned by pthread_barrier_wait().

	The results are undefined if this function is called with an uninitialized barrier.

	If  a signal is delivered to a thread blocked on a barrier, upon return from the signal handler the
	thread shall resume waiting at the barrier if the barrier wait has not completed (that is,  if  the
	required  number of threads have not arrived at the barrier during the execution of the signal han-
	dler); otherwise, the thread shall continue as normal from the completed barrier  wait.  Until  the
	thread  in  the signal handler returns from it, it is unspecified whether other threads may proceed
	past the barrier once they have all reached it.

	A thread that has blocked on a barrier shall not prevent any unblocked thread that is  eligible  to
	use the same processing resources from eventually making forward progress in its execution.  Eligi-
	bility for processing resources shall be determined by the scheduling policy.

	RETURN VALUE
	Upon  successful  completion,  the  pthread_barrier_wait()  function  shall   return   PTHREAD_BAR-
	RIER_SERIAL_THREAD for a single (arbitrary) thread synchronized at the barrier and zero for each of
	the other threads. Otherwise, an error number shall be returned to indicate the error.
*/


void *hello_w(void *thread_num)
{
	int t, result;
	char err_msg[LEN];
	t=random()%10;

	printf("Th %d vai dormir %d s\n",(int)thread_num,t);

	sleep(t);

	printf("Thread %d antes da barreira\n",(int)thread_num);

	result=pthread_barrier_wait(&_barrier);
	if(result>0) {
		strerror_r(result,err_msg,LEN);
		printf("Th %d - erro em barrier_wait: %s\n",(int)thread_num,err_msg);
	}else {
		printf("Th %d - resultado de barrier_wait: ",(int)thread_num);
		if(result==PTHREAD_BARRIER_SERIAL_THREAD)
			printf("PTHREAD_BARRIER_SERIAL_THREAD\n");
		else
			printf("%d\n",result);
	}

	printf("Thread %d depois da barreira\n",(int)thread_num);

	pthread_exit(NULL);
}

int
main()
{
	pthread_t threads[NUM_THREADS];
	int t, status;
	char err_msg[LEN];

	srandom(getpid());
/*
	// int pthread_barrier_init(pthread_barrier_t *restrict barrier,
	                            const pthread_barrierattr_t *restrict attr, unsigned count);

	The  pthread_barrier_init() function shall allocate any resources required to use the barrier referenced
	by barrier and shall initialize the barrier with attributes referenced by attr.  If attr  is  NULL,  the
	default  barrier  attributes  shall  be used; the effect is the same as passing the address of a default
	barrier attributes object. The results are undefined if pthread_barrier_init() is called when any thread
	is  blocked on the barrier (that is, has not returned from the pthread_barrier_wait() call). The results
	are undefined if a barrier is used without  first  being  initialized.  The  results  are  undefined  if
	pthread_barrier_init() is called specifying an already initialized barrier.

	The count argument specifies the  number  of  threads  that  must  call
	pthread_barrier_wait()  before any of them successfully return from the
	call.  The value specified by count must be greater than zero.
*/
 	pthread_barrier_init(&_barrier, NULL, NUM_THREADS);

	for(t=0; t<NUM_THREADS; t++){
		// printf("Criando thread %d\n", t);
		status = pthread_create(&threads[t], NULL, hello_w, (void *)t);
		if (status) {
			strerror_r(errno,err_msg,LEN);
			printf("Falha da criacao da thread %d (%d): %s\n",t,status,err_msg);
			exit(EXIT_FAILURE);
		}
	}

	for(t=0; t<NUM_THREADS; t++) {
		pthread_join(threads[t],NULL);
	}

/*
	// int pthread_barrier_destroy(pthread_barrier_t *barrier);

	The pthread_barrier_destroy() function shall destroy the barrier referenced by barrier and  release  any
	resources  used  by the barrier. The effect of subsequent use of the barrier is undefined until the bar-
	rier is reinitialized by another call to pthread_barrier_init(). An implementation may use this function
	to  set  barrier  to  an invalid value. The results are undefined if pthread_barrier_destroy() is called
	when any thread is blocked on the barrier, or if this function is called with an uninitialized  barrier.
*/

 	pthread_barrier_destroy(&_barrier);

	return(0);
}

