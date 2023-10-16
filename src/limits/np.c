/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : criacao de processos

** Comando: fork()
** Objetivo: teste do número máximo permitido - sujeito a restricoes do  "ulimit -u"
			 2 formas de consulta são providas: sysconf e getrlimit
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/resource.h>

int
main(int argc, char **argv)
{
	int i, num;
	pid_t pid;
	struct rlimit rlim;

	if(argc>1)
		num=atoi(argv[1]);
	else 
		num=(int)sysconf(_SC_CHILD_MAX) + 1;

/* 
	long sysconf(int name);
	CHILD_MAX - _SC_CHILD_MAX
	The max number of simultaneous processes per user id.  
	Must  not be less than _POSIX_CHILD_MAX (25). 
*/

/* 	
	int getrlimit(int resource, struct rlimit *rlim);
	RLIMIT_NPROC: The maximum number of processes that can be created for the real
		user ID of the calling process. Upon encountering this limit, fork() fails with the error EAGAIN.
*/


	// definindo limites com getrlimit
	if(getrlimit(RLIMIT_NPROC,&rlim)==-1) {
		perror("Erro executando getrlimit");
		exit(0);
	}
	num=rlim.rlim_max;
	if(num==-1) {
		printf("Número de processos ilimitado: RLIMIT_NPROC=-1\n"
				"Ajustando valor de teste para 100\n");
		num=100;
	}

	printf("Criando %d processos / _SC_CHILD_MAX: %d. Limits: %d(s) %d(h)\n",
	       num,(int)sysconf(_SC_CHILD_MAX),(int)rlim.rlim_cur,(int)rlim.rlim_max);

	for(i=0; i < num; i++)
		if((pid=fork())==-1) {
			printf("Erro criando processo %d: %s\n",i,strerror(errno)); 
			exit(EXIT_FAILURE);
		}else
			if(!pid) {
				sleep (20);
				break;
			}

	return(EXIT_SUCCESS);
}

