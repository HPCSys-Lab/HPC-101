/*
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : operacao do semaforo no tratamento de sinais
** Comando: 
** Objetivo: 
             Criar uma operacao de bloqueio no semaforo imune 
             ao recebimento de sinais.
*/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
  /* a união semun é definida pela inclusão de <sys/sem.h> */
#else
  /* de acordo com X/OPEN nós temos que a definir nós mesmos */
  union semun {
    int val;                    /* valor para SETVAL */
     struct semid_ds *buf;       /* buffer para IPC_STAT, IPC_SET */
     unsigned short int *array;  /* vetor para GETALL, SETALL */
     struct seminfo *__buf;      /* buffer para IPC_INFO */
  };
#endif

void
int_hand(int signo)
{
  printf("(%d) Signal: %d\n",getpid(),signo);
}

int
lock(int semid, int num_sem)
{
	struct sembuf ssbuf;
	int result;
	union semun sem_un;

	ssbuf.sem_num=num_sem;
	ssbuf.sem_op=-1;
	ssbuf.sem_flg=1;	// flag == 1 indica acesso bloqueante

	// reinsere processo na fila do semaforo em caso de sinal
	// problema: outro processo na fila vai passar na frente...
	do {
		if((result=semctl(semid,0,GETVAL,sem_un))==-1) {
			perror("Erro em getval");
		}else {
			// printf("Semval: %d\n",sem_un.val);
			printf("Semval: %d\n",result);
		}
		result=semop(semid,&ssbuf,1);
	} while (errno==EINTR);

	return(result);
}

int
unlock(int semid, int num_sem)
{
	struct sembuf ssbuf;

	ssbuf.sem_num=num_sem;
	ssbuf.sem_op=1;
	ssbuf.sem_flg=1;	// flag == 1 indica acesso bloqueante

	return(semop(semid,&ssbuf,1)!=-1);
}

int
init_sem(int semid, int num_sem, int val)
{
	union semun sem_un;
	
	sem_un.val = val;

	return(semctl(semid,0,SETVAL,sem_un)!=-1);
}


int
main()
{
	int semid,status,num_sem;
	union semun sem_un;

	/* instala rotina para tratar SIGINT */
	signal(SIGINT, int_hand);
	// signal(SIGUSR1, int_hand);

	num_sem=1;

	if((semid=semget(IPC_PRIVATE,num_sem,0744)) < 0) { 
		perror("Erro na alocacao do semaforo");
		exit(0);

	}else {
		/* atribuicao de valor inicial ao semaforo */ 
		if(init_sem(semid,num_sem,0)==-1) {
			perror("Falha na atribuicao de valor ao semaforo");
			// libera semaforo
			exit(0);
		}

		printf("semid: %d\n",semid); fflush(stdout);

		printf("%d esperando...\n",(int)getpid()); fflush(stdout);

		if(!lock(semid,0)) {
			perror("Falha na alocação do semáforo");
		}

		/* liberacao da estrutura */
		status=semctl(semid,0,IPC_RMID,sem_un);
	}

	return(0);
}


