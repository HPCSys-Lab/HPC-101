
MUTEX=mutex mutex-try mutex-timed # mutex-attr 
COND=pt-cond
SEM=sem sem-try sem-timed proc-sem 
BAR=pt-barrier 
PROGS=pt-create pt-join pt-ret pt-attr pt-arg pt-stack pt-detach pt-kill pt-cancel pt-sigmask pt-once n-threads $(MUTEX) $(SEM) $(BAR) $(COND)

# faltam: c pt-rwlock.c

# man gcc
#
#	-pthreads
#
#	Add support for multithreading using the POSIX threads library. This option
# 	sets flags for both the preprocessor and linker. This option does not affect
#	the thread safety of object code produced by the compiler or that of libraries
#	supplied with it.

# CC=gcc -Wall -lpthread
# CC=gcc -Wall -pthread

CC=gcc -Wall -pthread


all: $(PROGS)

clean:
	rm -f $(PROGS)

pt-create:	pt-create.c
	$(CC) pt-create.c -o pt-create 

pt-join:	pt-join.c
	$(CC) pt-join.c -o pt-join 

pt-ret:	pt-ret.c
	$(CC) pt-ret.c -o pt-ret 

pt-detach:	pt-detach.c
	$(CC) pt-detach.c -o pt-detach 

pt-stack:	pt-stack.c
	$(CC) pt-stack.c -o pt-stack 

create-join:	create-join.c
	$(CC) create-join.c -o create-join 

n-threads:	n-threads.c
	$(CC) n-threads.c -o n-threads 

pt-arg:	pt-arg.c
	$(CC) pt-arg.c -o pt-arg 

pt-kill:	pt-kill.c
	$(CC) pt-kill.c -o pt-kill 

pt-cancel:	pt-cancel.c
	$(CC) pt-cancel.c -o pt-cancel 

pt-sigmask:	pt-sigmask.c
	$(CC) pt-sigmask.c -o pt-sigmask 

pt-once:	pt-once.c
	$(CC) pt-once.c -o pt-once 

pt-attr:	pt-attr.c
	$(CC) pt-attr.c -o pt-attr 

mutex:	mutex.c
	$(CC) mutex.c -o mutex 

mutex-try:	mutex-try.c
	$(CC) mutex-try.c -o mutex-try 

mutex-timed:	mutex-timed.c
	$(CC) mutex-timed.c -o mutex-timed 

mutex-attr:	mutex-attr.c
	$(CC) mutex-attr.c -o mutex-attr 

sem:	sem.c
	$(CC) sem.c -o sem 

sem-try:	sem-try.c
	$(CC) sem-try.c -o sem-try 

sem-timed:	sem-timed.c
	$(CC) sem-timed.c -o sem-timed 

proc-sem:	proc-sem.c
	$(CC) proc-sem.c -o proc-sem 

pt-barrier:	pt-barrier.c
	$(CC) pt-barrier.c -o pt-barrier 

pt-cond:	pt-cond.c
	$(CC) pt-cond.c -o pt-cond 

