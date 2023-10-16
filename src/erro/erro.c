/* 
** Universidade Federal de S�o Carlos
** Departamento de Computa��o
** Prof. H�lio Crestana Guardia
** Programa��o Paralela e Distribu�da
*/

/*
** Programa : 
** Comandos: perror, strerror, strerror_r
** Objetivo: Examinar as mesagens de erro existentes
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX 128

int
main(int argc, char **argv)
{
	int i;
	char err_msg[MAX];

/*
	The externals sys_nerr and sys_errlist are defined by glibc, but in <stdio.h>.
	extern const char *sys_errlist[];
	extern int sys_nerr;
	extern int errno;
*/

/* 
	char *strerror(int errnum);
	int strerror_r(int errnum, char *buf, size_t n);

	The  strerror()  function  returns  a  string describing the error code
	passed in the argument errnum, possibly using the LC_MESSAGES  part  of
	the  current  locale  to  select the appropriate language.

	The strerror_r() function is similar to strerror(), but is thread safe.
	It returns the string in the user-supplied buffer buf of length n.

	The  strerror()  function  returns  the  appropriate  error description
	string, or an unknown error message if the error code is unknown.   The
	value  of  errno  is not changed for a successful call, and is set to a
	nonzero value upon error.  The strerror_r() function returns 0 on  suc-
	cess and -1 on failure, setting errno.

	EINVAL The value of errnum is not a valid error number.
	ERANGE Insufficient storage was supplied to contain the error description string.


	void perror(const char *s);

	The  routine  perror() produces a message on the standard error out-
 	put, describing the last error encountered during a call to a system
	or  library  function.  First the argument string s is printed, 
	followed by a colon and a blank.  Then the message and a new-line.

	To  be  of  most use, the argument string should include the name of
	the function that incurred the error.  The  error  number  is  taken
	from the external variable errno, which is set when errors occur but
	not cleared when non-erroneous calls are made.

	The global error list sys_errlist[] indexed by errno can be used  to
	obtain  the  error message without the newline.  The largest message
	number provided in the  table  is  sys_nerr  -1.   Be  careful  when
	directly  accessing  this list because new error values may not have
	been added to sys_errlist[].
*/


/* 
	errno � ajustado automaticamente com o status da �ltima chamada 
	de sistema. Aqui, estamos for�ando o valor s� para teste... 
*/

	if(argc<2) {
		printf("Uso: %s n�m_erro\n",argv[0]);
		exit(0);
	}

	i=atoi(argv[1]);
	if( i<0 || i>sys_nerr) {
		printf("Atualmente, s�o %d mensagens de erro definidas.\n",sys_nerr);
		exit(0);
	}
	// Exibindo diretamente o conte�do do vetor de mensagens de erro
	// N�o recomendado, tornando o programa n�o port�vel.
	fprintf(stderr,"%s\n",sys_errlist[i]);

	// For�a valor em errno, apenas para mostrar a mensagem correspondente. 
	// Na pr�tica, valor adequado vai estar ajustado automaticamente com o 
	// status da chamada de sistem mais recente realizada
	errno=i;
	perror("");

	// Outra forma de exibir a mensagem de erro: strerror()
	fprintf(stderr,"%s\n",strerror(i));

	// Outra forma de exibir a mensagem de erro: strerror_r()
	// vers�o reentrante (thread safe) de strerror
	strerror_r(i,err_msg,MAX);
	fprintf(stderr,"%s\n",err_msg);

	return(0);
}

