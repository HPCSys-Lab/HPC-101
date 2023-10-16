/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : substituicao do codigo do processo com vfork e exec
** Comandos: vfork(), exec()
** Objetivo: execucao de um processo a partir de um outro em execucao. 
             exec() sobrepoe a imagem do processo em execucao
             com vfork, pai fica suspenso até que filho execute exec() ou exit()
				 Verificar que filho compartilha area de memoria do pai. Nao eh
				 recomendado compartilhar memoria assim!
*/

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int
main()
{
	pid_t result;
	int a=10;  // variavel para testar compartilhamento

/*
	pid_t vfork(void);

	(From SUSv2 / POSIX draft.)  The vfork() function has the same effect as fork(), except
	that the behaviour is undefined if the process created by vfork() either  modifies  any
	data  other  than a variable of type pid_t used to store the return value from vfork(),
	or returns from the function in which vfork() was called, or calls any  other  function
	before successfully calling _exit() or one of the exec() family of functions.

	vfork()  differs  from  fork()  in that the parent is suspended until the child makes a
	call to execve(2) or _exit(2).  The child shares all memory with its parent,  including
	the  stack,  until execve() is issued by the child.  The child must not return from the
	current function or call exit(), but may call _exit().

	Signal handlers are inherited, but not shared.  Signals to the parent arrive after  the
	child releases the parent's memory.
*/

	// pid_t vfork(void);
	result=vfork();

	if(result==-1) {
		perror("Erro em vfork");
		exit(0);
	}

	if(!result) {	// filho
		// dorme antes do exec para ver se pai espera!
		sleep(5);

		// altera variavel no filho. Na pratica, diferentemente do fork, 
		// neste caso, aponta para a propria area de memoria do pai...
		// vfork compartilha area de memoria do pai, para melhorar 
		// tempo de resposta, supondo que filho apenas vai usar exec()
		a+=5;

		/* sobrepoe o processo atual com um novo processo */
		result=execlp("hello","hello",NULL);

		perror("Erro em execl");
		exit(0);

	}else {	// pai
		printf("Pai retomou a execucao. a=%d\n",a);
	}

	return(0);
}

/*
BUGS
	It  is  rather  unfortunate that Linux revived this spectre from the past.  The BSD man
	page states: "This system call will be eliminated when proper system sharing mechanisms
	are implemented.  Users should not depend on the memory sharing semantics of vfork() as
	it will, in that case, be made synonymous to fork(2)."
	Details of the signal handling are obscure and differ between  systems.   The  BSD  man
	page  states:  "To  avoid a possible deadlock situation, processes that are children in
	the middle of a vfork() are never sent SIGTTOU or SIGTTIN signals;  rather,  output  or
	ioctls are allowed and input attempts result in an end-of-file indication."
*/
