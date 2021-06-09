#include <stdio.h>
#include <time.h>
#include <assert.h>

#define QTD_ELEMENTOS 1024

void imprimeSoma(int *data, unsigned n)
{
    double soma = 0;
    for (int i=0; i < n; i++) {
				for (int j=0; j < n; j++){
					soma += data[i * n + j];		
				}	    
	  } 
    printf("A soma é %d\n",soma);
}

void inicializaMatriz(int *data, unsigned size)
{
  time_t t;
	srand((unsigned int) time(&t));
	for (int i=0; i<size; i++) {
     for (int j=0; j<size; j++) { 
		   //data[i * size + j] = (int)( rand() & 0xFF )/10.0f;
			 data[i * size + j] = ((int)rand() ) % 2;			 
     }  
	}
}


void warshallCPU(int* fechoMatriz, unsigned n)
{		
		for(int k = 0; k < n; k++){
			for(int i = 0; i < n; i++){
				for(int j = 0; j < n; j++){
						if(fechoMatriz[k * n + j] == 1 && fechoMatriz[i * n + k] == 1)	
							fechoMatriz[i * n + j] = 1;
				}			
			}					
		}
}

void processamentoCPU(int *A, unsigned n)
{
	int* F = (int*) malloc( sizeof(int) * n * n);
  
  memcpy(F, A, sizeof(int)*n*n);

  double tempoGasto;
	clock_t start = clock();

	warshallCPU(F, n);
	
	clock_t stop = clock();
	
	tempoGasto = (stop - start) / (float) CLOCKS_PER_SEC;
 
	printf("Tempo de execução da CPU: %f s\n", tempoGasto ); 
  imprimeSoma(F, n);

	free(F);
}


void mainWarshall()
{

	int byteNumber = QTD_ELEMENTOS * QTD_ELEMENTOS * sizeof(int);

	int *A = (int*) malloc(byteNumber);
	
	inicializaMatriz(A, QTD_ELEMENTOS);
	processamentoCPU(A, QTD_ELEMENTOS);

	free(A);	
}

int main(void)
{
	mainWarshall();
	return 0;
}

