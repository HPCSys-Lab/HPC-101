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


void
produtor()
{
	// do {
		// produz item
		printf("Produzindo...\n");
		// espera espa�o em buffer
		// insere item: pode requerer exclus�o m�tua
		// sinaliza consumidor, indicando item dispon�vel
	// } while (cond);
}

void
consumidor()
{
	// do { 
		// espera item produzido
		printf("Pronto para consumir...\n");
		// consome item: pode requerer exclus�o m�tua
		// sinaliza produtor, indicando espa�o em buffer
	// } while (cond);
}


int
main()
{
	pid_t result;

	if((result=fork())==-1) {
		printf("Erro na execucao do fork\n"); 
		exit(0);
	}

	/* Se c�digo chegou aqui, h� 2 processos em execu��o. */
	/* result cont�m o resultado da chamada fork. Para o pai, que fez a chamada, 
	* result cont�m o pid do filho criado. Para o filho, valor de result � 0 */

	/* � importante lembrar que, fork copia �rea de c�digo do pai para o filho,
	 * mas �reas n�o s�o compartilhadas */

	if(result!=0) {  // pai
		produtor();
		// pai sempre deve esperar filho terminar para evitar zombies
		// wait(&status);
	} else {  // filho
		consumidor();
	}

	return(0);
}

