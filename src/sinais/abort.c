
/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa 8: tratamento de sinais (signals).
** Comando: assert() + SIGABRT
** Objetivo: instalar rotina de tratamento e verificar o comportamento da rotina assert + sinal SIGABRT
             Observar que mesmo instalando rotina para tratar sinal, ação default é restaurada depois
             da chamada da rotina atual. Com isso, programa sempre é abortado.
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <assert.h>



/*
 void abort(void);

 The  abort()  first  unblocks  the SIGABRT signal, and then raises that signal for the calling
 process.  This results in the abnormal termination of the process unless the SIGABRT signal is
 caught and the signal handler does not return (see longjmp(3)).
 
 If the abort() function causes process termination, all open streams are closed and flushed.
 
 If  the  SIGABRT  signal is ignored, or caught by a handler that returns, the abort() function
 will still terminate the process.  It does this by restoring the default disposition for SIGA-
 BRT and then raising the signal for a second time.
*/


/*
 void assert(scalar expression);
 assert - abort the program if assertion is false

 If the macro NDEBUG was defined at the moment <assert.h> was last included, the macro assert()
 generates no code, and hence does nothing at all.  Otherwise, the  macro  assert()  prints  an
 error  message  to standard error and terminates the program by calling abort(3) if expression
 is false (i.e., compares equal to zero).

 The purpose of this macro is to help the programmer find bugs in  his  program. 
*/


void
sig_hand(int signo)
{
  printf("\nSignal %d recebido...\n\n",signo);
}

int 
main(int argc, char ** argv)
{

	if(signal(SIGABRT, sig_hand)==SIG_ERR) {
		perror("Erro capturando tratamento do sinal");
		return(0);
	}

	// Condição de teste do assert: verifica se programa foi chamado com argumentos
	assert(argc > 1);

	printf("Programa iniciado com argumentos\n");

	return(0);
}

