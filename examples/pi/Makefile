CC=gcc

all: clean sequential pthread openmp

sequential:
	$(CC) pi_seq.c -o pi_seq -lm

pthread:
	$(CC) pi_pth.c -o pi_pth -lpthread -lm

openmp:
	$(CC) pi_omp.c -o pi_omp -fopenmp -lm

clean:
	rm -f pi_seq pi_pth pi_omp
