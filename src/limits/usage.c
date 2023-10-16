/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: getrusage()
** Objetivo: observar consumo de recursos pelo processo com getrusage
*/

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>

#define toSeconds(t) (t.tv_sec + (t.tv_usec/1000000.))

/*
	int getrusage(int who, struct rusage *usage);

	getrusage returns the  current  resource  usages,  for  a  who  of  either
	RUSAGE_SELF or RUSAGE_CHILDREN.  The former asks for resources used by the
	current process, the latter for resources used by those  of  its  children
	that have terminated and have been waited for.

	struct rusage {
		struct timeval ru_utime; // user time used 
		struct timeval ru_stime; // system time used 
		long   ru_maxrss;        // maximum resident set size 
		long   ru_ixrss;         // integral shared memory size 
		long   ru_idrss;         // integral unshared data size 
		long   ru_isrss;         // integral unshared stack size 
		long   ru_minflt;        // page reclaims 
		long   ru_majflt;        // page faults 
		long   ru_nswap;         // swaps 
		long   ru_inblock;       // block input operations 
		long   ru_oublock;       // block output operations 
		long   ru_msgsnd;        // messages sent 
		long   ru_msgrcv;        // messages received 
		long   ru_nsignals;      // signals received 
		long   ru_nvcsw;         // voluntary context switches 
		long   ru_nivcsw;        // involuntary context switches
	};
*/

int 
main()
{
	struct rusage ru;
	int i,j;
	// int k;
	pid_t pid;
	long cont=0;
	
	// executa operações para consumir tempo de CPU em modo "user"
	for(i=0;i<32767;i++) {
		for(j=0;j<32767;j++);
			// for(k=0;k<5;k++);
			cont++;
	}

	// executa chamadas de sistema para consumir tempo em modo "kernel"
	for(i=0;i<500;i++) {
		pid=getpid();
		getrusage(RUSAGE_SELF,&ru);
		sched_yield();
	}

	// int getrusage(int who, struct rusage *usage);
	// RUSAGE_SELF or RUSAGE_CHILDREN
	if(getrusage(RUSAGE_SELF,&ru)==-1) {
		perror("Erro em getrusage");
		exit(0);
	}

	// struct timeval {
	//   time_t      tv_sec;	// seconds
	//   suseconds_t tv_usec;	// microseconds
	// };

	printf("\n");
	printf("User time used: %f s\n",toSeconds(ru.ru_utime));
	printf("System time used: %f s\n\n",toSeconds(ru.ru_stime));

	printf("Maximum resident size: %ld\n", ru.ru_maxrss);
	printf("Integral shared memory size: %ld\n", ru.ru_ixrss);
	printf("Integral unshared data size: %ld\n", ru.ru_idrss);
	printf("Integral unshared stack size: %ld\n", ru.ru_isrss);
	printf("Page reclaims: %ld\n", ru.ru_minflt);
	printf("Page faults: %ld\n", ru.ru_majflt);
	printf("Swaps: %ld\n", ru.ru_nswap);
	printf("Block input operations: %ld\n", ru.ru_inblock);
	printf("Block output operations: %ld\n", ru.ru_oublock);
	printf("Messages sent: %ld\n", ru.ru_msgsnd);
	printf("Messages received: %ld\n", ru.ru_msgrcv);
	printf("Signals received: %ld\n", ru.ru_nsignals);
	printf("Voluntary context switches: %ld\n", ru.ru_nvcsw);
	printf("Involuntary context switches: %ld\n\n", ru.ru_nivcsw);

	return(0);
}

