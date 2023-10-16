
PROGS= p1 p2 p3 p4 p5 p6  p7 p8 p9 hello system system2 prod-cons vfork

CC=gcc -Wall


all: $(PROGS)

clean:
	rm -f $(PROGS)

p1:	p1.c
	$(CC) p1.c -o p1

p2:	p2.c
	$(CC) p2.c -o p2

p3:	p3.c
	$(CC) p3.c -o p3

p4:	p4.c
	$(CC) p4.c -o p4

p5:	p5.c
	$(CC) p5.c -o p5

p6:	p6.c
	$(CC) p6.c -o p6

p6x:	p6x.c
	$(CC) p6x.c -o p6x

p7:	p7.c hello
	$(CC) p7.c -o p7

p8:	p8.c hello
	$(CC) p8.c -o p8

p9:	p9.c hello
	$(CC) p9.c -o p9

vfork:	vfork.c hello
	$(CC) vfork.c -o vfork

hello:	hello.c
	$(CC) hello.c -o hello

system:	system.c
	$(CC) system.c -o system

system2:	system2.c
	$(CC) system2.c -o system2

prod-cons:	prod-cons.c
	$(CC) prod-cons.c -o prod-cons
