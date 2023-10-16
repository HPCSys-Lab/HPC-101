
PROGS=yield sched affinity fifo rr

CC=gcc -Wall


all: $(PROGS)

clean:
	rm -f $(PROGS)

sched:	sched.c
	$(CC) sched.c -o sched

yeild:	yeild.c
	$(CC) yeild.c -o yeild

affinity:	affinity.c
	$(CC) affinity.c -o affinity

fifo:	fifo.c
	$(CC) fifo.c -o fifo

rr:	rr.c
	$(CC) rr.c -o rr
