#include <pthread.h> 
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#define MAX_THREADS 512 
#define LEN 128
  
void *mult(void *list_ptr);
// Semaforo para acesso a variavel compartilhada

int dim, cols_per_thread, num_threads; 
float **A, **B, **C; 
struct intervalo{
    int inic;
    int fim;
};

int  main(int argc, char *argv[]) { 
      unsigned int seed;
      int i, j, status;
      pthread_t thread[MAX_THREADS]; 
      pthread_attr_t attr; 
      char err_msg[LEN];
      double time_start, time_end;
      struct timeval tv;
      struct timezone tz;
      if (argc<3) {
         printf(" Uso: %s <dim>  <num_threads>\n",argv[0]);
        exit(0);
      }

      pthread_attr_init (&attr); 
      pthread_attr_setscope (&attr,PTHREAD_SCOPE_SYSTEM); 

      dim = atoi(argv[1]);
      num_threads = atoi(argv[2]);
 
      cols_per_thread = dim / num_threads;
      printf("Dim=%d Num_threads=%d cols per thread = %d\n",dim, num_threads, cols_per_thread); 

      // Alocacao das matrizes
      if (( A=(float**)malloc(dim*sizeof(float*)))==NULL) {
	printf("Erro em malloc() \n");
	exit(0);
      }
      for (i=0;i<dim;i++)
         A[i]=(float*)malloc(dim*sizeof(float));
      if (( B=(float**)malloc(dim*sizeof(float*)))==NULL) {
        printf("Erro em malloc() \n");
        exit(0);
      }
      for (i=0;i<dim;i++)
         B[i]=(float*)malloc(dim*sizeof(float));
      if (( C=(float**)malloc(dim*sizeof(float*)))==NULL) {
        printf("Erro em malloc() \n");
        exit(0);
      }
      for (i=0;i<dim;i++)
         C[i]=(float*)malloc(dim*sizeof(float));
     
      // Inicializacao das matrizes
      for (i=0; i<dim;i++)
        for (j=0; j<dim; j++){
          A[i][j] = (double)(rand_r(&seed))/((double)rand_r(&seed)+1);
          seed *=(i+1);
          printf("A[%d,%d]=%.2f \n",i,j,A[i][j]);
        }
      for (i=0; i<dim;i++)
        for (j=0; j<dim; j++){
          B[i][j] = (double)(rand_r(&seed))/((double)rand_r(&seed)+1);
          seed *=(i+1);
          printf("B[%d,%d]=%.2f \n",i,j,B[i][j]);
        }

      gettimeofday(&tv, &tz);
      time_start = (double)tv.tv_sec +
                   (double)tv.tv_usec / 1000000.0;

      // Cria os threads
      for (i=0; i< num_threads; i++) { 
          printf("Criando thread %d\n", i);
          // guarda o inicio e fim do intervalo 
          struct intervalo  *interv = (struct intervalo *) malloc(sizeof(struct intervalo));
          interv->inic = ... // guarde aqui o intervalo a ser calculado por cada thread
          interv->fim = ...
          // COmplete a linha abaixo
          status = pthread_create(&thread[i], NULL, ..., ...);
          if (status) {
              strerror_r(status,err_msg,LEN);
              printf("Falha da criacao da thread %d (%d): %s\n",i,status,err_msg);
                  exit(EXIT_FAILURE);
          } else {
//              printf("%lu criou thread %lu\n",pthread_self(),thread[i]);
          }
      }
      for (i=0; i< num_threads; i++) { 
          pthread_join(thread[i], NULL); 
      } 

      gettimeofday(&tv, &tz);
      time_end = (double)tv.tv_sec +
                 (double)tv.tv_usec / 1000000.0;
      printf("Tempo gasto: %lf\n", time_end - time_start);

      for (i=0; i< dim; i++) 
         for (j=0; j<dim; j++)
            printf(" C[%d][%d]=%f\n",i,j,C[i][j]);
      exit(0);
  }

// Funcao a ser executada pelos threads
void *mult(void *list_ptr){
	int i, j, k;
        struct intervalo *p;
        // Obtem o endereco a partir do qual o thread deve somar
	p = ...
        // e finalmente paralelize o algoritmo abaixo 
          for (i=0; i<dim; i++)
            for (j=0; j<dim;j++) {
              C[i][j] = 0.0;
              for (k=0; k<dim;k++)
                C[i][j] += A[i][k]*B[k][j];
            } 
	pthread_exit(0);
}


