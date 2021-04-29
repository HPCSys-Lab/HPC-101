CC=gcc

all: clean sequential pthread openmp

sequential:
	$(CC) vecadd_seq.c -o vecadd_seq

pthread:
	$(CC) vecadd_pth.c -o vecadd_pth -lpthread

openmp:
	# For Linux systems
	$(CC) vecadd_omp.c -o vecadd_omp -fopenmp
	# For MacOS with clang
	# /usr/local/opt/llvm/bin/clang vecadd.c -o vecadd -fopenmp -L/usr/local/opt/llvm/lib

clean:
	rm -f vecadd_seq vecadd_pth vecadd_omp
