/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Prof. Helio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : exemplo de produtor X consumidor usando processos
** Comando: 
** Objetivo:
*/

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


/* semáforos:
	mutex: valor inicial=1
	itens: valor inicial=0
	espacos: valor inicial=TAM
*/

void
produtor()
{
	// do {
		// produz item
		printf("Produzindo...\n");
		// espera espaço em buffer
		lock(espacos);
		lock(mutex);
		// insere item: pode requerer exclusão mútua
		unlock(mutex);
		// sinaliza consumidor, indicando item disponível
		unlock(itens);
	// } while (cond);
}

void
consumidor()
{
	// do { 
		// espera item produzido
		lock(itens);
		lock(mutex);
		printf("Pronto para consumir...\n");
		// consome item: pode requerer exclusão mútua
		unlock(mutex);
		// sinaliza produtor, indicando espaço em buffer
		unlock(espacos);
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

	if(result!=0) {  // pai
		produtor();
		// pai sempre deve esperar filho terminar para evitar zombies
		// wait(&status);
	} else {  // filho
		consumidor();
	}

	return(0);
}

