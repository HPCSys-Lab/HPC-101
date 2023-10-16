
PROGS=p-s p-d p-o

CC=gcc -Wall


all: $(PROGS)

clean:
	rm -f f1.o f2.o s1.o s2.o libstat.a libdyn.so p-d p-s p-o

libstat.a:	f1.o f2.o
	ar cr libstat.a f1.o f2.o 

libdyn.so:	s1.o s2.o
	$(CC) -shared -o libdyn.so s1.o s2.o 

f1.o:	f1.c
	$(CC) -c f1.c -o f1.o

f2.o:	f2.c
	$(CC) -c f2.c -o f2.o

# -fpic
#		Generate position-independent code (PIC) suitable for use in a shared
#		library, if supported for the target machine. 
# -fPIC
#		If supported for the target machine, emit position-independent code,
#		suitable for dynamic linking and avoiding any limit on the size of the
#		global offset table. 

# PIC: Position-Independent Code
s1.o:	f1.c
	$(CC) -c -fPIC f1.c -o s1.o

s2.o:	f2.c
	$(CC) -c -fPIC f2.c -o s2.o

p-o:	p.c f1.o f2.o
	$(CC) p.c -o p-o f1.o f2.o

p-s:	p.c libstat.a
	$(CC) p.c -o p-s -L. -lstat --static

p-d:	p.c libdyn.so
	@ # $(CC) p.c -o p-d -L. -ldyn -W1,-rpath,.
	@ echo Ajustes: export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.
	$(CC) p.c -o p-d -L. -ldyn
