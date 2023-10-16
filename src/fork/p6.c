/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 5: criacao e manipulacao de processos
** Comandos: fork(), getpid(), getppid(), kill()
** Objetivo: apresentar comando para enviar um "sinal" para um processo
*/

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#define ESPERA 5


int
main(int argc, char **argv)
{
  pid_t pid;
  int espera;

  if(argc>1){
    espera=atoi(argv[1]);
    if(espera<0 || espera>ESPERA) 
      espera=ESPERA;
  }else
    espera=ESPERA;

  if((pid=fork())<0) {
    printf("Erro na execucao do fork\n");
    exit(0);
  }
  if(pid==0) { /* filho */
    printf("Filho: pid %d, pai %d\n",(int)getpid(),(int)getppid()); 
    /* loop infinito, esperando sem "morto" pelo pai */
    for(;;) {
		printf(".");
		fflush(stdout);
		sleep(1);
	 }

	}else { /* pai */
		printf("Pai:%d, filho %d avo %d\n",(int)getpid(),(int)pid,(int)getppid());

		// dorme
		sleep(espera);

		// para processo filho
		if(kill(pid,SIGSTOP)==-1) {
			perror("Erro enviando sinal de parada para filho");
			exit(0);
		}
		// dorme
		sleep(espera);

		// acorda processo filho
		if(kill(pid,SIGCONT)==-1) {
			perror("Erro enviando sinal de reinicio para filho");
			exit(0);
		}
		// dorme
		sleep(espera);

		printf("%d matando proc %d...\n",(int)getpid(),(int)pid);
		if(kill(pid,SIGKILL)<0)
			perror("matando proc filho");
	}
	printf("Tchau!\n");

  return(0);
}
