
/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: getrlimit(), setrlimit()
** Objetivo: ilustrar o consumo de recursos, ajustando o tempo máximo de consumo de CPU
*/

#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <stdio.h>

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

/*
	#include <ulimit.h>
	long ulimit(int cmd, long newlimit);
	Warning:  This  routine is obsolete. The include file is no longer provided by
	glibc.  Use getrlimit(2), setrlimit(2) and sysconf(3) instead.  

	int getrlimit(int resource, struct rlimit *rlim);
	int setrlimit(int resource, const struct rlimit *rlim);

	getrlimit  and  setrlimit  get and set resource limits respectively.  Each
	resource has an associated soft and hard limit, as defined by  the  rlimit
	(the rlim argument to both getrlimit() and setrlimit()):

	struct rlimit {
		rlim_t rlim_cur;   // Soft limit
		rlim_t rlim_max;   // Hard limit (ceiling for rlim_cur) 
	};

	The soft limit is the value that the kernel enforces for the corresponding
	resource.  The hard limit acts as a ceiling for the soft limit: an unpriv-
	ileged  process may only set its soft limit to a value in the range from 0
	up to the hard limit, and (irreversibly) lower its hard limit.   A  privi-
	leged process may make arbitrary changes to either limit value.

	The value RLIM_INFINITY denotes no limit on a resource (both in the struc-
	ture returned by getrlimit() and in the structure passed to  setrlimit()).

	resource must be one of:

	RLIMIT_AS: The maximum size of the process's virtual memory in bytes. 
	RLIMIT_CORE: Maximum  size  of core file. When 0 no core dump files are created.
	RLIMIT_CPU: CPU time limit in seconds.   When  the  process  reaches  the  soft
              limit,  it  is  sent a SIGXCPU signal. The default action for this
              signal is to terminate the process.  However,  the  signal  can  be
              caught, and the handler can return control to the main program.  If
              the process continues to consume CPU time, it will be sent  SIGXCPU
              once  per  second until the hard limit is reached, at which time it
              is sent SIGKILL.
	RLIMIT_DATA: The maximum size of the process's data segment (initialized  data,
              uninitialized  data,  and heap). 
	RLIMIT_FSIZE: The maximum size of files that the process may create.
	RLIMIT_LOCKS: A limit on the combined number of flock() locks and fcntl() leases
              that this process may establish.  (Early Linux 2.4 only.)
	RLIMIT_MEMLOCK: The maximum number of bytes of virtual memory that may be locked.
	RLIMIT_NOFILE: Specifies a value one greater than the maximum file descriptor num-
              ber that can be opened by this process.  Attempts (open(),  pipe(),
              dup(), etc.)  to exceed this limit yield the error EMFILE.
	RLIMIT_NPROC:The  maximum  number  of processes that can be created for the real
              user ID of the calling  process.
              Upon  encountering  this  limit, fork() fails with the error EAGAIN.
	RLIMIT_RSS: Specifies  the  limit (in pages) of the process's resident set.
	RLIMIT_STACK: The maximum size of the process stack,  in  bytes. 
              Upon  reaching this  limit, a SIGSEGV signal is generated.  
              To handle this signal, a process must employ an alternate signal stack (sigaltstack(2)).
	RLIMIT_OFILE is the BSD name for RLIMIT_NOFILE.
*/

int _num=0;

#define SOFT 5
#define HARD 10


void
sighand(int signum)
{
	printf("SIGXCPU: %d\n",_num++);
} 

int
main()
{
	struct rlimit rlim;

	rlim.rlim_cur=SOFT;
	rlim.rlim_max=HARD;

	// instala rotina para tratar CPU Exceeded
	// SIGXCPU	24,24,30    Core    CPU time limit exceeded (4.2 BSD)
	signal(SIGXCPU, sighand);	

   // int setrlimit(int resource, const struct rlimit *rlim);
   // struct rlimit {
   //   rlim_t rlim_cur;   // Soft limit
   //   rlim_t rlim_max;   // Hard limit (ceiling for rlim_cur)
   // };
	printf("Ajustando RLIMIT_CPU para %d, %d\n",SOFT,HARD);
	if(setrlimit(RLIMIT_CPU,&rlim)==-1) {
		perror("Erro ajustando RLIMIT_CPU");
		exit(0);
	}

	// loop infinito
	for(;;);

	return(0);
}

