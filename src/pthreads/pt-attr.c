/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/
/*
** Programa : 
** Comando:
** Objetivo: ajustar atributos variáveis para a criação de threads
**		Testar como root: pt-attr -exp -rr -p 1
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>


#define NUM_THREADS	4

#define LEN 		128
#define MIN_PRIO	0
#define MAX_PRIO	99

/*
int pthread_attr_init(pthread_attr_t *attr);
int pthread_attr_destroy(pthread_attr_t *attr);

int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int pthread_attr_getdetachstate(const pthread_attr_t *attr, int *detachstate);

int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);

int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);

int pthread_attr_setinheritsched(pthread_attr_t *attr, int inherit);
int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inherit);

int pthread_attr_setscope(pthread_attr_t *attr, int scope);
int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
*/


/*
	Setting  attributes  for  threads  is  achieved  by  filling  a  thread
	attribute  object  |attr|  of type !pthread_attr_t!, then passing it as
	second argument to !pthread_create!(3). Passing !NULL! is equivalent to
	passing  a  thread  attribute  object  with all attributes set to their
	default values.

	!pthread_attr_init! initializes the thread attribute object |attr|  and
	fills  it  with  default values for the attributes. (The default values
	are listed below for each attribute.)
	
	Each attribute |attrname| (see below for a list of all attributes)  can
	be individually set using the function !pthread_attr_set!|attrname| and
	retrieved using the function !pthread_attr_get!|attrname|.
	
	!pthread_attr_destroy! destroys a thread attribute object,  which  must
	not  be  reused until it is reinitialized. !pthread_attr_destroy!  does
	nothing in the LinuxThreads implementation.

	Attribute objects are consulted only when creating a  new  thread.  The
	same attribute object can be used for creating several threads. Modify-
	ing an attribute object after  a  call  to  !pthread_create!  does  not
	change the attributes of the thread previously created.

	The following thread attributes are supported:


   detachstate
       Control  whether  the  thread  is  created in the joinable state (value
       !PTHREAD_CREATE_JOINABLE!) or  in  the  detached  state  (!PTHREAD_CRE-
       ATE_DETACHED!).
       Default value: !PTHREAD_CREATE_JOINABLE!.

       In  the  joinable  state,  another thread can synchronize on the thread
       termination and recover its termination code  using  !pthread_join!(3),
       but  some  of  the thread resources are kept allocated after the thread
       terminates,  and  reclaimed   only   when   another   thread   performs
       !pthread_join!(3) on that thread.
       In  the detached state, the thread resources are immediately freed when
       it terminates, but !pthread_join!(3) cannot be used to  synchronize  on
       the thread termination.
       A thread created in the joinable state can later be put in the detached
       thread using !pthread_detach!(3).

   schedpolicy
       Select the scheduling policy for the thread: one of !SCHED_OTHER! (reg-
       ular,  non-realtime  scheduling), !SCHED_RR! (realtime, round-robin) or
       !SCHED_FIFO! (realtime, first-in first-out).  See  !sched_setpolicy!(2)
       for more information on scheduling policies.
       Default value: !SCHED_OTHER!.

       The realtime scheduling policies !SCHED_RR! and !SCHED_FIFO! are avail-
       able only to processes with superuser privileges.
       The scheduling policy of a thread can be changed  after  creation  with
       !pthread_setschedparam!(3).

   schedparam
       Contain  the  scheduling parameters (essentially, the scheduling prior-
       ity) for the thread. See !sched_setparam!(2) for  more  information  on
       scheduling parameters.
       Default value: priority is 0.
       This   attribute  is  not  significant  if  the  scheduling  policy  is
       !SCHED_OTHER!; it only matters for the realtime policies !SCHED_RR! and
       !SCHED_FIFO!.
       The  scheduling priority of a thread can be changed after creation with
       !pthread_setschedparam!(3).

   inheritsched
       Indicate whether the scheduling policy and  scheduling  parameters  for
       the newly created thread are determined by the values of the |schedpol-
       icy| and |schedparam| attributes  (value  !PTHREAD_EXPLICIT_SCHED!)  or
       are inherited from the parent thread (value !PTHREAD_INHERIT_SCHED!).
       Default value: !PTHREAD_EXPLICIT_SCHED!.

   scope
       Define  the  scheduling  contention  scope for the created thread.  The
       only  value   supported   in   the   LinuxThreads   implementation   is
       !PTHREAD_SCOPE_SYSTEM!,  meaning  that the threads contend for CPU time
       with all processes running on the machine. In particular, thread prior-
       ities are interpreted relative to the priorities of all other processes
       on  the  machine.  The  other  value   specified   by   the   standard,
       !PTHREAD_SCOPE_PROCESS!,  means  that scheduling contention occurs only
       between the threads of  the  running  process:  thread  priorities  are
       interpreted  relative  to  the  priorities  of the other threads of the
       process,   regardless   of   the   priorities   of   other   processes.
       !PTHREAD_SCOPE_PROCESS! is not supported in LinuxThreads.
       Default value: !PTHREAD_SCOPE_SYSTEM!.
*/


