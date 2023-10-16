/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionas 2
*/

/*
** Programa 8: substituicao do codigo do processo com exec
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
  /* sobrepoe o processo atual com um novo processo */
  // execl("hello","&",NULL); /* & nao tem sentido com exec */
  // execl("./hello","",NULL);
  execlp("hello","hello",NULL); /* execlp usa o path do sistema */

  /* processo nao continua depois do exec! */
  printf("Erro na execucao do execl\n");

  printf("Fim\n");

  return(0);
}

