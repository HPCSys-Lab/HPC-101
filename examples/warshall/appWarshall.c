#include <stdio.h>
#include <time.h>
#include <assert.h>

#define QTD_ELEMENTOS 1024

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


void warshallCPU(int* A, int* F, unsigned n)
{
		for(int k = 0; k < n; k++){
			for(int lin = 0; lin < n; lin ++){
				for(int col = 0; col < n; col ++){
						if(A[k * n + col] == 1 && A[lin * n + k] == 1)
							F[lin * n + col] = 1;
				}
			}
		}
}


void processamentoCPU(int *A, unsigned n)
{
	int* F = (int*) malloc( sizeof(int) * n * n);

  double tempoGasto;
	clock_t start = clock();

	warshallCPU(A, F, n);

	clock_t stop = clock();

	tempoGasto = 1000 *  (stop - start) / (float) CLOCKS_PER_SEC;
	printf("Tempo de execução da CPU: %f ms\n", tempoGasto );

	free(F);
}

void mainWarshall()
{

	int byteNumber = QTD_ELEMENTOS * QTD_ELEMENTOS * sizeof(int);

	int *A = (int*) malloc(byteNumber);

	inicializaMatriz(A, QTD_ELEMENTOS);
	processamentoCPU(A, QTD_ELEMENTOS);

  int soma = 0;
  for (int i=0; i< QTD_ELEMENTOS; i++)
    for (int j=0; j< QTD_ELEMENTOS; j++)
      soma += A[i * QTD_ELEMENTOS + j];
  printf("Total de arestas: %d \n ",soma);
  
	free(A);
}

int main(void)
{
	mainWarshall();
	return 0;
}
