CC=gcc

all: clean sequential pthread openmp

sequential:
	$(CC) mmul_seq.c -o mmul_seq

pthread:
	$(CC) mmul_pth.c -o mmul_pth -lpthread

openmp:
	$(CC) mmul_omp.c -o mmul_omp -fopenmp

clean:
	rm -f mmul_seq mmul_pth mmul_omp
