

PROGS= limits np usage

CC=gcc -Wall


all: $(PROGS)

clean:
	rm -f $(PROGS)

limits:	limits.c
	$(CC) limits.c -o limits

np:	np.c
	$(CC) np.c -o np

usage:	usage.c
	$(CC) usage.c -o usage
