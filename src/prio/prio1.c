/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa prio1: ajuste da prioridade de processos
** Comandos: getpriority() e setpriority()
   Idéia: verificar prioridade do processo, tal qual manipulada pelos utilitários nice e renice
	Valor da prioridade de tempo real (ajustada com sched_setparam) não é alterado com 
	as funções setpriority e nice.
	sched_getparam usado no exemplo mostra o valor da prioridade de tempo real.
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>

#define MIN_PRIO -20
#define MAX_PRIO  20

// Alteração do nível de nice do processo
//
// int getpriority(int which, int who);
// int setpriority(int which, int who, int prio);

/* The  scheduling priority of the process, process group, or user, as indicated by
   which and who is obtained with the getpriority call and set with the setpriority
   call.   Which is one of PRIO_PROCESS, PRIO_PGRP, or PRIO_USER, and who is inter-
   preted relative to which (a process identifier for PRIO_PROCESS,  process  group
   identifier  for  PRIO_PGRP,  and a user ID for PRIO_USER).  A zero value for who
   denotes (respectively) the calling process, the process  group  of  the  calling
   process,  or  the  real  user ID of the calling process.  Prio is a value in the
   range -20 to 20 (but see the Notes below).  The default  priority  is  0;  lower
   priorities cause more favorable scheduling.
   The  getpriority  call  returns  the  highest  priority (lowest numerical value)
   enjoyed by any of the specified processes.  The setpriority call sets the prior-
   ities of all of the specified processes to the specified value.  Only the super-
   user may lower priorities.
*/

int
main()
{
	int new_prio;
	pid_t my_pid=getpid();
	gid_t my_gid=getgid();
	uid_t my_uid=getuid();
	uid_t my_pgid=getpgrp();

	do {
		printf("\n");

		// printf("Minha prioridade: %d\n", getpriority(PRIO_PROCESS,(int)my_pid));
		printf("Minha prioridade: %d\n", getpriority(PRIO_PROCESS,0));
		// printf("Prio usuario (%d): %d\n", (int)my_uid,getpriority(PRIO_USER,(int)my_uid));
		printf("Prio usuario (%d): %d\n", (int)my_uid,getpriority(PRIO_USER,0));
		// printf("Prio meu grupo (%d): %d\n", (int)my_gid,getpriority(PRIO_PGRP,my_pgid));
		printf("Prio meu grupo (%d): %d\n", (int)my_gid,getpriority(PRIO_PGRP,0));

		printf("\nNova prioridade (99 para encerrar): ");
		scanf("%d",&new_prio);

		// if(setpriority(PRIO_PROCESS,(int)my_pid,new_prio)==-1) 
		if(setpriority(PRIO_PROCESS,0,new_prio)==-1) 
			perror("Erro ajustando prioridade");

	}while (new_prio!=99);

	return 0;
}



