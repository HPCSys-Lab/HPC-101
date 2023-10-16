
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <time.h>


#define N_TIMES	5
#define LEN 128


int pthread_yield (void);

/* time()  returns  the  time  since  the  Epoch (00:00:00 UTC, January 1,
 * 1970), measured in seconds.
 * If t is non-NULL, the return value is also stored in the memory pointed to by t.
 * On success, the value of time in seconds since the Epoch is returned.
 * On error, ((time_t)-1) is returned, and errno is set appropriately. */
// time_t time(time_t *t);

/*
	struct timespec {
		__time_t tv_sec;    // Seconds
		long int tv_nsec;   // Nanoseconds
	};
*/


/*
// System specific semaphore definition.  
typedef struct {
  struct _pthread_fastlock __sem_lock;
  int __sem_value;
  _pthread_descr __sem_waiting;
} sem_t;

// Value returned if `sem_open' failed.  
#define SEM_FAILED	((sem_t *) 0)

// Maximum value the semaphore can have.  
#define SEM_VALUE_MAX (2147483647)

// Initialize semaphore object SEM to VALUE. If PSHARED then share it
// with other processes.  
int sem_init (sem_t *sem, int pshared, unsigned int value);

// Free resources associated with semaphore object SEM.  
int sem_destroy (sem_t *sem);

// Open a named semaphore NAME with open flaot OFLAG.  
sem_t *sem_open (const char *name, int oflag, ...);

// Close descriptor for named semaphore SEM.  
int sem_close (sem_t *sem);

// Remove named semaphore NAME.  
int sem_unlink (const char *name);

// Wait for SEM being posted.  
int sem_wait (sem_t *sem);

// Similar to `sem_wait' but wait only until ABSTIME.  
int sem_timedwait (sem_t *restrict sem, const struct timespec *restrict abstime);

// Test whether SEM is posted.  
int sem_trywait (sem_t *sem);

// Post SEM.  
int sem_post (sem_t *sem);

// Get current value of SEM and store it in *SVAL.  
int sem_getvalue (sem_t *restrict sem, int *restrict sval);
*/


/*
	POSIX 1003.1b semaphores; not to be  confused  with
	SystemV semaphores as described in !ipc!(5), !semctl!(2) and !semop!(2).

	Semaphores are counters for resources shared between threads. The basic opera-
	tions on semaphores are: increment the counter atomically, and wait until  the
	counter is non-null and decrement it atomically.

	!sem_init!  initializes  the  semaphore  object pointed to by |sem|. The count
	associated with the semaphore is set initially to |value|. The |pshared| argu-
	ment  indicates  whether  the  semaphore  is  local  to  the  current  process
	(|pshared| is zero) or is to be shared between several processes (|pshared| is
	not  zero). LinuxThreads currently does not support process-shared semaphores,
	thus !sem_init! always returns with error !ENOSYS! if |pshared| is not zero.

	!sem_wait! suspends the calling thread until the semaphore pointed to by |sem|
	has non-zero count. It then atomically decreases the semaphore count.

	!sem_trywait!  is  a  non-blocking  variant  of  !sem_wait!.  If the semaphore
	pointed to by |sem| has non-zero count, the count is atomically decreased  and
	!sem_trywait!  immediately  returns  0.   If  the  semaphore  count  is  zero,
	!sem_trywait! immediately returns with error !EAGAIN!.

	!sem_post! atomically increases the count  of  the  semaphore  pointed  to  by
	|sem|.  This function never blocks and can safely be used in asynchronous sig-
	nal handlers.

	!sem_getvalue! stores in the location pointed to by |sval| the  current  count
	of the semaphore |sem|.

	!sem_destroy!  destroys  a  semaphore  object,  freeing the resources it might
	hold. No threads should be waiting on the semaphore at the time  !sem_destroy!
	is  called.  In  the  LinuxThreads implementation, no resources are associated
	with semaphore objects,  thus  !sem_destroy!   actually  does  nothing  except
	checking that no thread is waiting on the semaphore.
*/


sem_t s_prod, s_cons; // semaforos

int _val;


void *
produz(void *n_times)
{
	int i, status;
	char err_msg[LEN];
	struct timespec tspec;

	for (i=0; i<(int)n_times; i++) {
		
		// Similar to `sem_wait' but wait only until ABSTIME.  
		// int sem_timedwait (sem_t *restrict sem, const struct timespec *restrict abstime);

		do {
			// determina instante atual + 4 s: timeout
			tspec.tv_sec=time(NULL)+4;
			tspec.tv_nsec=0;

			if((status=sem_timedwait (&s_prod, &tspec))) {
				strerror_r(errno,err_msg,LEN);
				printf("Produtor: falhou sem_timedwait em s_prod (%d): %s\n",errno,err_msg);
		
				// timedwait pode ser usado tambem para evitar deadlock

				// na pratica, deveria fazer outra coisa em vez de ficar em busy wait

				pthread_yield();
			}
			sleep(1);
		} while (status);

		printf("Produtor bloqueou s_prod\n");

		_val++; 
		printf("Produziu %d\n",_val); fflush(stdout);
		sleep(5);

		sem_post(&s_cons);

		printf("Produtor liberou s_cons\n");
	}
	pthread_exit(NULL);
}       

void *
consome(void *n_times)
{
	int i, status;
	char err_msg[LEN];
	struct timespec tspec;

	for (i=0; i<(int)n_times; i++) {
		do {
			// determina instante atual + 4 s: timeout
			tspec.tv_sec=time(NULL)+4;
			tspec.tv_nsec=0;

			if((status=sem_timedwait (&s_cons, &tspec))) {
				strerror_r(errno,err_msg,LEN);
				printf("Consumidor: falhou sem_timedwait em s_cons (%d): %s\n",errno,err_msg);

				// timedwait pode ser usado tambem para evitar deadlock

				// na pratica, deveria fazer outra coisa em vez de ficar em busy wait

				pthread_yield();
			}
			sleep(1);
		} while (status);

		printf("Consumidor bloqueou s_cons\n");

		printf("Consumiu: %d\n",_val);
		sleep(5);

		sem_post(&s_prod);

		printf("Consumidor liberou s_prod\n");
	}
	pthread_exit(NULL);
}


int
main()
{
	pthread_t th_prod, th_cons;
	char err_msg[LEN];

	_val = 0;

	// char *strerror_r(int errnum, char *buf, size_t buflen);

	// int sem_init (sem_t *sem, int pshared, unsigned int value);
	if (sem_init(&s_cons, 0, 0) < 0) {
		strerror_r(errno,err_msg,LEN);
		printf("Erro em sem_init: %s\n",err_msg);
		exit(1);
	}

	// int sem_init (sem_t *sem, int pshared, unsigned int value);
	if (sem_init(&s_prod, 0, 1) < 0) {
		strerror_r(errno,err_msg,LEN);
		printf("Erro em sem_init: %s\n",err_msg);
		exit(1);
	}
	if (pthread_create(&th_prod, NULL, produz, (void *)N_TIMES) != 0) {
		strerror_r(errno,err_msg,LEN);
		printf("Erro em pthread_create: %s\n",err_msg);
		exit(1);
	}
	if (pthread_create(&th_cons, NULL, consome, (void *)N_TIMES) != 0) {
		strerror_r(errno,err_msg,LEN);
		printf("Erro em pthread_create: %s\n",err_msg);
		exit(1);
	}

	pthread_join(th_prod, NULL);
	pthread_join(th_cons, NULL);

	sem_destroy(&s_prod);
	sem_destroy(&s_cons);

	return(0);
}
