CC=gcc

all: clean sequential pthread openmp

sequential:
	$(CC) laplace_seq.c -o laplace_seq

pthread:
	$(CC) laplace_pth.c -o laplace_pth -lpthread

openmp:
	$(CC) laplace_omp.c -o laplace_omp -fopenmp

clean:
	rm -f laplace_seq laplace_pth laplace_omp
