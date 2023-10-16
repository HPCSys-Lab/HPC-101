/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Sistemas Operacionais 2
*/

/*
** Programa : 
** Comando: ilustrar o fim de programa / retorno da função main
** Objetivo: 
*/

#include <stdlib.h>

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
  return(EXIT_SUCCESS);
}
