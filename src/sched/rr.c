/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
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

	// int sched_setscheduler(pid_t pid, int policy, const struct  sched_param *p);
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
	fflush(stdout);

	// struct sched_param {
	//		int sched_priority;
	// };
	p.sched_priority=1;

	// int sched_setscheduler(pid_t pid, int policy, const struct  sched_param *p);
	if(sched_setscheduler(0,SCHED_RR,&p)==-1) {
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
	fflush(stdout);

	for(n=0;n<3;n++) {
		printf("Mais prioritário, vou monopolizar uma CPU..\n"); fflush(stdout);

		for(l=0;l<100;l++) {
			for(i=0;i<327;i++) 
				for(j=0;j<32767;j++)
					for(k=0;k<10;k++);
			printf(" rr ");fflush(stdout);
		}	
		printf("Vou dormir 10 segundos... aproveitem...\n"); fflush(stdout);;
		sleep(10);
	}

	return 0;
}

