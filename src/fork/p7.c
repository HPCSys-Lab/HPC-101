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
	int result;

	// int execl(const char *path, const char *arg, ...);
  	// int execlp(const char *file, const char *arg, ...);
  	// int execle(const char *path, const char *arg, ..., char * const envp[]);
	// int execv(const char *path, char *const argv[]);
	// int execvp(const char *file, char *const argv[]);

	printf("%d vai executar o exec...\n",getpid());

	/* sobrepoe o processo atual com um novo processo */
	// execl("hello","&",NULL); /* & nao tem sentido com exec */
	// execlp("hello",NULL); /* execlp usa o path do sistema */
  	if((result=execl("./hello","hello",NULL))==-1)
		perror("Falha no exec..."); 

	// poderia apenas executar assim. Se esse codigo ainda existe, exec falhou!
  	// result=execl("./hello","hello",NULL);

	/* processo nao continua depois do exec! */
	printf("Erro na execucao do execl\n");

	return(0);
}

