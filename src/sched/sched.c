/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: 
** Objetivo: verificar informações da política de escalonamento e prioridade estática ativa
*/


#include <sched.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/resource.h>



int sched_getaffinity(pid_t pid, unsigned int len, unsigned long *mask);


int 
main()
{
	unsigned long mask;
	int prio_max, prio_min;
	struct sched_param p;
	struct timespec ts;

/*	
int sched_setscheduler(pid_t pid, int policy, const struct  sched_param *p);
int sched_getscheduler(pid_t pid);

struct sched_param {
	...
	int sched_priority;
	...
};
*/
/*
	sched_setscheduler  sets  both the scheduling policy and the associated
	parameters for the process identified by pid. If pid equals  zero,  the
	scheduler of the calling process will be set. The interpretation of the
	parameter p depends on the selected policy.  Currently,  the  following
	three  scheduling  policies  are  supported  under  Linux:  SCHED_FIFO,
	SCHED_RR, and SCHED_OTHER; their respective semantics are described below.

	sched_getscheduler  queries  the scheduling policy currently applied to
	the process identified by pid. If pid equals zero, the  policy  of  the
	calling process will be retrieved.

	The  scheduler  is  the kernel part that decides which runnable process
	will be executed by the CPU next. The Linux scheduler offers three dif-
	ferent  scheduling policies, one for normal processes and two for real-
	time applications. A static priority value sched_priority  is  assigned
	to  each  process  and this value can be changed only via system calls.
	Conceptually, the scheduler maintains a list of runnable processes  for
	each possible sched_priority value, and sched_priority can have a value
	in the range 0 to 99. In order to determine the process that runs next,
	the  Linux  scheduler  looks  for  the  non-empty list with the highest
	static priority and takes the process at the head  of  this  list.  The
	scheduling  policy  determines  for  each  process,  where  it  will be
	inserted into the list of processes with equal static priority and  how
	it will move inside this list.

	SCHED_OTHER is the default universal time-sharing scheduler policy used
	by most processes, SCHED_FIFO and SCHED_RR  are  intended  for  special
	time-critical  applications  that  need precise control over the way in
	which runnable processes are selected for execution.  Processes  sched-
	uled with SCHED_OTHER must be assigned the static priority 0, processes
	scheduled under SCHED_FIFO or SCHED_RR can have a  static  priority  in
	the  range  1 to 99. Only processes with superuser privileges can get a
	static priority higher than 0 and  can  therefore  be  scheduled  under
	SCHED_FIFO  or  SCHED_RR.  The  system calls sched_get_priority_min and
	sched_get_priority_max can be used to find out the valid priority range
	for  a  scheduling  policy in a portable way on all POSIX.1b conforming
	systems.

	All scheduling is preemptive: If a process with a higher static  prior-
	ity  gets  ready  to  run,  the  current  process will be preempted and
	returned into its wait list. The scheduling policy only determines  the
	ordering within the list of runnable processes with equal static priority.

	Child processes inherit the scheduling algorithm and parameters across a fork.
*/

	// int sched_getscheduler(pid_t pid);
	// int sched_getparam(pid_t pid, struct sched_param *p);
	// switch (sched_getscheduler(getpid())) {

	if(sched_getparam(0,&p)==-1)
		perror("Falha em sched_getparam");

	switch (sched_getscheduler(0)) {
		case SCHED_FIFO:
			// printf("\nScheduler: SCHED_FIFO. Priority: %d\n",getpriority(PRIO_PROCESS,0));
			printf("\nScheduler: SCHED_FIFO. Priority: %d\n",p.sched_priority);
			break;
		case SCHED_RR:
			// printf("\nScheduler: SCHED_RR. Priority: %d\n",getpriority(PRIO_PROCESS,0));
			printf("\nScheduler: SCHED_RR. Priority: %d\n",p.sched_priority);
			break;
		case SCHED_OTHER:
			// printf("\nScheduler: SCHED_OTHER. Priority: %d\n",getpriority(PRIO_PROCESS,0));
			printf("\nScheduler: SCHED_OTHER. Priority: %d\n",p.sched_priority);
			break;
	}
/*
	// int getpriority(int which, int who);
	// getpriority retorna nível de nice, não prioridade de tempo real
*/

/*
	int sched_setparam(pid_t pid, const struct sched_param *p);
	int sched_getparam(pid_t pid, struct sched_param *p);

	struct sched_param {
		...
		int sched_priority;
		...
	};

	sched_setparam  sets  the  scheduling parameters associated with the
	scheduling policy for the process identified by pid. If pid is zero,
	then  the parameters of the current process are set. The interpreta-
	tion of the parameter p depends on the selected  policy.  Currently,
	the  following  three scheduling policies are supported under Linux:
	SCHED_FIFO, SCHED_RR, and SCHED_OTHER.
	sched_getparam retrieves the scheduling parameters for  the  process
	identified  by  pid. If pid is zero, then the parameters of the cur-
	rent process are retrieved.

	sched_setparam checks the validity of p for the scheduling policy of
	the  process.  The  parameter  p->sched_priority must lie within the
	range given by sched_get_priority_min and sched_get_priority_max.

	POSIX systems on which sched_setparam and sched_getparam are  avail-
	able define _POSIX_PRIORITY_SCHEDULING in <unistd.h>.
*/

/*
	int sched_setaffinity(pid_t pid, unsigned int len, unsigned long *mask);
	int sched_getaffinity(pid_t pid, unsigned int len, unsigned long *mask);

	sched_setaffinity sets the CPU affinity mask of the process  denoted
	by pid.  If pid is zero, then the current process is used.

	The affinity mask is represented by the bitmask stored in mask.  The
	least significant bit corresponds to  the  first  logical  processor
	number  on the system, while the most significant bit corresponds to
	the last logical processor number on the system.  A set  bit  corre-
	sponds  to  a legally schedulable CPU while an unset bit corresponds
	to an illegally schedulable CPU.  In other words, a process is bound
	to  and  will only run on processors whose corresponding bit is set.
	Usually, all bits in the mask are set.

	The argument len is the length of the data pointed to by mask.  Nor-
	mally  this  is the size of a word on the system.  For compatibility
	with future versions of  the  Linux  kernel,  since  this  size  can
	change, the bitmask supplied must be at least as large as the affin-
	ity mask stored in the kernel.

	The function sched_getaffinity writes into the pointer  supplied  by
  	mask  that  is size len the affinity mask of process pid.  If pid is
	zero, then the mask of the current process is returned.
*/
	// int sched_getaffinity(pid_t pid, unsigned int len, unsigned long *mask);
	if(sched_getaffinity(0, sizeof(unsigned long), &mask)==-1)
		perror("Erro em sched_getaffinity");
	else
		printf("Affinity: %ld\n",mask);


/*
	int sched_get_priority_max(int policy);
	int sched_get_priority_min(int policy);

	sched_get_priority_max returns the maximum priority value  that  can
	be   used  with  the  scheduling  algorithm  identified  by  policy.
	sched_get_priority_min returns the minimum priority value  that  can
	be  used  with  the  scheduling algorithm identified by policy. Sup-
	ported policy values are SCHED_FIFO, SCHED_RR, and SCHED_OTHER.

	Processes with numerically  higher  priority  values  are  scheduled
	before  processes  with numerically lower priority values. Thus, the
	value returned by sched_get_priority_max will be  greater  than  the
	value returned by sched_get_priority_min.

	Linux  allows the static priority value range 1 to 99 for SCHED_FIFO
	and SCHED_RR and the priority 0 for SCHED_OTHER. Scheduling priority
	ranges for the various policies are not alterable.

	The  range of scheduling priorities may vary on other POSIX systems,
	thus it is a good idea for portable applications to  use  a  virtual
	priority  range and map it to the interval given by sched_get_prior-
	ity_max and sched_get_priority_min.  POSIX.1b requires a  spread  of
	at  least  32  between  the  maximum  and  the  minimum  values  for
	SCHED_FIFO and SCHED_RR.

	POSIX systems on which sched_get_priority_max  and  sched_get_prior-
	ity_min   are   available   define   _POSIX_PRIORITY_SCHEDULING   in
	<unistd.h>.
*/
#ifdef _POSIX_PRIORITY_SCHEDULING
	// int sched_get_priority_max(int policy);
	if((prio_max=sched_get_priority_max(SCHED_FIFO))==-1)
		perror("Erro em sched_get_priority_max");
	// int sched_get_priority_min(int policy);
	if((prio_min=sched_get_priority_min(SCHED_FIFO))==-1)
		perror("Erro em sched_get_priority_min");

	printf("SCHED_FIFO: Priority Max: %d. Priority Min: %d\n",prio_max,prio_min);

	if((prio_max=sched_get_priority_max(SCHED_RR))==-1)
		perror("Erro em sched_get_priority_max");
	if((prio_min=sched_get_priority_min(SCHED_RR))==-1)
		perror("Erro em sched_get_priority_min");

	printf("SCHED_RR: Priority Max: %d. Priority Min: %d\n",prio_max,prio_min);

	if((prio_max=sched_get_priority_max(SCHED_OTHER))==-1)
		perror("Erro em sched_get_priority_max");
	if((prio_min=sched_get_priority_min(SCHED_OTHER))==-1)
		perror("Erro em sched_get_priority_min");

	printf("SCHED_OTHER: Priority Max: %d. Priority Min: %d\n",prio_max,prio_min);

/*
	int sched_rr_get_interval(pid_t pid, struct timespec *tp);

	struct timespec {
		time_t  tv_sec;     // seconds
		long    tv_nsec;    // nanoseconds
	};

	sched_rr_get_interval writes into the timespec structure pointed  to
	by  tp  the  round  robin time quantum for the process identified by
	pid. If pid is zero, the time quantum for  the  calling  process  is
	written into *tp. The identified process should be running under the
	SCHED_RR scheduling policy.

	The round robin time quantum value is not alterable under Linux 1.3.81.

	On success,  sched_rr_get_interval  returns  0.   On  error,  -1  is
	returned, and errno is set appropriately.
*/
	// int sched_rr_get_interval(pid_t pid, struct timespec *tp);
	if(sched_rr_get_interval(0, &ts)==-1)
		perror("Erro em sched_rr_get_interval");
	else {
		printf("\n");
		printf("Quantum: %.9f\n",ts.tv_sec+(ts.tv_nsec/1000000000.));
		printf("Quantum: %d.%ld\n",(int)ts.tv_sec,ts.tv_nsec);
	}

	printf("\n");
#endif

	return 0;
}

