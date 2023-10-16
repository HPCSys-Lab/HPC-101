/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 4: criacao de processos
** Comandos: fork(), getpid(), getppid()
** Objetivo: ilustrar identificadores de processos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

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
    pid=getpid();
    printf("Filho: pid %d, pai %d\n",(int)pid,(int)getppid()); 
    sleep(espera);

  }else { /* pai */

    printf("Pai: %d, filho %d avo %d\n",(int)getpid(),(int)pid,(int)getppid());
    /* sem espera, pai termina antes e ppid do filho passa a ser 1 (init) */
    /* com espera so' no pai, filho termina antes e vira um zombie */
    /* sleep(espera); */
  }
  printf("Tchau!\n");

  return(0);
}