int pthread_getattr_np (pthread_t __th, pthread_attr_t *__attr);

/* The official definition
struct sched_param {
	int __sched_priority;
};
*/

void *hello_w(void *thread_num)
{
	char err_msg[LEN];
	pthread_t id;
	pthread_attr_t attr;
	int status, state, policy, inherit, scope;
	struct sched_param param;

	id=pthread_self();

	// obtém atributos desta thread
	// int pthread_getattr_np (pthread_t th, pthread_attr_t *attr);
	pthread_getattr_np (id, &attr);

	// obtém detachstate	
	// int pthread_attr_getdetachstate (const pthread_attr_t *attr, int *detachstate);
	if((status=pthread_attr_getdetachstate(&attr, &state))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em getdetachstate: %s\n",err_msg);
	}

	// int pthread_attr_getschedpolicy(const pthread_attr_t *attr, int *policy);
	if((status=pthread_attr_getschedpolicy(&attr, &policy))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em getschedpolicy: %s\n",err_msg);
	}

	// int pthread_attr_getschedparam(const pthread_attr_t *attr, struct sched_param *param);
	if((status=pthread_attr_getschedparam(&attr, &param))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em getschedparam: %s\n",err_msg);
	}

	// int pthread_attr_getinheritsched(const pthread_attr_t *attr, int *inherit);
	if((status=pthread_attr_getinheritsched(&attr, &inherit))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em getinheritsched: %s\n",err_msg);
	}

	// int pthread_attr_getscope(const pthread_attr_t *attr, int *scope);
	if((status=pthread_attr_getscope(&attr, &scope))) {
		strerror_r(status,err_msg,LEN);
		printf("Erro em getscope: %s\n",err_msg);
	}

	// libera attr
	pthread_attr_destroy(&attr);

	// sleep(20);

	printf("Hello, this is thread %d! tid=%lu pid=%d "
	       "state=%d policy=%d sched_param=%d inherit=%d scope=%d\n",
	       (int)thread_num,id,getpid(),
	       state,policy,param.sched_priority,inherit,scope);
	       // state,policy,param.__sched_priority,inherit,scope);

	pthread_exit(NULL);
}

void
uso(char *nome)
{
	printf("Uso: %s\n"
		"\t[-j]      // (PTHREAD_JOINABLE)\n"
		"\t[-d]      // (PTHREAD_DETACHED)\n"
		"\t[-exp]    // (EXPLICIT_SCHED)\n"
		"\t[-inh]    // (INHERIT_SCHED)\n"
		"\t[-other]  // (SCHED_OTHER)\n"
		"\t[-rr]     // (SCHED_RR)\n"
		"\t[-fifo]   // (SCHED_FIFO)\n"
		"\t[-p prio] // (SCHED_PARAM: OTHER(0), RR e FIFO: [1..99]\n"
		"\t[-sys]    // (SCOPE_SYSTEM)\n"
		"\t[-proc]   // (SCOPE_PROCESS)\n"
		,nome);
}

