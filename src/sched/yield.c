/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: sched_yield()
** Objetivo: ilustrar a liberação voluntária da CPU com sched_yield()
*/


#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>


#define toSeconds(t) (t.tv_sec + (t.tv_usec/1000000.))

int 
main()
{
	struct timeval inic,fim;
	struct rusage r1, r2;
	int result;

/*
	int sched_yield(void);

A  process  can  relinquish  the processor voluntarily without blocking by calling sched_yield.
The process will then be moved to the end of the queue for its static priority and a  new  pro-
cess gets to run.
Note:  If  the  current  process is the only process in the highest priority list at that time,
this process will continue to run after a call to sched_yield.
POSIX systems on which sched_yield is available define _POSIX_PRIORITY_SCHEDULING in <unistd.h>.
*/


#ifndef _POSIX_PRIORITY_SCHEDULING
	printf("sched_yield not supported in unistd.h\n");
	return(0);
#endif
	
/*
	struct timeval {
		time_t		tv_sec;	// seconds
		suseconds_t	tv_usec;	// microseconds
	};
*/

	// Idéia: tentar medir o tempo para troca de contexto e uma rodada inteira de escalonamento
	
	// int gettimeofday(struct timeval *tv, struct timezone *tz);
	gettimeofday(&inic,0);
	getrusage(RUSAGE_SELF, &r1);

	// while(!cond);	// busy wait, espera ocupada; gasta fatia de tempo
	// ou
	// while(!cond)
	// 	sched_yield();
	// ou
	//	sem_wait(semáforo); 	// bloqueia processo até condição; 
	//								// não volta à condição de pronto

	// if(sched_yield()==-1) {
	result=sched_yield();
	if(result==-1) {	// se está executando essa linha, significa que já saiu, 
							// outros possivelmente executaram uma rodada de fatias de 
							// tempo, e este retornor à execução
		perror("Falha em sched_yield");
		exit(0);
	}
	gettimeofday(&fim,0);
	getrusage(RUSAGE_SELF, &r2);

	printf("\n");
	printf("Início: %f\nFim: %f\n",toSeconds(inic),toSeconds(fim));
	printf("Elapsed time:%f sec\nUser time:%f sec\nSystem time:%f sec\n\n",
		toSeconds(fim)-toSeconds(inic),
		toSeconds(r2.ru_utime)-toSeconds(r1.ru_utime),
		toSeconds(r2.ru_stime)-toSeconds(r1.ru_stime));

	return 0;
}

