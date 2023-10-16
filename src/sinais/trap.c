/* 
** Universidade Federal de Sao Carlos
** Departamento de Computacao
** Sistemas Operacionais 2
** Prof. Helio Crestana Guardia
*/

/*
** Programa : tratamento de sinais - Traps (SIGILL, SIGFPE, SIGSEGV)
** Comando: signal()
** Objetivo: verificar o tratamento de sinais (TRAPS) gerados em decorrência de erros na
**           execução do processo
*/

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define LIM 10
#define MAX 1024
#define MAXINT 32767

int _estourou=0;

void
div_hand(int signo)
{
	printf("Sinal %d: divisão por 0 ou overflow...\n\n",signo);
	// _estourou=1;
	exit(0);
}

void
seg_hand(int signo)
{
	printf("Sinal %d: invalid memory reference...\n\n",signo);
	exit(0);
}

void
bus_hand(int signo)
{
	printf("Sinal %d: bus erros (bad memory access)...\n\n",signo);
	exit(0);
}

void
handler(int signo)
{
	printf("Sinal %d: \n\n",signo);
	exit(0);
}


void
fill_stack()
{
	float mat[MAX][MAX];
	fill_stack();
}

void
div_zero()
{
	float num_f; 
	int num_i; 
	int divisor;

	printf("\nVou tentar dividir por 0...\n"); fflush(stdout);
	num_i=(int)getpid();
	divisor=0;
	num_i=num_i/divisor;
	// num_i=num_i/0;
	num_f=num_f/0;
	printf("Dividiu? num_i: %d  num_f: %f\n",num_i,num_f); fflush(stdout);
/*
	if(_estourou){
		printf("Saindo...\n");
		exit(0);
	}
*/
}

void
overflow()
{
	int i, num_i; 

	printf("Vou tentar gerar overflow...\n"); fflush(stdout);
	num_i=600000000*1024;

	// problema: como desfazer o overflow???
	printf("Estourou? num = %d\n",num_i); fflush(stdout);

	num_i=99999999;
	for(i=0;i<10;i++) {
		num_i=num_i*num_i;
		printf("%d\n",num_i);
	}
	printf("Estourou? num_i = %d\n",num_i); fflush(stdout);
}

void
illegal_ref()
{
	int i;
	int vet[LIM];

	printf("\nVou tentar exceder limite de vetor...\n");

	for(i=0;i<MAXINT;i++) {
		// printf("%i ",i);
		vet[i]=i;
	}
	printf("Excedeu? i (%d): %d\n\n",LIM,i);
}


void
mostra_uso()
{
  printf("\nUso: trap [-s|-r host] [-t|-u] [-p port] [-l pkt_len] [-n num_pkts] [-c] [-v]\n\n");
  printf("-z: testa divisão por 0\n");
  printf("-m: testa referência inválida à memória\n");
  printf("-o: teste overflow\n");
  printf("-v: verbose\n\n");
}

int 
main(int argc, char **argv)
{
	int i;

	// tratar entrada de dados, testando apenas um trap de cada vez
	if (argc<2) {
		mostra_uso();
		exit(0);
	}

	// instala rotina padrão para tratar todos os sinais
	for(i=1;i<=32;i++)
		if(signal(i, handler)==SIG_ERR) {
			// espere-se erro em 9 e 19
			// printf("Erro capturando sinal %d: %s\n",strerror(errno));
			// return(0);
		}

	// Sinais associados a traps
	// SIGILL    4  Core  Illegal Instruction
	// SIGFPE    8  Core  Floating point exception
	// SIGSEGV  11  Core  Invalid memory reference
	// SIGBUS    7  Core  Bus error (bad memory access)
	// SIGTRAP   5  Core  Trace/breakpoint trap
	// SIGXFSZ  25  Core  File size limit exceeded (4.2BSD)

	// instala rotinas para tratar os sinais associados a traps

	// instala rotina para tratar Floating Point Exception
	if(signal(SIGFPE, div_hand)==SIG_ERR) {
		printf("Erro capturando sinal SIGFPE: %s\n",strerror(errno));
		return(0);
	}
	// instala rotina para tratar Invalid Memory Reference
	if(signal(SIGSEGV, seg_hand)==SIG_ERR) {
		printf("Erro capturando sinal SIGSEGV: %s\n",strerror(errno));
		return(0);
	}
	// instala rotina para tratar limite de tamanho de arquivo
	if(signal(SIGXFSZ, seg_hand)==SIG_ERR) {
		printf("Erro capturando sinal SIGXFSZ: %s\n",strerror(errno));
		return(0);
	}

	for(i=0;i<argc;i++) {
		if(!strcmp(argv[i],"-z")) 
			div_zero();
		if(!strcmp(argv[i],"-m"))
			illegal_ref();
		if(!strcmp(argv[i],"-v"))
			mostra_uso();
		if(!strcmp(argv[i],"-o"))
			overflow();
	}

	// estourando o limite da pilha 
	fill_stack();

	return(0);
}

