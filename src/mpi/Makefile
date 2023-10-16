
# CC=mpicc
# CC=/opt/mpich2/gnu/bin/mpicc
CC=/opt/openmpi/bin/mpicc

# CC=/opt/mpich/gnu/bin/mpicc

I=/opt/mpich2/gnu/include/mpi.h
# I=/opt/openmpi/include/mpi.h

ALL=hello send-rec bcast reduce

all:	$(ALL)

clean:
		rm -f $(ALL)
		rm -f *.o

hello: hello.c
		$(CC) -Wall hello.c -o hello
		rm -f hello.o
send-rec: send-rec.c
		$(CC) -Wall send-rec.c -o send-rec
		rm -f send-rec.o
bcast: bcast.c
		$(CC) -Wall bcast.c -o bcast
		rm -f bcast.o
reduce: reduce.c
		$(CC) -Wall reduce.c -o reduce
		rm -f reduce.o
