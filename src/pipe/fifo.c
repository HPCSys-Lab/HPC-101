/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/
/*
** Programa : comunicacao entre processos usando named pipes: fifo
** Comando: mkfifo()
** Objetivo: criar e usar um named pipe / fifo
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define LEN 256


int 
main(int argc, char **argv)
{
	int fd,n_bytes,status;
	pid_t pid;
	char msg[LEN];
	char f_name[LEN];
	// char *f_name=(char *)malloc(LEN);
	char resp;

	if(argc<2) {
		printf("Uso: %s nome_fifo\n",argv[0]);
		return(0);
	}

/*
   mkfifo  makes a FIFO special file with name pathname.  mode specifies the
   FIFO's permissions. It is modified by the process's umask  in  the  usual
   way: the permissions of the created file are (mode & ~umask).

   A  FIFO special file is similar to a pipe, except that it is created in a
   different way.  Instead of being an anonymous communications  channel,  a
   FIFO special file is entered into the file system by calling mkfifo.

   Once  you  have  created a FIFO special file in this way, any process can
   open it for reading or writing, in the same  way  as  an  ordinary  file.
   However,  it  has  to  be open at both ends simultaneously before you can
   proceed to do any input or output operations on it.  Opening a  FIFO  for
   reading  normally blocks until some other process opens the same FIFO for
   writing, and vice versa. See fifo(4) for non-blocking  handling  of  FIFO
   special files.

  A  FIFO  special file (a named pipe) is similar to a pipe, except that it
  is accessed as part of the file system.  It can  be  opened  by  multiple
  processes  for reading or writing. When processes are exchanging data via
  the FIFO, the kernel passes all data internally without writing it to the
  file system. Thus, the FIFO special file has no contents on the file sys-
  tem, the file system entry merely serves as a  reference  point  so  that
  processes can access the pipe using a name in the file system.

  The  kernel  maintains exactly one pipe object for each FIFO special file
  that is opened by at least one process.  The FIFO must be opened on  both
  ends  (reading  and writing) before data can be passed. Normally, opening
  the FIFO blocks until the other end is opened also.

  A process can open a FIFO in non-blocking mode. In this case, opening for
  read  only  will  succeed even if noone has opened on the write side yet;
  opening for write only will fail with ENXIO (no such device  or  address)
  unless the other end has already been opened.

  Under  Linux,  opening  a  FIFO  for  read and write will succeed both in
  blocking and non-blocking mode. POSIX leaves  this  behaviour  undefined.
  This  can  be  used to open a FIFO for writing while there are no readers
  available. A process that uses both ends of the connection  in  order  to
  communicate with itself should be very careful to avoid deadlocks.

  When  a  process  tries to write to a FIFO that is not opened for read on
  the other side, the process is sent a SIGPIPE signal.
*/

	strncpy(f_name,argv[1],LEN);

	// int mkfifo(const char *pathname, mode_t mode);

	if(mkfifo(f_name,00644)==-1) 
		perror("Erro criando fifo");
		
/*
  The open() system call is used to convert a pathname into a file descrip-
  tor (a small, non-negative integer for use  in  subsequent  I/O  as  with
  read,  write,  etc.).   When  the call is successful, the file descriptor
  returned will be the lowest file descriptor not currently  open  for  the
  process.   This  call  creates a new open file, not shared with any other
  process.  (But shared open files may arise via the fork(2) system  call.)
  The  new file descriptor is set to remain open across exec functions (see
  fcntl(2)).  The file offset is set to the beginning of the file.

  The parameter flags is one of O_RDONLY, O_WRONLY or O_RDWR which  request
  opening  the file read-only, write-only or read/write, respectively, bit-
  wise-or'd with zero or more of the following:

  O_CREAT
    If the file does not exist it will be created.   The  owner  (user
    ID)  of  the  file is set to the effective user ID of the process.
    The group ownership (group ID) is  set  either  to  the  effective
    group ID of the process or to the group ID of the parent directory
    (depending on filesystem type and mount options, and the  mode  of
    the  parent  directory, see, e.g., the mount options bsdgroups and
    sysvgroups of the ext2 filesystem, as described in mount(8)).

  O_NONBLOCK or O_NDELAY
    When  possible,  the  file is opened in non-blocking mode. Neither
    the open nor any subsequent  operations  on  the  file  descriptor
    which is returned will cause the calling process to wait.  For the
    handling of FIFOs (named pipes), see also fifo(4).  This mode need
    not have any effect on files other than FIFOs.

  O_SYNC The  file is opened for synchronous I/O. Any writes on the result-
    ing file descriptor will block the calling process until the  data
    has  been  physically  written  to  the  underlying hardware.  See
    RESTRICTIONS below, though.
*/

	
	if((pid=fork())==-1) {
		perror("Falha no fork");
		exit(0);
	}

	if(pid) {  // pai: abre fifo para escrita

		// para abrir fifo com open, entrada no sistema de arquivos deve 
		// existir, criada com mkfifo

		if((fd=open(f_name,O_WRONLY))==-1) {
			perror("Erro abrindo fifo para escrita");
			exit(0);
		}

		/* le de STDIN e escreve no fifo */
		do {
			printf("Msg p/ filho: "); fflush(stdout);

			// char *fgets(char *s, int size, FILE *stream);
			fgets(msg,LEN,stdin);

			// envia -1 byte, para eliminar o <CR> lido com fgets
			if((n_bytes=write(fd,msg,strlen(msg)-1))==-1) {
				perror("Erro escrevendo no fifo");
				break;
			}
			// sleep(1);
		} while (strncmp(msg,"fim",3));

		close(fd);

		printf("Pai terminando...\n"); fflush(stdout);

	} else {   // filho: le do fifo e escreve em stdout

		if((fd=open(f_name,O_RDONLY))==-1) {
			perror("Erro abrindo fifo para leitura");
			exit(0);
		}

		/* le do fifo e escreve em STDOUT */
		do {
			sleep(10);
			if((n_bytes=read(fd,msg,LEN))==-1) {
				perror("Erro lendo no fifo");
				break;
			}
			msg[n_bytes]='\0'; // ajusta terminação da string
			printf("Filho leu (%d): %s\n",strlen(msg),msg);  fflush(stdout);

		} while (n_bytes>=0 && strncmp(msg,"fim",3));

		close(fd);

		printf("Filho terminando...\n"); fflush(stdout);
	}

	if(pid) { // pai remove fifo
		// espera filho terminar
		printf("Pai esperando filho...\n");
		if(wait(&status)==-1) 
			perror("Erro em wait");

		printf("Remove %s (s/n)?",f_name); fflush(stdout);
		do {
			scanf("%c",&resp);
		} while(resp!='s' && resp!='n');
		if(resp=='s') {
			if(unlink(f_name)==-1)
				perror("Erro removendo fifo");
			else
				printf("%s removido\n",f_name);
		} 
	}

	return(0);
}

