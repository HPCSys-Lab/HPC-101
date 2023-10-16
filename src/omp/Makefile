
# Uso: make COMP=gcc _ou_ make COMP=icc

ifeq ($(COMP),icc)
	# Definições para compilador Intel: icc
	CC=/opt/intel/cc/10.0.025/bin/icc 
	CFLAGS=-openmp
	INC=-I /opt/intel/cc/10.0.025/include
	# Ajustar: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/intel/cc/10.0.025/lib
else # gcc
	# Definições para compilador GNU: gcc
	CC=gcc
	CFLAGS=-fopenmp -Wall
	# INC=-I /opt/gcc-4.2.1/lib/gcc/i686-pc-linux-gnu/4.2.1/include
	# Ajustar: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/gcc-4.2.1/lib
endif


PROGS=omp1 omp2 par1 par2 par3 func for1 for2 chunk firstprivate lastprivate nested nowait numprocs numthreads pi reduce schedule sections sharing single threads

all: $(PROGS)

clean:
	rm -f $(PROGS)

chunk:	chunk.c
	$(CC) $(CFLAGS) $(INC) chunk.c -o chunk 
firstprivate:	firstprivate.c
	$(CC) $(CFLAGS) $(INC) firstprivate.c -o firstprivate 
for1:	for1.c
	$(CC) $(CFLAGS) $(INC) for1.c -o for1
for2:	for2.c
	$(CC) $(CFLAGS) $(INC) for2.c -o for2 
func:	func.c
	$(CC) $(CFLAGS) $(INC) func.c -o func 
lastprivate:	lastprivate.c
	$(CC) $(CFLAGS) $(INC) lastprivate.c -o lastprivate 
nested:	nested.c
	$(CC) $(CFLAGS) $(INC) nested.c -o nested 
nowait:	nowait.c
	$(CC) $(CFLAGS) $(INC) nowait.c -o nowait 
numprocs:	numprocs.c
	$(CC) $(CFLAGS) $(INC) numprocs.c -o numprocs 
numthreads:	numthreads.c
	$(CC) $(CFLAGS) $(INC) numthreads.c -o numthreads 
omp1:	omp1.c
	$(CC) $(CFLAGS) $(INC) omp1.c -o omp1
omp2:	omp2.c
	$(CC) $(CFLAGS) $(INC) omp2.c -o omp2 
par1:	par1.c
	$(CC) $(CFLAGS) $(INC) par1.c -o par1 
par2:	par2.c
	$(CC) $(CFLAGS) $(INC) par2.c -o par2 
par3:	par3.c
	$(CC) $(CFLAGS) $(INC) par3.c -o par3 
pi:	pi.c
	$(CC) $(CFLAGS) $(INC) pi.c -o pi 
reduce:	reduce.c
	$(CC) $(CFLAGS) $(INC) reduce.c -o reduce 
schedule:	schedule.c
	$(CC) $(CFLAGS) $(INC) schedule.c -o schedule 
sections:	sections.c
	$(CC) $(CFLAGS) $(INC) sections.c -o sections 
sharing:	sharing.c
	$(CC) $(CFLAGS) $(INC) sharing.c -o sharing 
single:	single.c
	$(CC) $(CFLAGS) $(INC) single.c -o single 
threads:	threads.c
	$(CC) $(CFLAGS) $(INC) threads.c -o threads 

