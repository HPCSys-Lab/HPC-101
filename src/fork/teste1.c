#include <stdio.h> 
#include <unistd.h>
int main()
{

	printf("1 - %d\n",(int) getpid());

	fork();

	printf("2 - %d\n",(int) getpid());

	fork();

	printf("3 - %d\n",(int) getpid());

	fork();

	printf("4 - %d\n",(int) getpid());


	return 0;

}
