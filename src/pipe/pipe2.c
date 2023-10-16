/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Programação Paralela e Distribuída
** Prof. Helio Crestana Guardia
*/

/*
** Programa : comunicacao entre processos
** Comando: pipe()
** Objetivo: ilustrar a comunicacao bidirecional, utilizando 2 pipes
*/

#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define DATA1 "msg PAI para FILHO"
#define DATA2 "msg FILHO para PAI"
#define N_MSG 5

int
main()
{
  pid_t pid;
  int i;
  char buf[1024];
  // pipe[0]: read, pipe[1]: write
  int pipe1[2], // envio pai para filho
      pipe2[2]; // envio filho para pai

  if(pipe(pipe1) == -1) {
    perror("abertura do pipe");
    exit(0);
  }
  if(pipe(pipe2) == -1) {
    perror("abertura do pipe");
    exit(0);
  }
  if((pid=fork())==-1) {
    perror("fork");
    exit(0);
  }
  if(pid) {  // pai: transmite em pipe1, recebe em pipe2 
    close(pipe1[0]); // fecha leitura em pipe1 
    close(pipe2[1]); // fecha escrita em pipe2 

    for(i=0;i<N_MSG;i++) {  
      sleep(1);
      if(write(pipe1[1],DATA1,sizeof(DATA1))< 0)
        perror("Pai escrevendo mensagem");  

      if(read(pipe2[0],buf,1024) == -1)
        perror("Pai lendo mensagem");  
      else
        printf("Pai leu--> %s\n",buf);           
    } 

    close(pipe1[1]); // fecha escrita em pipe1
    close(pipe2[0]); // fecha leitura em pipe2 

  }else { // filho: recebe em pipe1, transmite em pipe2
    close(pipe1[1]); // fecha escrita em pipe1
    close(pipe2[0]); // fecha leitura em pipe2

    for(i=0;i<N_MSG;i++) { 
      sleep(1);
      if(read(pipe1[0],buf,1024) == -1)
        perror("lendo mensagem");
      else
        printf("Filho leu--> %s\n",buf);           

      if(write(pipe2[1],DATA2,sizeof(DATA2))< 0)
        perror("Filho escrevendo mensagem");  
    } 

    close(pipe1[0]); // fecha leitura em pipe1
    close(pipe2[1]); // fecha escrita em pipe2 
  }

  return 0;
}
