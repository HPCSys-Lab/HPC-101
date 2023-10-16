/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: 
** Objetivo: 
*/


#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/resource.h>



int 
main()
{
	int i,j,k,n,l;
	struct sched_param p;

/*	
int sched_setscheduler(pid_t pid, int policy, const struct  sched_param *p);

	sched_setscheduler  sets  both the scheduling policy and the associated
	parameters for the process identified by pid. If pid equals  zero,  the
	scheduler of the calling process will be set. The interpretation of the
	parameter p depends on the selected policy.  Currently,  the  following
	three  scheduling  policies  are  supported  under  Linux:  SCHED_FIFO,
	SCHED_RR, and SCHED_OTHER; their respective semantics are described below.

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

	if(sched_getparam(0,&p)==-1)
		perror("Falha em sched_getparam");

	switch (sched_getscheduler(0)) {
		case SCHED_FIFO:
			printf("\nScheduler: SCHED_FIFO. Priority: %d\n",p.sched_priority);
			break;
		case SCHED_RR:
			printf("\nScheduler: SCHED_RR. Priority: %d\n",p.sched_priority);
			break;
		case SCHED_OTHER:
			printf("\nScheduler: SCHED_OTHER. Priority: %d\n",p.sched_priority);
			break;
	}

	// struct sched_param {
	//		...
	//		int sched_priority;
	//		...
	// };
	p.sched_priority=1;

	// int sched_setscheduler(pid_t pid, int policy, const struct  sched_param *p);
	if(sched_setscheduler(0,SCHED_FIFO,&p)==-1) {
		perror("Falha em sched_setscheduler");
		exit(0);
	}	

	if(sched_getparam(0,&p)==-1)
		perror("Falha em sched_getparam");

	switch (sched_getscheduler(0)) {
		case SCHED_FIFO:
			printf("\nScheduler: SCHED_FIFO. Priority: %d\n",p.sched_priority);
			break;
		case SCHED_RR:
			printf("\nScheduler: SCHED_RR. Priority: %d\n",p.sched_priority);
			break;
		case SCHED_OTHER:
			printf("\nScheduler: SCHED_OTHER. Priority: %d\n",p.sched_priority);
			break;
	}

	for(n=0;n<5;n++) {
		printf("Mais prioritário, vou monopolizar uma CPU..\n"); fflush(stdout);

		for(l=0;l<100;l++) {
			for(i=0;i<327;i++) 
				for(j=0;j<32767;j++)
					for(k=0;k<10;k++);
			printf(" fifo ");fflush(stdout);
		}	

		printf("Vou dormir 3 segundos... aproveitem...\n"); fflush(stdout);;
		sleep(3);
	}

	return 0;
}

