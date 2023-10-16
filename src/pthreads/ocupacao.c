/*
** Programa : 
** Comando: 
** Objetivo: estudar espera pelo fim de thread e tratamento do valor de retorno
             join e normalmente feito na main thread apenas porque ela e que tem
             os identificadores dos threads. Sendo variaveis globais, qualquer
             thread pode fazer o join por qualquer outra do mesmo processo.
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>

int _num;

int n_threads, dim_matriz, n_vezes;

#define LEN 128
#define MAX_THREADS  1024

void *soma(void *id)
{

	// dorme um tempo aleatório
	//sleep(random()%MAX_THREADS);

	// for (i=0; i<1000; i++)
	//		_num += (int)(random()%N_THREADS);

	_num=(int)id;

	printf("Soma (%d) = %d\n",(int)id, (int)_num);

	// void pth:w
	// read_exit(void *retval);

	// pthread_exit terminates the execution of the calling thread. All cleanup handlers 
	// that have been set for the calling thread with !pthread_cleanup_push!(3) are
	// executed in reverse order (the most recently pushed handler is executed first).
	// Finalization functions for thread-specific data are then called for all keys that
	// have non-!NULL! values associated with them in the calling thread (see
	// !pthread_key_create!(3)). Finally, execution of the calling thread is stopped.
	// The |retval| argument is the return value of the thread. It can be consulted from
	// another thread using !pthread_join!(3).

	// pthread_exit(NULL);
	
	double** A =(double**)malloc(dim_matriz*sizeof(double*));
	for (int index=0;index<dim_matriz;index++){
		A[index] = (double*) malloc(dim_matriz * sizeof(double));
	}
	double** B =(double**)malloc(dim_matriz*sizeof(double*));
	for (int index=0;index<dim_matriz;index++){
		B[index] = (double*) malloc(dim_matriz * sizeof(double));
	}
	double** C =(double**)malloc(dim_matriz*sizeof(double*));
	for (int index=0;index<dim_matriz;index++){
		C[index] = (double*) malloc(dim_matriz * sizeof(double));
	}

	int i,j,k;
	double soma=0.0;
	for (i=0;i<dim_matriz;i++) 
		for(j=0;j<dim_matriz;j++){ 
			A[i][j]=(double) (i * dim_matriz + j);
			B[i][j]=(double) (i * dim_matriz + j);
		}

	for (int n=0;n<n_vezes;n++)
		for (i=0;i<dim_matriz;i++) 
			for (j=0;j<dim_matriz;j++){
				C[i][j]=0.0;
				for (k=0;k<dim_matriz;k++)
					C[i][j] += A[i][k]*B[k][j];
			}
        free(A);
	free(B);
	free(C);

	pthread_exit((void *)_num);
}

int 
main (int argc, char *argv[])
{
	pthread_t thread[MAX_THREADS];
	int result, t, final;
	char err_msg[LEN];
 	double time_start, time_end;
        struct timeval tv;
        struct timezone tz;

	// void srandom(unsigned int seed);
	srandom((unsigned int)getpid());

        if (argc < 3)  {
		printf("    Formato:  %s  <numthreads> <dim_matriz> <n_vezes> \n",argv[0]);
		exit(1);
	}
        n_threads = atoi(argv[1]);
	printf("   n_threads =%d\n",n_threads);
	if (n_threads > MAX_THREADS) {
		printf ("n_threads muito grande!!\n");
		exit(1);
	}

	dim_matriz = atoi(argv[2]);
        printf("   dim_matriz =%d\n", dim_matriz);

	n_vezes = atoi(argv[3]);
        printf("   n_vezes =%d\n",n_vezes);


	gettimeofday(&tv, &tz);
        time_start = (double)tv.tv_sec +
                     (double)tv.tv_usec / 1000000.0;
 

	for(t=0; t<n_threads; t++) {
		printf("Criando thread %d\n", t);
		result = pthread_create(&thread[t], NULL, soma, (void *)t); 
		if (result) {
			strerror_r(result,err_msg,LEN);
			printf("Erro criando thread %d: %s\n",t,err_msg);
			exit(0);
		}
	}

	// int pthread_join(pthread_t thread, void **value_ptr);

	// The pthread_join() function shall suspend execution of the calling thread until the
	// target thread terminates, unless the target thread has already terminated. On
	// return from a successful pthread_join() call with a non-NULL value_ptr argument,
	// the value passed to pthread_exit() by the terminating thread shall be made avail-
	// able in the location referenced by value_ptr. When a pthread_join() returns suc-
	// cessfully, the target thread has been terminated. 
	// If successful, the pthread_join() function shall return zero; otherwise, an error
	// number shall be returned to indicate the error.

	// espera pelas outras threads
	for(t=0; t<n_threads; t++) {

		 result = pthread_join(thread[t], NULL);
		// para ter resultado aleatorio, precisaria fazer o join em sequencia aleatoria tambem...
		// result = pthread_join(thread[t], (void *)&final);
		if (result) {
			strerror_r(result,err_msg,LEN);
			fprintf(stderr,"Erro em pthread_join: %s\n",err_msg);
			exit(0);
		}else
			printf("Thread %d joined\n",t);
	}
	gettimeofday(&tv, &tz);
	time_end = (double)tv.tv_sec +
                 (double)tv.tv_usec / 1000000.0;
        
	printf("Tempo gasto: %lf\n", time_end - time_start);
	printf("Final: %d\n",final);

	pthread_exit(NULL);
}



