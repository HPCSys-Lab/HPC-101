%%cu

#define N 2048
#define NUM_THREADS 128
#include <stdio.h>
#include <time.h>
#include <assert.h>

typedef float* ptrmat; 
typedef float typemat; 


inline cudaError_t checkCuda(cudaError_t result)
{
  if (result != cudaSuccess) {
    fprintf(stderr, "CUDA Runtime Error: %s\n", cudaGetErrorString(result));
    assert(result == cudaSuccess);
  }
  return result;
}

//--------------------------------------------------------------------------
void initMatrix(ptrmat matrix, unsigned size)
{
  time_t t;
	srand((unsigned int) time(&t));
	for (int i=0; i<size; i++) {
     for (int j=0; j<size; j++) { 
		   //matrix[i * size + j] = (typemat)( rand() & 0xFF )/10.0f;
			 matrix[i * size + j] = rand() % 50;			 
     }  
	}
}

void initMatrixL(ptrmat matrixL, unsigned size)
{
  
	for (int i=0; i<size; i++) {
     for (int j=0; j<size; j++) { 
       if(i == j )
			    matrixL[i * size + j] = 1;			 
     }  
	}
}


void multiMatrizesCPU(ptrmat A, ptrmat B, ptrmat C, unsigned n)
{
		typemat soma;
		for(int lin = 0; lin < n; lin ++){
				for(int col = 0; col < n; col ++){
						soma = 0;
						for(int k = 0; k < n; k++){
								soma += A[lin * n + k] * B[k * n + col];
						}
						C[lin * n + col] = soma;
				}					
		}
}


void imprimeSomaElementos(ptrmat matrix, unsigned n)
{
    double soma = 0;
    for (int i=0; i < n; i++) {
        for (int j=0; j < n; j++){
					soma += matrix[i * n + j];		
				}	    
	  } 
    printf("A soma dos elementos da matriz é %f\n",soma);
}

void checkDecompLU(ptrmat matrixL, ptrmat matrixU){

  int elementNumber  = N * N * sizeof(typemat);
  ptrmat C  = (ptrmat) malloc(elementNumber);

  multiMatrizesCPU(matrixL, matrixU, C, N);
  imprimeSomaElementos(C, N);

  free(C);
}

//--------------------------------------------------------

__global__ 
void parallel_algorithm_LU(ptrmat matrixL, ptrmat matrixU, int pivotStep, int n){

  int pivotIndex = pivotStep - 1;

  int i = blockIdx.y * blockDim.y + pivotStep  + threadIdx.y;
  int j = blockIdx.x * blockDim.x + pivotIndex + threadIdx.x;
  
  __shared__ typemat pivotValue;
  __shared__ double factor;

  if(threadIdx.x == 0 && threadIdx.y == 0){
      pivotValue = matrixU[pivotIndex * n + pivotIndex];
      if( pivotValue != 0 )
        factor = ((double) matrixU[i * n + pivotIndex]) / pivotValue;
      else
        printf("Não tem decomposição LU!!!\n");  
  }

  __syncthreads();

  if( (i >= n) ||  (j >= n) || pivotValue == 0 ) return;

  int indexGlobal = i * n + j;
    
  matrixU[ indexGlobal ] = matrixU[ indexGlobal ] - factor * matrixU[ pivotIndex * n + j];

  if(j == pivotIndex && i > pivotIndex)
  {
      matrixL[ indexGlobal ] = factor; 
  }
}

void decompLU_GPU(ptrmat matrix, ptrmat matrixL, ptrmat matrixU){

  int byteNumber = N * N * sizeof(typemat);

  ptrmat gpu_matrixU;
  ptrmat gpu_matrixL;

  //Define o tamanho das dimensões do bloco
  int blockSize_X = NUM_THREADS;
  int blockSize_Y = 1;
 
  //Define o tamanho das dimensões da grid
  int gridSize_X = ceil( ((float) N )/ blockSize_X);
  int gridSize_Y = ceil( ((float)  (N - 1) ) / blockSize_Y); 
  
  dim3 block = dim3(blockSize_X, blockSize_Y); 
  dim3 grid  = dim3(gridSize_X, gridSize_Y); 

  //Device memory allocation
  checkCuda( cudaMalloc( (void**) &gpu_matrixU, byteNumber) );
  checkCuda( cudaMalloc( (void**) &gpu_matrixL, byteNumber) );

  //Copy -- Host memory ---> Device memory
  checkCuda( cudaMemcpy(gpu_matrixL, matrixL, byteNumber, cudaMemcpyHostToDevice) );	
	checkCuda( cudaMemcpy(gpu_matrixU, matrix, byteNumber, cudaMemcpyHostToDevice) );	
 	
  cudaEvent_t start, stop;
	float cpu_time = 0.0f;

	checkCuda( cudaEventCreate(&start) );
	checkCuda( cudaEventCreate(&stop) );

	checkCuda( cudaEventRecord(start, 0) );
  
  //Launching kernels - DecompLU Execution
  for(int stepPivot = 1; stepPivot < N; stepPivot++){
          
    parallel_algorithm_LU<<<grid, block>>>(gpu_matrixL, gpu_matrixU, stepPivot, N);  
	
    grid.x = ceil( ((float) (N - stepPivot)) / blockSize_X);
    grid.y = grid.y - 1;

  // Obtém os erros de lançamento de kernel
	  checkCuda( cudaGetLastError() );  
    
  }
  
	checkCuda( cudaEventRecord(stop, 0) );
	checkCuda( cudaEventSynchronize(stop) );
	checkCuda( cudaEventElapsedTime(&cpu_time, start, stop) );

  //Copy -- Device memory ---> Host memory
  checkCuda( cudaMemcpy(matrixL, gpu_matrixL, byteNumber, cudaMemcpyDeviceToHost) );	
	checkCuda( cudaMemcpy(matrixU, gpu_matrixU, byteNumber, cudaMemcpyDeviceToHost) );
    
  //Free memory - Device
  checkCuda( cudaFree(gpu_matrixU) );
  checkCuda( cudaFree(gpu_matrixL) );

  printf("Tempo de Execução da GPU: %.4f ms \n", cpu_time);
}


int main(){
    
  ptrmat matrix;
  ptrmat matrixL;
  ptrmat matrixU;

  int elementNumber = N * N * sizeof(typemat);
  
  //Device memory allocation
  matrix  = (ptrmat) malloc(elementNumber);
  matrixL = (ptrmat) malloc(elementNumber);
  matrixU = (ptrmat) malloc(elementNumber);

  memset(matrixL, 0, elementNumber);
  initMatrixL(matrixL, N);
  
  initMatrix(matrix, N);
  imprimeSomaElementos(matrix, N);
 
  decompLU_GPU(matrix, matrixL, matrixU);

  checkDecompLU(matrixL, matrixU);

  //Free memory - Host 
  free(matrix);
  free(matrixU);
  free(matrixL);
  
  return 0;
}
