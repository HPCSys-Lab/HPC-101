/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : 
** Comando: 
** Objetivo: testar o uso de POSIX semaphores para sincronização entre processos.
		Parâmetro pshared no ajuste do semáforo indica se compartilhamento do
		semáforo é apenas entre threads de um processo ou global,
		entre threads de processos distintos.
		Para compartilhar semáforos entre processos, usa-se descritor em memória
		compartilhada, o que gera a mesma sobrecarga que usar SYSV IPC! 
*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define N_TIMES	5
#define MAX 128

/*
	POSIX 1003.1b semaphores; not to be  confused  with
	SystemV semaphores as described in !ipc!(5), !semctl!(2) and !semop!(2).

	Semaphores are counters for resources shared between threads. The basic opera-
	tions on semaphores are: increment the counter atomically, and wait until  the
	counter is non-null and decrement it atomically.

//antigo
	!sem_init!  initializes  the  semaphore  object pointed to by |sem|. The count
	associated with the semaphore is set initially to |value|. The |pshared| argu-
	ment  indicates  whether  the  semaphore  is  local  to  the  current  process
	(|pshared| is zero) or is to be shared between several processes (|pshared| is
	not  zero). LinuxThreads currently does not support process-shared semaphores,
	thus !sem_init! always returns with error !ENOSYS! if |pshared| is not zero.

// mais recente
sem_init() initializes the unnamed semaphore at the address pointed to by sem.  The
value argument specifies the initial value for the semaphore.
The pshared argument indicates whether this semaphore is to be shared  between  the
threads of a process, or between processes.
If  pshared  has the value 0, then the semaphore is shared between the threads of a
process, and should be located at some address  that  is  visible  to  all  threads
(e.g., a global variable, or a variable allocated dynamically on the heap).
If  pshared is non-zero, then the semaphore is shared between processes, and should
be located in a region of shared memory (see shm_open(3), mmap(2), and  shmget(2)).
(Since  a  child  created  by fork(2) inherits its parent's memory mappings, it can
also access the semaphore.)  Any process that can access the shared  memory  region
can operate on the semaphore using sem_post(3), sem_wait(3), etc.
Initializing  a  semaphore  that  has already been initialized results in undefined
behavior.

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

	RETURN VALUE
	The  !sem_wait!  and  !sem_getvalue! functions always return 0.  All other 
	semaphore functions return 0 on success and -1 on error, in
	addition to writing an error code in !errno!.
*/

// semáforos
// sem_t s_prod, s_cons; 
sem_t *sema;

// buffer compartilhado
int *val;


void
produz(int n_times)
{
	int i;

	for (i=0; i<n_times; i++) {
		// sem_wait(&s_prod);
		sem_wait(&sema[1]);
		// deveria usar outro semáforo (ou mutex) para garantir exclusão mútua no acesso a val
		(*val)++; 
		printf("Produziu %d\n",*val); fflush(stdout);
		// sem_post(&s_cons);
		sem_post(&sema[0]);
	}
	// pthread_exit(NULL);
}       

void 
consome(int n_times)
{
	int i;

	for (i=0; i<n_times; i++) {
		// sem_wait(&s_cons);
		sem_wait(&sema[0]);
		// deveria usar outro semáforo (ou mutex) para garantir exclusão mútua no acesso a val
		printf("Consumiu: %d\n",*val);
		sleep(random()%5);
		// sem_post(&s_prod);
		sem_post(&sema[1]);
	}
	// pthread_exit(NULL);
}


int
main()
{
	pid_t pid;
	char err_msg[MAX];
	int status, shmid1, shmid2;
	struct shmid_ds shmds;

	if((shmid1=shmget(IPC_PRIVATE,2*sizeof(sem_t),0700)) == -1) {
		perror("Erro na alocação da shm");
		exit(0);
	}
	// associa semáforo à shm 
	sema=(sem_t *)shmat(shmid1,0,0);

	if((shmid2=shmget(IPC_PRIVATE,sizeof(int),0700)) == -1) {
		perror("Erro na alocação da shm");
		exit(0);
	}
	// associa buffer (contador) à shm 
	val=(int *)shmat(shmid2,0,0);
	*val = 0;

	// int sem_init (sem_t *sem, int pshared, unsigned int value);
	// if (sem_init(&s_cons, 1, 0) == -1) {
	if (sem_init(&sema[0], 1, 0) == -1) {
		strerror_r(errno,err_msg,MAX);
		printf("Erro em sem_init: %s\n",err_msg);
		exit(1);
	}

	// int sem_init (sem_t *sem, int pshared, unsigned int value);
	//if (sem_init(&s_prod, 1, 1) == -1) {
	if (sem_init(&sema[1], 1, 1) == -1) {
		strerror_r(errno,err_msg,MAX);
		printf("Erro em sem_init: %s\n",err_msg);
		exit(1);
	}

	if((pid=fork())==-1) {
		printf("Erro criando processo filho: %s\n",(char *)strerror_r(errno,err_msg,MAX));
	} else {
		if(pid)	// pai
			produz(N_TIMES);
		else		// filho
			consome(N_TIMES);
	}	

	// pai e filho desconectam-se da mem compart 
	status=shmdt((sem_t *)sema);
	status=shmdt((int *)val);

	if(pid) {
		pid=waitpid(-1,&status,0);
		
		// sem_destroy(&s_prod);
		// sem_destroy(&s_cons);
		sem_destroy(&sema[0]);
		sem_destroy(&sema[1]);

		// libera shm
		status=shmctl(shmid1,IPC_RMID,&shmds); 
		status=shmctl(shmid2,IPC_RMID,&shmds); 
	}

	return(0);
}

