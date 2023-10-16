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

	ssbuf.sem_num=num_sem;
	ssbuf.sem_op=-1;
	ssbuf.sem_flg=1;	// flag == 1 indica acesso bloqueante

	return(semop(semid,&ssbuf,1)!=-1);
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
	int semid,status,num_sem,val;
	union semun sem_un;
	pid_t pid;

	/* instala rotina para tratar SIGINT */
	// signal(SIGINT, int_hand);
	signal(SIGUSR1, int_hand);
	// signal(SIGINT, int_hand);

	num_sem=1;

	if((semid=semget(IPC_PRIVATE,num_sem,0744)) < 0) { 
		perror("Erro na alocacao do semaforo");
		exit(0);

	}else {
		/* atribuicao de valor inicial ao semaforo */ 
		init_sem(semid,num_sem,0);	

		printf("semid: %d\n",semid); fflush(stdout);

		if((pid=fork())<0) {
			perror("erro na execucao do fork");
			// liberar o semaforo
			exit(0);
		}

		if(!pid) { /* filho */

			do {  
				printf("%d esperando...\n",(int)getpid()); fflush(stdout);

				if((val=!lock(semid,0))) {
					printf("Falha na alocação do semáforo\n");
					break;
				}
			} while (1); // } while (val);

			printf("%d na secao critica\n",(int)getpid()); fflush(stdout);
			exit(0);

		} else { /* pai */

			printf("%d dormindo...\n",(int)getpid()); fflush(stdout);
			sleep(30);

			if(!unlock(semid,0)) 
				printf("Falha na liberação dos semáforo\n");
			
			printf("%d liberou semaforo...\n",(int)getpid()); fflush(stdout);
		}

		pid=wait(&status);
		printf("%d filho terminou ...\n",(int)getpid()); fflush(stdout);

		/* liberacao da estrutura */
		status=semctl(semid,0,IPC_RMID,sem_un);
	}

	return(0);
}


