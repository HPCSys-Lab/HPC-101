/* 
** Universidade Federal de Sao Carlos
** Departamento de Computa��o
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : sincroniza��o entre processos usando sem�foros
** Comandos: semget(), semop(), semctl()
** Objetivo: Verificar a sintaxe dos comandos e o significado dos par�metros.
             Discutir a implementa��o de primitivas para bloqueio e libera��o
	     do sem�foros (lock() e unlock()).
             Exemplo do uso de sem�foros para exclus�o m�tua
*/

#define _GNU_SOURCE 

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
 

#define SEMKEY 75

#define N_VEZES 3

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
  /* a uni�o semun � definida pela inclus�o de <sys/sem.h> */
#else
  /* de acordo com X/OPEN n�s temos que a definir n�s mesmos */
  union semun {
    int val;                    /* valor para SETVAL */
     struct semid_ds *buf;       /* buffer para IPC_STAT, IPC_SET */
     unsigned short int *array;  /* vetor para GETALL, SETALL */
     struct seminfo *__buf;      /* buffer para IPC_INFO */
  };
#endif


/* Estruturas de dados (definidas em sys/sem.h)
struct sembuf {
  ushort sem_num;
  short  sem_op;
  short  sem_flg;
}
union semun {
  int val;                    // value for SETVAL
  struct semid_ds *buf;       // buffer for IPC_STAT, IPC_SET
  unsigned short int *array;  // array for GETALL, SETALL
  struct seminfo *__buf;      // buffer for IPC_INFO
};
struct semid_ds {
  struct ipc_perm sem_perm;            // permissions .. see ipc.h 
  __kernel_time_t sem_otime;           // last semop time 
  __kernel_time_t sem_ctime;           // last change time 
  struct sem      *sem_base;           // ptr to first semaphore in array 
  struct sem_queue *sem_pending;       // pending operations to be processed 
  struct sem_queue **sem_pending_last; // last pending operation 
  struct sem_undo *undo;               // undo requests on this array 
  unsigned short  sem_nsems;           // no. of semaphores in array 
}
*/

/* prototipos */
/* int semctl (int semid, int semnum, int cmd, union semun arg) */
/* int semop (int semid, struct sembuf *sops, unsigned nsops) */
/* int semget (key_t key, int nsems, int semflg) */


