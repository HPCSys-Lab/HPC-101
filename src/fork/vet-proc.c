/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : exemplo de produtor X consumidor usando processos
** Comando: fork()
** Objetivo: observar que o mesmo codigo e' executado pelos processos pai e
             filho, que diferem apenas pelo valor de retorno do comando fork()  
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NPROC 5

// criar exemplo com vetor de fun��es...

void
funcao(int i)
{
	// do {
		// manipula dados em fun��o do par�metro i
	// } while (cond);

	// switch (i) {}
	// case ()
	// ...
	// }

	// exit(0);
}


int
main()
{
	int i;
	pid_t filho[NPROC];

	for(i=0;i<NPROC;i++) {
		filho[i]=fork();
		if(filho[i]==-1) {
			perror("Erro na execucao do fork"); 
			// termina_e_sai();	
			// trata processos que j� foram iniciados: 
			// for(j=0;j<i;j++)
			// 	kill(filho[j],SIGKILL);
			exit(0);
		}
		if(!filho[i]) {	// filho
			break;			// interrompe la�o
			// ou
			// funcao(i);	// chama fun��o de acordo com seu �ndice l�gico 
			// exit(0);
		}
	}
	// pai n�o chama fun��o; espera filhos terminarem
	if(i<NPROC) {	// todos os filhos: pai sai do loop com i=NPROC
		funcao(i);	// chama fun��o de acordo com seu �ndice l�gico 
		exit(0);
	}

	// pai deve esperar os filhos terminarem, liberar estruturas, ...
	// for(i=0;i<NPROC;i++)
	// 	wait(&status);
	//
	return(0);
}

