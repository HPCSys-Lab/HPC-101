#include <stdio.h>
#include <time.h>
#include <assert.h>

#define QTD_ELEMENTOS 1024
#define NUM_THREAD_BLOCK_Y 32
#define NUM_THREAD_BLOCK_X 32

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

__global__ 
void warshallKernelShared_Principal(int *F, int k, unsigned n){
    
  //Variáveis declaradas nos registradores - visíveis somente para o thread  
  int j = blockIdx.x * blockDim.x + threadIdx.x;
  int i = blockIdx.y * blockDim.y + threadIdx.y;
 
  //Variáveis declaradas nos registradores - visíveis somente para o thread  
  int tidX;
  int tidY;

  //Variável compartilhada - visível somente para os threads do bloco
  __shared__ int ladrilhoPrincipal[NUM_THREAD_BLOCK_Y][NUM_THREAD_BLOCK_X];  

  //Obtém os índices dos threads
  tidX = k * blockDim.x + threadIdx.x;
  tidY = k * blockDim.y + threadIdx.y;
   
  //OBTÉM o ladrilho da DIAGONAL K (Principal)
  ladrilhoPrincipal[threadIdx.y][threadIdx.x] = F[tidY * n + tidX];
  
  //BARREIRA de sincronização - Garante que a memória compartilhada seja preenchida antes do processamento	
  __syncthreads();

  //PROCESSA o ladrilho da DIAGONAL K
  for(int m=0; m < blockDim.x; m++) {

    if(ladrilhoPrincipal[m][threadIdx.x] == 1 && ladrilhoPrincipal[threadIdx.y][m] == 1){
      ladrilhoPrincipal[threadIdx.y][threadIdx.x] = 1;   
    } 
	//BARREIRA de sincronização - Garante que as dependências de dados sejam satisfeitas. 
    __syncthreads();
  }
  //ATUALIZA os valores do ladrilho da DIAGONAL K na memória global
  F[i * n + j] = ladrilhoPrincipal[threadIdx.y][threadIdx.x];   
}

__global__ 
void warshallKernelShared_LinhaColuna(int *F, int k, unsigned n){
    
  //Variáveis declaradas nos registradores - visíveis somente para o thread	
  int tidX;
  int tidY;

  //Variáveis compartilhadas - visíveis somente para os threads do bloco
  __shared__ int ladrilhoPrincipal[NUM_THREAD_BLOCK_Y][NUM_THREAD_BLOCK_X];
  __shared__ int ladrilho[NUM_THREAD_BLOCK_Y][NUM_THREAD_BLOCK_X];
  
  //OBTÉM os índices dos threads
  tidX = k * blockDim.x + threadIdx.x;
  tidY = k * blockDim.y + threadIdx.y;

  //OBTÉM o ladrilho da DIAGONAL na memória global e atribui para a memória compartilhada
  ladrilhoPrincipal[threadIdx.y][threadIdx.x] = F[tidY * n + tidX];

  //OBTÉM os índices dos threads
  tidX = blockIdx.x * blockDim.x + threadIdx.x;
  tidY = k * blockDim.y + threadIdx.y;
  //OBTÉM o ladrilho da LINHA k na memória global e atribui para a memória compartilhada
  ladrilho[threadIdx.y][threadIdx.x] = F[tidY * n + tidX];

  //BARREIRA de sincronização - Garante que a memória compartilhada seja preenchida antes do processamento	
  __syncthreads();

  //PROCESSA o ladrilho da LINHA k
  for(int m=0; m < blockDim.x; m++) {

    if(ladrilhoPrincipal[threadIdx.y][m] == 1 && ladrilho[m][threadIdx.x] == 1){
      ladrilho[threadIdx.y][threadIdx.x] = 1;   
    }
	//BARREIRA de sincronização - Garante que as dependências de dados sejam satisfeitas. 
    __syncthreads();
  }

  //ATUALIZA os valores do ladrilho da LINHA k na memória global
  F[tidY * n + tidX] = ladrilho[threadIdx.y][threadIdx.x];   

  //===========================================================
  //OBTÉM os índices dos threads
  tidX = k * blockDim.x + threadIdx.x;
  tidY = blockIdx.x * blockDim.y + threadIdx.y;
  
  //OBTÉM o ladrilho da COLUNA k na memória global e atribui para a memória compartilhada
  ladrilho[threadIdx.y][threadIdx.x] = F[tidY * n + tidX];

  //BARREIRA de sincronização - Garante que a memória compartilhada seja preenchida antes do processamento	
  __syncthreads();
  
  //PROCESSA o ladrilho da COLUNA k
  for(int m=0; m < blockDim.x; m++) {

    if(ladrilhoPrincipal[m][threadIdx.x] == 1 && ladrilho[threadIdx.y][m] == 1){
      ladrilho[threadIdx.y][threadIdx.x] = 1;   
    }        
	//BARREIRA de sincronização - Garante que as dependências de dados sejam satisfeitas. 
    __syncthreads();
  }
  
  //ATUALIZA os valores do ladrilho da COLUNA k na memória global
  F[tidY * n + tidX] = ladrilho[threadIdx.y][threadIdx.x];   
}

__host__
void pre_processamento(int* gpuF, int k, int size, dim3 grid, dim3 bloco){
    warshallKernelShared_Principal<<< 1, bloco  >>>(gpuF, k, size);
    warshallKernelShared_LinhaColuna<<< dim3(grid.x,1), bloco  >>>(gpuF, k, size);
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

