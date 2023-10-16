/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa 6: criacao e execucao de processos
** Comandos: fork(), system()
** Objetivo: disparar a execucao de um comando passando como parametro. 
             Observer quantas vezes a palavra "Fim" e' impressa.
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>


int
main()
{
  pid_t pid, pid2, status;
  int ret;

  if((pid=fork())<0) {
    printf("Erro na execucao do fork\n");
    exit(0);
  }
  if(pid==0) { /* filho */
    printf("Filho\n");
    /* system dispara shell para executar o comando passando como parametro */

    // ret=system("ls"); 
    ret=system("sleep 10"); 

    // ret=system("sleep 10 &"); 
    // ret=system("/bin/ps -ef | grep $USER");

    printf("System retornou %d\n",ret);
    /* processo continua normalmente depois do system */

  }else {
    pid2=wait(&status);
    printf("Pai\n");
  }

  printf("Fim\n");

  return(0);
}

