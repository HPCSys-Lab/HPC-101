
PROGS=wait sigchild

CC=gcc -Wall


all: $(PROGS)

clean:
	rm -f $(PROGS)

wait:	wait.c
	$(CC) wait.c -o wait

sigchild:	sigchild.c
	$(CC) sigchild.c -o sigchild
