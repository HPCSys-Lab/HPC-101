#include<stdlib.h>
#include <omp.h>
int main(){
  #pragma omp parallel {
     printf(" Olá mundo!\n");
  }

}
