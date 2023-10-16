#include <string.h>
#include <stdio.h>

#define LEN 256

int
main(int argc, char **argv)
{
   char string[LEN]=" ";
   char *ptr;
   int i;

   if(argc==1) {
      printf("Uso: %s string_to_parse\n",argv[0]);
      return(0);
   }
   for(i=1;i<argc;i++)
      printf("argv[%d]: %s\n",i,argv[i]);

	// concatena parametros em uma unica string: atencao para o limite do vetor!
   for(i=1;i<argc;i++) {
      strcat(string,argv[i]);
      strcat(string," ");
   }

   printf("String: %s\n",string);

   ptr=strtok(string," ");
   do {
      printf("%s\n",ptr);
      ptr=strtok(NULL," ");
   }while(ptr);

   return(0);
}

