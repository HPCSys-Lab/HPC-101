
PROGS=pipe1 pipe2 dup2 fifo popen1 popen2 eco

all: $(PROGS)

clean:
	rm -f $(PROGS)

pipe1:	pipe1.c
	gcc -Wall pipe1.c -o pipe1

pipe2:	pipe2.c
	gcc -Wall pipe2.c -o pipe2

dup2:	dup2.c
	gcc -Wall dup2.c -o dup2

fifo:	fifo.c
	gcc -Wall fifo.c -o fifo

popen1:	popen1.c eco
	gcc -Wall popen1.c -o popen1

popen2:	popen2.c
	gcc -Wall popen2.c -o popen2

eco:	eco.c
	gcc -Wall eco.c -o eco

