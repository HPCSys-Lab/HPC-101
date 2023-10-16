/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: atexit()
** Objetivo: experimentar funcao de saida para liberar estruturas, como os mecanismos
             de IPC, que nao sao liberados automaticamente quando o programa termina
*/

#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
 int atexit(void (*function)(void));
 
 The atexit() function registers the given function to be called at nor-
 mal program termination, either via exit(3) or via return from the pro-
 gram main().  Functions so registered are called in the reverse order
 of their registration; no arguments are passed.

 At least ATEXIT_MAX functions can be registered. This value is at least
 32. It can be obtained using sysconf(3).
*/      

#define ESPERA 5

void
the_end3(void)
{
  printf("the end.\n\n");
}
void
the_end2(void)
{
  printf("my only friend...\n");
  sleep(1);
}
void
the_end1(void)
{ 
  printf("my friend...\n");
  sleep(2);
}
void
the_end0(void)
{ 
  printf("\nThis is the end...\n");
  sleep(2);
}

int 
main(int argc, char *argv[])
{
  int espera;

  if(argc==1) {
    printf("Uso: %s tempo_de_espera\n",argv[0]);
    return(0);
  }
  if((espera=atoi(argv[1]))<=0)
    espera=ESPERA;

  // int atexit(void (*function)(void));
  if(atexit(the_end3)==-1) {
    perror("Erro em atexit");
    exit(EXIT_FAILURE);
  }
  if(atexit(the_end2)==-1) {
    perror("Erro em atexit");
    exit(EXIT_FAILURE);
  }
  if(atexit(the_end1)==-1) {
    perror("Erro em atexit");
    exit(EXIT_FAILURE);
  }
  if(atexit(the_end0)==-1) {
    perror("Erro em atexit");
    exit(EXIT_FAILURE);
  }

  sleep(espera);
  return(EXIT_SUCCESS);
}
