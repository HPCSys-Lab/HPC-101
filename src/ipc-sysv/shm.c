/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : comunicacao entre processos usando memoria compartilhada
** Comandos: shmget(), shmat(), shmdt(), shmctl()
** Objetivo: criacao, associacao, uso e liberacao de uma area de memoria
             compartilhada. Pai altera valor e filho verifica conteudo
             de uma area compartilhada.
             Observar diversas formas de alocacao de memoria compartilhada,
             tambem utilizadas para semaforos e filas de mensagens.
*/

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_MAX 80

char *shm_buffer;  /* buffer compartilhado */

int
main()
{
  int shmid,status;
  struct shmid_ds shmds;
  pid_t pid;

	/// Cria uma area nova para o processo. 
	// Porem, nao impede que outros venham a se conectar (shmat()) a ela 
  if((shmid=shmget(IPC_PRIVATE,TAM_MAX,0644)) > -1) {

     printf("shmid: %d\n",shmid); fflush(stdout);

     /* associa ponteiro local `a shm */
     shm_buffer=(char *)shmat(shmid,0,0);

		// imprime endereço para ver qual posição o SO escolheu
		// printf("shm_buffer: %d (%d)(%d)\n",(int)shm_buffer,(int)(shm_buffer/4096),(int)(shm_buffer%4096)); 

	  // zera string
	  // shm_buffer[0]='\0';
     strcpy(shm_buffer,"");

     if((pid=fork())<0) {
       printf("Erro na criacao do processo filho\n");
       fflush(stdout);
     }else {
 
       /* filho fica imprimindo o conteudo da memoria compartilhada */
       if(!pid) {
         while(strncmp(shm_buffer,"fim",3)){
           sleep(3);
           printf("\nfilho leu: %s\n",shm_buffer);
			  // reimprime mensagem de leitura do pai apenas para indicar ao usuário... 
           printf("Novo valor: "); fflush(stdout);
         }
         /* desconecta da mem compart para que possa ser liberada pelo pai */
         status=shmdt((char *)shm_buffer);

         exit(0);

       /* pai le valor do console e atualiza memoria compartilhada */
       }else {
         do {
           printf("Novo valor: "); fflush(stdout);
           fgets(shm_buffer,TAM_MAX,stdin);
         } while(strncmp(shm_buffer,"fim",3));

         /* espera filho terminar para poder liberar a memoria compartilhada */
         pid=wait(&status);

         status=shmdt((char *)shm_buffer);
         status=shmctl(shmid,IPC_RMID,&shmds); 
       }
     }
  }else
     printf("Erro na alocacao da memoria compartilhada\n");

  return 0;
}


