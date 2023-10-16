/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 7: criacao e execucao de processos
** Comandos: fork(), exec()
** Objetivo: execucao de um processo a partir de um outro em execucao. 
             exec() sobrepoe a imagem do processo em execucao
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int
main()
{
  pid_t pid;

  if((pid=fork())<0) {
    printf("Erro na execucao do fork\n");
    exit(0);
  }
  if(pid==0) { /* filho */
    printf("Filho: %d\n",(int)getpid());

    /* sobrepoe o processo atual com um novo processo */
    // execl("hello","&",NULL); /* & nao tem sentido com exec */
    // execlp("hello","",NULL);
    execlp("./hello","hello",NULL); /* execlp usa o path do sistema */

    /* processo nao continua depois do exec! */
    printf("Filho: erro na execucao do execl\n");

  }else {
    printf("Pai\n");
    // pai pode esperar pelo filho
    // wait(...);
  } 

  printf("Fim\n"); fflush(stdout);

  return(0);
}

