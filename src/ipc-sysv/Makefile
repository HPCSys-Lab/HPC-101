
CC=gcc -Wall

PROGS=shm shm2 sem sem2 sem3 msg timedsem

all: $(PROGS)

clean:
	rm -f $(PROGS)


shm:	shm.c
	$(CC) shm.c -o shm

shm2:	shm2.c
	$(CC) shm2.c -o shm2

sem:	sem.c
	$(CC) sem.c -o sem

sem2:	sem2.c
	$(CC) sem2.c -o sem2

sem3:	sem3.c
	$(CC) sem3.c -o sem3

timedsem:	timedsem.c
	$(CC) timedsem.c -o timedsem

msg:	msg.c
	gcc -Wall msg.c -o msg

