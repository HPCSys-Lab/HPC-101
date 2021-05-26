#include <omp.h>
#include <stdio.h> 
int main(){
#pragma omp parallel 
{
		printf(" hello "); 
		printf(" world \n");
}

}
