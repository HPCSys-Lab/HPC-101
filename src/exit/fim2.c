/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: 
** Objetivo: Ilustrar o tratamento da condição de saída de um processo filho
*/

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
 As required by the ISO C standard, using return  from  main()  has  the
 same  behavior  (other  than  with respect to language scope issues) as
 calling exit() with the returned value.  Reaching the end of the main()
 function has the same behavior as calling exit(0).
 
 A value of zero (or EXIT_SUCCESS, which is required to be zero) for the
 argument status conventionally indicates successful  termination.  This
 corresponds  to the specification for exit() in the ISO C standard. The
 convention is followed by utilities such as make  and  various  shells,
 which interpret a zero status from a child process as success. For this
 reason, applications should not call exit(0) or _exit(0) when they ter-
 minate unsuccessfully; for example, in signal-catching functions.
*/                                                                                                   
int 
main()
{
	pid_t pid;
	int status;
	int fim;

	if((pid=fork())==-1) {
		perror("Erro no fork");
		// Embora a chamada falhou, não houve um erro na execução do programa: 
		// não está saindo por ter recebido um sinal, e.g.
		exit(EXIT_SUCCESS);
	}
	if(pid==0) {	// filho
		sleep(10);
		// exit(EXIT_SUCCESS);
		// return(EXIT_SUCCESS);
		exit(0);

	}else {	// pai
	/*
   WIFEXITED(status): returns true if the child terminated normally, that is, by calling 
		exit(3) or _exit(2), or by returning from main().
   WEXITSTATUS(status): returns the exit status of the child. This consists of the least
      significant 8 bits  of  the  status  argument  that  the child specified in a call
      to exit() or _exit() or as the argument for a return statement in main(). This macro
      should only be employed if WIFEXITED returned true.
   WIFSIGNALED(status): returns true if the child process was terminated by a signal.
   WTERMSIG(status): returns the number of the signal that caused the child process to
      terminate. This macro should only be employed if WIFSIGNALED returned true.
   WCOREDUMP(status): returns true if the child produced a core dump. This macro should
      only be employed if WIFSIGNALED returned true. This macro is not specified in
      POSIX.1-2001 and is not  available  on  some  Unix  implementations  (e.g.,  AIX,
      SunOS).  Only use this enclosed in #ifdef WCOREDUMP ... #endif.
   WIFSTOPPED(status): returns true if the child process was stopped by delivery of a
      signal; this is only possible if the call was done using WUNTRACED or when the
      child is being traced (see ptrace(2)).
	*/
		fim=0;
		do {
			printf("Pai esperando filho %d\n",pid); fflush(stdout);

			pid=wait(&status);
	
			if(WIFEXITED(status)) {	
				printf("Filho terminou com status %d\n",WEXITSTATUS(status));
				fim=1;
			}
	
			if(WIFSIGNALED(status)) {
				printf("Filho terminou terminou com o recebimento do sinal: %d\n", WTERMSIG(status));
				fim=1;
	
				if(WCOREDUMP(status)) 
					printf("Filho terminou terminou gerando core dump...\n");
			}	
			if(WIFSTOPPED(status)) 
				printf("Filho foi parado (stopped) com o recebimento de um sinal...\n");

		} while(!fim);
	} 

	return(EXIT_SUCCESS);
}

