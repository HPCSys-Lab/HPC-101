/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
*/

/*
** Programa : 
** Comando: getenv() e variável environ
** Objetivo: acesso às variáveis de ambiente
*/


#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* ENVIRON contém as variáveis de ambiente.  */
extern char** environ;
/*
	The  variable  environ  points  to an array of strings called the `environment'.  (This
	variable must be declared in the user program, but  is  declared  in  the  header  file
	<unistd.h>  in  case  the  header files came from libc4 or libc5, and in case they came
	from glibc and _GNU_SOURCE was defined.)  This array of strings is  made  available  to
	the  process by the exec(3) call that started the process.  By convention these strings
	have the form `name=value'. 
*/

int main ()
{
	char** var;
	char* user = getenv ("USER");

	for (var = environ; *var != NULL; ++var)
		printf ("%s\n", *var);

	if (user == NULL) 
		user = "Indefinido...";

	printf ("\nUSER: %s\n\n", user);

	return(0);
}