int
main()
{
	int semid,status,num_sem,val,i,num_proc;
	struct sembuf ssbuf;
	union semun sem_un;
	pid_t pid, pai;
	struct seminfo ssinfo;

	num_sem=1;

	// Cria um sem�foro novo, sem impedir que outros venham conectar-se a ele 
	// if((semid=semget(IPC_PRIVATE,count,0600)) == -1) { 

	// Cria sem�foro com chave SEMKEY, se nao existir. Retorna ID se j� existir e 
	// houver permiss�es de leitura e escrita 
	// if((semid=semget(SEMKEY,count,0600|IPC_CREAT)) == -1) { 
	
	// flag IPC_EXCL causa erro, caso um sem�foro com mesma chave j� exista 
	// if((semid=semget(SEMKEY,count,0600|IPC_CREAT|IPC_EXCL)) == -1) { 

	// if((semid=semget(SEMKEY,num_sem,0644|IPC_CREAT)) == -1) { 
	if((semid=semget(IPC_PRIVATE,num_sem,0644)) == -1) { 
		perror("Erro na aloca��o do sem�foro");
		exit(0);

	}else {

		/* atribui��o de valor inicial ao sem�foro */ 
		sem_un.val = 1;
		status = semctl(semid,0,SETVAL,sem_un);

		printf("\nSemid: %d\n",semid); fflush(stdout);

		/* obt�m informa��es do sem�foro e de mecanismos de IPC */
	/*
		semctl - semaphore control operations
		int semctl(int semid, int semnum, int cmd, ...);
		...	
		IPC_INFO (Linux specific)
			Returns  information  about  system-wide semaphore limits and parameters in the
			structure pointed to by arg.__buf.  This structure is of type seminfo,  defined
			in <sys/sem.h> if the _GNU_SOURCE feature test macro is defined:

		struct  seminfo {
			int semmap;  // # of entries in semaphore map; unused 
			int semmni;  // Max. # of semaphore sets 
			int semmns;  // Max. # of semaphores in all semaphore sets 
			int semmnu;  // System-wide max. # of undo structures; unused 
			int semmsl;  // Max. # of semaphores in a set 
			int semopm;  // Max. # of operations for semop() 
			int semume;  // Max. # of undo entries per process; unused 
			int semusz;  // size of struct sem_undo 
			int semvmx;  // Maximum semaphore value 
			int semaem;  // Max. value that can be recorded for semaphore adjustment (SEM_UNDO) 
		};

		The semmsl, semmns, semopm, and semmni settings can be changed via
			/proc/sys/kernel/sem; see proc(5) for details.
	*/

		status=semctl(semid,0,IPC_INFO,&ssinfo);
		printf("\nNum of entries in semaphore map: %d\n"
				"Max. # of semaphore sets: %d\n"
				"Max. # of semaphores in all semaphore sets: %d\n"
				"System-wide max. # of undo structures; unused: %d\n"
				"Max. # of semaphores in a set: %d\n"
				"Max. # of operations for semop(): %d\n"
				"Max. # of undo entries per process; unused: %d\n"
				"size of struct sem_undo: %d\n"
				"Maximum semaphore value: %d\n"
				"Max. value that can be recorded for semaphore adjustment (SEM_UNDO) : %d\n\n",
			ssinfo.semmap, ssinfo.semmni, ssinfo.semmns, ssinfo.semmnu, 
			ssinfo.semmsl, ssinfo.semopm, ssinfo.semume, ssinfo.semusz, 
			ssinfo.semvmx, ssinfo.semaem);

		// semctl - semaphore control operations
		// int semctl(int semid, int semnum, int cmd, ...);
		// ...
		// GETNCNT	
		// The system call returns the value of semncnt (i.e., the  number  of  pro-
		// cesses  waiting for the value of this semaphore to increase) for the sem-
		// num-th semaphore of the set (i.e. the number of processes waiting for  an
		// increase  of semval for the semnum-th semaphore of the set).  The calling
		// process must have read permission on the semaphore set.

		for(i=0;i<5;i++) {
			if((pai=fork())==-1) {
				perror("erro na execu��o do fork");
				exit(0);
			}
			if(!pai)
				break;
		}
/*
		if((pai=fork())==-1) {
			perror("erro na execu��o do fork");
			// liberar sem�foro j� aloado...
			exit(0);
		}
*/

		/* atualiza��o do pid */
		pid=getpid();

		/* pai e filho executam o mesmo codigo */

		for(i=0;i<N_VEZES;i++) {
  
			val = semctl(semid,0,GETVAL);
			num_proc = semctl(semid,0,GETNCNT);
			printf("%d esperando. Sem_val: %d. %d processo(s) na fila.\n",(int)pid,val,num_proc); fflush(stdout);

			/* tenta alocar o sem�foro: opera��o l�gica lock ou wait */     
			ssbuf.sem_num=0;
			ssbuf.sem_op=-1;
			ssbuf.sem_flg=1; /* flag != IPC_NOWAIT && != SEM_UNDO indica op bloqueante */
			val=semop(semid,&ssbuf,1);

			// se passou pelo sem�foro, est� dentro da regi�o cr�tica
			// dorme um pouquinho, simulando uso da regi�o cr�tica
			sleep(2);

			val = semctl(semid,0,GETVAL);
			num_proc = semctl(semid,0,GETNCNT);
			printf("%d na se��o cr�tica. Sem_val: %d. %d processo(s) na fila.\n",(int)pid,val,num_proc); fflush(stdout);

			sleep(2);

			/* libera sem�foro: opera��o l�gica unlock ou signal */ 
			ssbuf.sem_num=0;
			ssbuf.sem_op=1;
			ssbuf.sem_flg=1; /* flag != IPC_NOWAIT && != SEM_UNDO indica op bloqueante */
			val=semop(semid,&ssbuf,1);

			val = semctl(semid,0,GETVAL);
			num_proc = semctl(semid,0,GETNCNT);
			printf("%d liberou se��o cr�tica. Sem_val: %d. %d processo(s) na fila.\n",(int)pid,val,num_proc); fflush(stdout);

			// sleep(1);

			/*
			if(sched_yield()<0)
				perror("Erro liberando processador");
			*/
		}

		// libera��o da estrutura: apenas 1 processo deve liberar
		if(pai) {
			// espera filho terminar e libera sem�foros
			pid=wait(&status);
			status=semctl(semid,0,IPC_RMID,sem_un);
		}
	}

	return(0);
}



