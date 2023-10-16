
PROGS= syscall hello 

CC=gcc -Wall


all: $(PROGS)

clean:
	rm -f $(PROGS) hello.o

syscall:	syscall.c
	$(CC) syscall.c -o syscall

hello:	hello.asm
	nasm -f elf hello.asm
	ld hello.o -o hello
	rm -f hello.o

fork:	fork.asm
	nasm -f elf fork.asm
	ld fork.o -o fork
	rm -f fork.o