int 
main (int argc, char *argv[])
{
	pthread_t threads[NUM_THREADS];
	pthread_attr_t attr;
	int i, status, prio, min_prio, max_prio, policy=SCHED_OTHER;
	struct sched_param  param;
	char err_msg[LEN];

	if(argc<2) {
		uso(argv[0]);
		exit(0);
	}

	// Cria pthread_attr e ajusta atributos
	pthread_attr_init(&attr);

	for(i=1;i<argc;i++) {

		// printf("Arg: %s\n",argv[i]);

		// Ajusta detachstate: PTHREAD_CREATE_JOINABLE ou PTHREAD_CREATE_DETACHED
		// int pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
		if(!strcmp(argv[i],"-j"))
			if((status=pthread_attr_setdetachstate(&attr, (int)PTHREAD_CREATE_JOINABLE))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setsdetachstate (%d): %s\n",status,err_msg);
			}
		if(!strcmp(argv[i],"-d"))
			if((status=pthread_attr_setdetachstate(&attr, (int)PTHREAD_CREATE_DETACHED))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setsdetachstate (%d): %s\n",status,err_msg);
			}

		// Ajusta inheritsched
		// PTHREAD_EXPLICIT_SCHED (default): policy e schedparam determinados pelos atributos 
		// PTHREAD_INHERIT_SCHED: policy e schedparam herdados do pai
		// int pthread_attr_setinheritsched(pthread_attr_t *attr, int inherit);
		if(!strcmp(argv[i],"-exp")) 
			if((status=pthread_attr_setinheritsched(&attr, (int)PTHREAD_EXPLICIT_SCHED))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setinheritsched (%d): %s\n",status,err_msg);
			}
		if(!strcmp(argv[i],"-inh")) 
			if((status=pthread_attr_setinheritsched(&attr, (int)PTHREAD_INHERIT_SCHED))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setinheritsched (%d): %s\n",status,err_msg);
			}

		// Ajusta schedpolicy: 
		// SCHED_OTHER normal (default), SCHED_RR realtime, r-robin, SCHED_FIFO realtime, fifo
		// int pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
		if(!strcmp(argv[i],"-other"))  {
			policy=SCHED_OTHER;
			if((status=pthread_attr_setschedpolicy(&attr, SCHED_OTHER))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setschedpolicy (%d): %s\n",status,err_msg);
			}
		}
		if(!strcmp(argv[i],"-rr")) {
			policy=SCHED_RR;
			if((status=pthread_attr_setschedpolicy(&attr, SCHED_RR))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setschedpolicy (%d): %s\n",status,err_msg);
			}
		}
		if(!strcmp(argv[i],"-fifo")) {
			policy=SCHED_RR;
			if((status=pthread_attr_setschedpolicy(&attr, SCHED_FIFO))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setschedpolicy (%d): %s\n",status,err_msg);
			}
		}

		// Ajusta schedparam (prioridade). Relevante apenas para SCHED_RR e SCHED_FIFO
		// int pthread_attr_setschedparam(pthread_attr_t *attr, const struct sched_param *param);
		// Default value: priority is 0.
		if(!strcmp(argv[i],"-p")) {
			if(argc>i+1) {
				prio=atoi(argv[i+1]);
				max_prio=sched_get_priority_max(policy);
				min_prio=sched_get_priority_min(policy);
				if(prio>=min_prio && prio<=max_prio) {
					param.sched_priority=prio;
					if((status=pthread_attr_setschedparam(&attr, &param))) {
						strerror_r(status,err_msg,LEN);
						printf("Erro em pthread_attr_setschedparam (%d): %s\n",status,err_msg);
					}
				} else {
					uso(argv[0]);
					exit(0);
				}
				i++;
			}else {
				uso(argv[0]);
				exit(0);
			}
		}

		// Ajusta scope
		// PTHREAD_SCOPE_SYSTEM (default): threads contend for CPU time with all processes 
		// Priorities are interpreted relative to the priorities of all other processes
		// PTHREAD_SCOPE_PROCESS: means that scheduling contention occurs only between the
		// threads of the running process: PTHREAD_SCOPE_PROCESS is not supported in LinuxThreads.
		// Default value: PTHREAD_SCOPE_SYSTEM
		// int pthread_attr_setscope(pthread_attr_t *attr, int scope);
		if(!strcmp(argv[i],"-sys")) 
			if((status=pthread_attr_setscope(&attr, (int)PTHREAD_SCOPE_SYSTEM))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setscope (%d): %s\n",status,err_msg);
			}
		if(!strcmp(argv[i],"-proc")) 
			if((status=pthread_attr_setscope(&attr, (int)PTHREAD_SCOPE_PROCESS))) {
				strerror_r(status,err_msg,LEN);
				printf("Erro em pthread_attr_setscope (%d): %s\n",status,err_msg);
			}
	} // for

	for(i=0; i<NUM_THREADS; i++) {
		// printf("Criando thread %d\n", i);
		if((status=pthread_create(&threads[i], &attr, hello_w, (void *)i))) {
			strerror_r(status,err_msg,LEN);
			printf("Erro criando thread (%d): %s\n",status,err_msg);
			exit(0);
		}
	}

	// Libera estrutura dos atributos 
	pthread_attr_destroy(&attr);

	pthread_exit(NULL);
}


