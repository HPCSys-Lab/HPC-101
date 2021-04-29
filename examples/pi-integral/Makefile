CC=gcc

all: clean sequential pthread openmp

sequential:
	$(CC) pi_seq.c -o pi_seq

pthread:
	$(CC) pi_pth.c -o pi_pth -lpthread

openmp:
	$(CC) pi_omp.c -o pi_omp -fopenmp

clean:
	rm -f pi_seq pi_pth pi_omp
