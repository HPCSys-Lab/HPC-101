#include<unistd.h>

int
main()
{
	char c;
	int nbytes;

	do {
		nbytes=read(0,&c,1);
	} while(nbytes!=-1);

	return(0);
}
