/******************************************************************************
* FILE: condvar.c
* DESCRIPTION:
*   Example code for using Pthreads condition variables.  The main thread
*   creates three threads.  Two of those threads increment a "count" variable,
*   while the third thread watches the value of "count".  When "count" 
*   reaches a predefined limit, the waiting thread is signaled by one of the
*   incrementing threads. The waiting thread "awakens" and then modifies
*   count. The program continues until the incrementing threads reach
*   TCOUNT. The main program prints the final value of count.
* SOURCE: Adapted from example code in "Pthreads Programming", B. Nichols
*   et al. O'Reilly and Associates. 
* LAST REVISED: 01/30/09  Blaise Barney
******************************************************************************/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_THREADS  3
#define TCOUNT 10
#define COUNT_LIMIT 12

int     count = 0;
pthread_mutex_t count_mutex;
pthread_cond_t count_threshold_cv;

void *incrementa(void *t) 
{
	int i;
	long my_id = (long)t;

	for (i=0; i < TCOUNT; i++) {
		pthread_mutex_lock(&count_mutex);
		count++;

		/* 
		Check the value of count and signal waiting thread when condition is
		reached.  Note that this occurs while mutex is locked. 
		*/
		if (count == COUNT_LIMIT) {
			printf("incrementa(): thread %ld, count = %d  Threshold reached. ", my_id, count);
			pthread_cond_signal(&count_threshold_cv);
			printf("Just sent signal.\n");
		}
		printf("incrementa(): thread %ld, count = %d, unlocking mutex\n", my_id, count);
		pthread_mutex_unlock(&count_mutex);

		/* Do some work so threads can alternate on mutex lock */
		sleep(1);
	}

	pthread_exit(NULL);
}

void *observa(void *t) 
{
	int my_id = (int)t;

	printf("Starting observa(): thread %d\n", my_id);

	/*
	Lock mutex and wait for signal.  Note that the pthread_cond_wait routine
	will automatically and atomically unlock mutex while it waits. 
	Also, note that if COUNT_LIMIT is reached before this routine is run by
	the waiting thread, the loop will be skipped to prevent pthread_cond_wait
	from never returning.
	*/
	pthread_mutex_lock(&count_mutex);
	if (count < COUNT_LIMIT) {
		printf("observa(): thread %d going into wait...\n", my_id);
		pthread_cond_wait(&count_threshold_cv, &count_mutex);
		printf("observa(): thread %d Condition signal received.\n", my_id);
		count += 125;
		printf("observa(): thread %d count now = %d.\n", my_id, count);
    }
	pthread_mutex_unlock(&count_mutex);

	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	int i; 
	pthread_t threads[NUM_THREADS];

	// Inicializa estruturas
	pthread_mutex_init(&count_mutex, NULL);
	pthread_cond_init (&count_threshold_cv, NULL);

	pthread_create(&threads[0], NULL, observa, (void *)0);
	for(i=1;i<NUM_THREADS;i++) 
		pthread_create(&threads[i], NULL, incrementa, (void *)i);

	// Espera threads terminarem
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}
	printf ("Threads concluídas. Contador: %d\n",count);

	// libera estruturas
	pthread_mutex_destroy(&count_mutex);
	pthread_cond_destroy(&count_threshold_cv);

	pthread_exit (NULL);
}


