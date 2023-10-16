
PROGS=ignore sigprocmask sigpending sig1 sig2 sig3 sig4 sig5 sig6 sig7 sig8 pause sigsuspend trap abort

CC=gcc -Wall


all:	$(PROGS)

clean:
	rm -f $(PROGS)

ignore:	ignore.c
	$(CC) ignore.c -o ignore

sigprocmask:	sigprocmask.c
	$(CC) sigprocmask.c -o sigprocmask

sigpending:	sigpending.c
	$(CC) sigpending.c -o sigpending

sig1:	sig1.c
	$(CC) sig1.c -o sig1

sig2:	sig2.c
	$(CC) sig2.c -o sig2

sig3:	sig3.c
	$(CC) sig3.c -o sig3

sig4:	sig4.c
	$(CC) sig4.c -o sig4

sig5:	sig5.c
	$(CC) sig5.c -o sig5

sig6:	sig6.c
	$(CC) sig6.c -o sig6

sig7:	sig7.c
	$(CC) sig7.c -o sig7

sig8:	sig8.c
	$(CC) sig8.c -o sig8

pause:	pause.c
	$(CC) pause.c -o pause

sigsuspend:	sigsuspend.c
	$(CC) sigsuspend.c -o sigsuspend

trap:	trap.c
	$(CC) trap.c -o trap

abort:	abort.c
	$(CC) abort.c -o abort
