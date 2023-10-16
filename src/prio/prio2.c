/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa prio2: ajuste do nivel de nice de um processo
** Comandos: nice()
*/
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>

#define MIN_PRIO -20
#define MAX_PRIO  20

// int nice(int inc);

/* nice  adds  inc  to the nice value for the calling pid.  (A large nice value means a
   low priority.)  Only the superuser may specify a  negative  increment,  or  priority
   increase.
   RETURN VALUE
   On  success, zero is returned.  On error, -1 is returned, and errno is set appropriately.

	Manipula o mesmo valor de prioridade tratado por set_priority e get_priority
*/

int
main()
{
	int nice_val;
	pid_t my_pid=getpid();
	gid_t my_gid=getgid();
	uid_t my_uid=getuid();

	do {
		// printf("\nMinha prioridade: %d\n",getpriority(PRIO_PROCESS,(int)my_pid));
		// printf("Prio meu grupo (%d): %d\n",(int)my_gid,getpriority(PRIO_PGRP,(int)my_gid));
		// printf("Prio usuario (%d): %d\n",(int)my_uid,getpriority(PRIO_USER,(int)my_uid));
		printf("\nMinha prioridade: %d\n",getpriority(PRIO_PROCESS,0));
		printf("Prio meu grupo (%d): %d\n",(int)my_gid,getpriority(PRIO_PGRP,0));
		printf("Prio usuario (%d): %d\n",(int)my_uid,getpriority(PRIO_USER,0));

		printf("\nNovo valor nice (99 para encerrar): ");
		scanf("%d",&nice_val);

		if(nice(nice_val)==-1)
			perror("Erro ajustando nice");

	}while (nice_val!=99);

	return(0);
}

