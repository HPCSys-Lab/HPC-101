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


/*
POSIX (Portable Operating System Interface) 
É uma família de padrões desenvolvidos pelo IEEE 
(Institute of Electrical and Electronic Engineers). 
Entre eles, o 1003.1 define um padrão para a 
interface do sistema operacional.  
*/

#include <syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <linux/unistd.h>	// _syscallX macros/related stuff
#include <linux/kernel.h>	// struct sysinfo
#include <errno.h>

// #include <sys/sysinfo.h>

int 
main(void) 
{
	pid_t pid1, pid2;
	struct sysinfo s_info;
	int error;

/* 
  syscall - indirect system call

  int syscall(int number, ...);

  Syscall() performs the system call whose assembly language interface has the 
specified number with the specified arguments. Symbolic constants for system calls 
can be found in the header file sys/syscall.h.

  The return value is defined by the system call being invoked. In general, a 0 
return value indicates success. A -1 return value indicates an error, and an error 
code is stored in errno.

  BUGS: There is no way to simulate system calls that have multiple return values such as pipe(2).
*/
	/* chamada direta: SYS_getpid */
	pid1 = syscall(SYS_getpid);
	printf("\nsyscall(SYS_getpid): %d\n", pid1);

	// chamada via interface "libc"
	pid2 = getpid();
	printf ("getpid(): %d\n\n", pid2);

/*
   Calling Directly
   In most cases, it is unnecessary to invoke a system call directly,  but
   there  are  times when the Standard C library does not implement a nice
   function call for you.

   Setup
   The important thing to know about a system call is its prototype.   You
   need  to  know how many arguments, their types, and the function return
   type.  There are six macros that make the actual call into  the  system
   easier.  They have the form:

	_syscallX(type,name,type1,arg1,type2,arg2,...)

   where  X  is 0â€“5, which are the number of arguments taken by the system call
   type is the return type of the system call
   name is the name of the system call
   typeN is the Nth argumentâ€™s type
   argN is the name of the Nth argument

   These macros create a function called name with the arguments you specify.  
   Once you include the _syscall() in your source file, you call the
   system call by name.
*/

/*
	// _syscall1(int, sysinfo, struct sysinfo *, info);
	_syscall1(int, sinfo, struct sysinfo *, info);

	// error = sysinfo(&s_info);
	error = sinfo(&s_info);
	printf("code error = %d\n", error);
	printf("Uptime = %lds\nLoad: 1 min %lu / 5 min %lu / 15 min %lu\n"
	       "RAM: total %lu / free %lu / shared %lu\n"
	       "Memory in buffers = %lu\nSwap: total %lu / free %lu\n"
	       "Number of processes = %d\n\n",
	        s_info.uptime, s_info.loads[0],
	        s_info.loads[1], s_info.loads[2],
	        s_info.totalram, s_info.freeram,
	        s_info.sharedram, s_info.bufferram,
	        s_info.totalswap, s_info.freeswap,
	        s_info.procs);
*/
	return(0);
}

