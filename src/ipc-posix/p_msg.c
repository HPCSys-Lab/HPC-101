
/* 
** Universidade Federal de São Carlos
** Departamento de Computação
** Prof. Hélio Crestana Guardia
** Programação Paralela e Distribuída
*/

/*
** Programa : Posix Message Queues
** Comando: 
** Objetivo: 
*/


#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <mqueue.h>
#include <stdio.h>
#include <string.h>

/* requires librt: -lrt */

// #define MQ_PRIO_MAX     32768
// per-uid limit of kernel memory used by mqueue, in bytes 
//	#define MQ_BYTES_MAX    819200

#define LEN 128


int
main()
{
	// typedef int mqd_t;
	mqd_t mq;
	int n_bytes, status;
	unsigned int prio;
	char fila_msg[LEN]="/mq.id";
	struct mq_attr attrib;
	char *msg;
	pid_t pid;

/*
	// mqd_t mq_open(const char *name, int oflag, ...);

	The mq_open() function shall establish the connection between a process
	and a message queue with a message queue descriptor. It shall create an
	open  message queue description that refers to the message queue, and a
	message queue  descriptor  that  refers  to  that  open  message  queue
	description. The message queue descriptor is used by other functions to
	refer to that message queue. The name argument points to a string  nam-
	ing  a message queue. It is unspecified whether the name appears in the
	file system and is visible to other functions that  take  pathnames  as
	arguments.   The  name argument shall conform to the construction rules
	for a pathname. If name begins with the slash character, then processes
	calling  mq_open()  with the same value of name shall refer to the same
	message queue object, as long as that name has  not  been  removed.  If
	name does not begin with the slash character, the effect is implementa-
	tion-defined.  The interpretation of slash characters  other  than  the
	leading slash character in name is implementation-defined.  If the name
	argument is not the name of an existing message queue and  creation  is
	not requested, mq_open() shall fail and return an error.
	
	A  message queue descriptor may be implemented using a file descriptor,
	in which case applications can open up to at least {OPEN_MAX} file  and
	message queues.

	The  oflag  argument requests the desired receive and/or send access to
	the message queue. The requested access permission to receive  messages
	or  send  messages  shall  be  granted  if the calling process would be
	granted read or write access, respectively,  to  an  equivalently  pro-
	tected file.

	The  value of oflag is the bitwise-inclusive OR of values from the fol-
	lowing list. Applications shall specify exactly one of the first  three
	values (access modes) below in the value of oflag:

	O_RDONLY
	Open  the  message queue for receiving messages. The process can
	use the returned message queue descriptor with mq_receive(), but
	not mq_send(). A message queue may be open multiple times in the
	same or different processes for receiving messages.

	O_WRONLY
	Open the queue for sending messages. The  process  can  use  the
	returned   message  queue  descriptor  with  mq_send()  but  not
	mq_receive().  A message queue may be open multiple times in the
	same or different processes for sending messages.

	O_RDWR Open the queue for both receiving and sending messages. The pro-
	cess can use any of  the  functions  allowed  for  O_RDONLY  and
	O_WRONLY. A message queue may be open multiple times in the same
	or different processes for sending messages.
	
	Any combination of the remaining flags may be specified in the value of
	oflag:

	O_CREAT
	Create  a  message  queue. It requires two additional arguments:
	mode, which shall be of type mode_t, and attr, which shall be  a
	pointer  to  an  mq_attr  structure.   If  the pathname name has
	already been used to create a message queue that  still  exists,
	then  this  flag  shall  have  no  effect, except as noted under
	O_EXCL. Otherwise, a message queue shall be created without  any
	messages in it. The user ID of the message queue shall be set to
	the effective user ID of the process, and the group  ID  of  the
	message queue shall be set to the effective group ID of the pro-
	cess. The file permission bits shall be  set  to  the  value  of
	mode. When bits in mode other than file permission bits are set,
	the effect is implementation-defined. If attr is NULL, the  mes-
  	sage  queue shall be created with implementation-defined default
	message queue attributes. If attr is non-NULL  and  the  calling
	process has the appropriate privilege on name, the message queue
	mq_maxmsg and mq_msgsize attributes shall be set to  the  values
	of  the  corresponding members in the mq_attr structure referred
	to by attr. If attr is non-NULL, but the  calling  process  does
	not  have the appropriate privilege on name, the mq_open() func-
	tion shall fail and return an error without creating the message
	queue.

	O_EXCL If  O_EXCL and O_CREAT are set, mq_open() shall fail if the mes-
	sage queue name exists. The check for the existence of the  mes-
	sage  queue and the creation of the message queue if it does not
	exist shall be atomic with respect to  other  threads  executing
	mq_open()  naming  the same name with O_EXCL and O_CREAT set. If
	O_EXCL is set and O_CREAT is not set, the result is undefined.
	
	O_NONBLOCK
	Determines  whether  an  mq_send()  or  mq_receive()  waits  for
	resources or messages that are not currently available, or fails
	with errno set to [EAGAIN]; see mq_send() and  mq_receive()  for
	details.

RETURN VALUE
	Upon successful completion, the function shall return a  message  queue
	descriptor;  otherwise,  the  function  shall  return (mqd_t)-1 and set
	errno to indicate the error.
*/
/*
	struct mq_attr {
		long int mq_flags;	// Message queue flags.
		long int mq_maxmsg;	// Maximum number of messages. 
		long int mq_msgsize;	// Maximum message size.
		long int mq_curmsgs;	// Number of messages currently queued.
		long int __pad[4];
	};
*/
	
	// mqd_t mq_open(const char *name, int oflag [, ...
	//	              mode_t mode, struct mq_attr *attr ] );
	if((mq=mq_open(fila_msg, O_RDWR|O_CREAT, 0644, 0))==(mqd_t)-1) {
		perror("Erro criando Posix Message Queue");
		exit(0);
	}else {
		printf("\nPosix Message Queue: %d, MQ_PRIO_MAX: %ld\n\n",mq,sysconf(_SC_MQ_PRIO_MAX));
	}

/*
	int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);

	The   mq_getattr()   function   shall  obtain  status  information  and
	attributes of the message queue and the open message queue  description
	associated with the message queue descriptor.

	The mqdes argument specifies a message queue descriptor.

	The  results  shall  be returned in the mq_attr structure referenced by
	the mqstat argument.

	Upon return, the following members shall  have  the  values  associated
	with  the  open message queue description as set when the message queue
	was opened and as modified by subsequent mq_setattr() calls:  mq_flags.

	The  following attributes of the message queue shall be returned as set
	at message queue creation: mq_maxmsg, mq_msgsize.

	Upon return, the following members within the mq_attr structure  refer-
	enced  by  the mqstat argument shall be set to the current state of the
	message queue:

	mq_curmsgs: The number of messages currently on the queue.
*/
/*
	struct mq_attr {
		long int mq_flags;	// Message queue flags.
		long int mq_maxmsg;	// Maximum number of messages. 
		long int mq_msgsize;	// Maximum message size.
		long int mq_curmsgs;	// Number of messages currently queued.
		long int __pad[4];
	};
*/

	// int mq_getattr(mqd_t mqdes, struct mq_attr *mqstat);
	if(mq_getattr(mq, &attrib)==-1) {
		perror("Erro em mq_getattr");
	}else {
		printf("Message queue flags: %ld\n"
		       "Maximm number of messages: %ld\n"
		       "Maximum message size: %ld\n"
			    "Number of messages currently queued: %ld\n\n",
		       attrib.mq_flags,
		       attrib.mq_maxmsg,
		       attrib.mq_msgsize, 
		       attrib.mq_curmsgs);
	}	

/*
	// int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, 
	//		unsigned msg_prio);

	// int mq_timedsend(mqd_t mqdes, const char *msg_ptr, size_t msg_len,
	//		unsigned msg_prio, const struct timespec *abs_timeout);

	The mq_send() function shall add the message pointed to by the argument
	msg_ptr to the message queue specified by mqdes. The  msg_len  argument
	specifies  the  length of the message, in bytes, pointed to by msg_ptr.
	The value of msg_len shall be less than  or  equal  to  the  mq_msgsize
	attribute of the message queue, or mq_send() shall fail.

	If  the  specified message queue is not full, mq_send() shall behave as
	if the message is inserted into the message queue at the position indi-
	cated  by  the msg_prio argument. A message with a larger numeric value
	of msg_prio shall be inserted before  messages  with  lower  values  of
	msg_prio.  A  message  shall  be  inserted  after other messages in the
	queue, if any, with equal msg_prio. The value of msg_prio shall be less
	than {MQ_PRIO_MAX}.

	If the specified message queue is full and O_NONBLOCK is not set in the
	message queue description associated with mqdes, mq_send() shall  block
	until  space  becomes  available  to  enqueue  the  message,  or  until
	mq_send() is interrupted by a signal. If more than one thread is  wait-
	ing  to  send when space becomes available in the message queue and the
	Priority Scheduling option is supported, then the thread of the highest
	priority  that  has been waiting the longest shall be unblocked to send
	its message. Otherwise, it  is  unspecified  which  waiting  thread  is
	unblocked.   If  the  specified message queue is full and O_NONBLOCK is
	set in the message queue description associated with mqdes, the message
	shall not be queued and mq_send() shall return an error.

	The  mq_timedsend()  function  shall add a message to the message queue
	specified by mqdes in the manner defined for  the  mq_send()  function.
	However,  if  the specified message queue is full and O_NONBLOCK is not
	set in the message queue description associated with  mqdes,  the  wait
	for sufficient room in the queue shall be terminated when the specified
	timeout expires. If O_NONBLOCK is set in the message queue description,
	this function shall be equivalent to mq_send().

	The  timeout shall expire when the absolute time specified by abs_time-
	out passes, as measured by the clock on which timeouts are based  (that
	is,  when the value of that clock equals or exceeds abs_timeout), or if
	the absolute time specified by abs_timeout has already been  passed  at
	the time of the call.

	If  the  Timers  option is supported, the timeout shall be based on the
	CLOCK_REALTIME clock; if the Timers option is not supported, the  time-
	out  shall be based on the system clock as returned by the time() func-
	tion.

	The resolution of the timeout shall be the resolution of the  clock  on
	which  it  is  based.  The timespec argument is defined in the <time.h>
	header.

	Under no circumstance shall the operation fail with a timeout if  there
	is  sufficient  room  in  the queue to add the message immediately. The
	validity of the abs_timeout parameter need not be checked when there is
	sufficient room in the queue.

	Upon  successful  completion, the mq_send()    and mq_timedsend() func-
	tions shall return a value of zero.  Otherwise,  no  message  shall  be
	enqueued,  the  functions  shall  return  -1, and errno shall be set to
	indicate the error.
*/

/*
	// ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len,
	//		unsigned *msg_prio);

	// ssize_t mq_timedreceive(mqd_t mqdes, char *restrict msg_ptr,
	//		size_t msg_len, unsigned *restrict msg_prio,
	//		const struct timespec *restrict abs_timeout);

	The mq_receive() function shall receive the oldest of the highest  pri-
	ority message(s) from the message queue specified by mqdes. If the size
	of the buffer in bytes, specified by the msg_len argument, is less than
	the  mq_msgsize attribute of the message queue, the function shall fail
	and return an error. Otherwise, the selected message shall  be  removed
	from the queue and copied to the buffer pointed to by the msg_ptr argu-
	ment.
	
	If the value of msg_len is greater  than  {SSIZE_MAX},  the  result  is
	implementation-defined.
	
	If the argument msg_prio is not NULL, the priority of the selected mes-
	sage shall be stored in the location referenced by msg_prio.
	
	If the specified message queue is empty and O_NONBLOCK is  not  set  in
	the message queue description associated with mqdes, mq_receive() shall
	block until a message  is  enqueued  on  the  message  queue  or  until
	mq_receive()  is  interrupted  by  a signal. If more than one thread is
	waiting to receive a message when a message arrives at an  empty  queue
	and  the  Priority  Scheduling  option is supported, then the thread of
	highest priority that has been waiting the longest shall be selected to
	receive  the message. Otherwise, it is unspecified which waiting thread
	receives the message. If the  specified  message  queue  is  empty  and
	O_NONBLOCK  is  set  in  the  message queue description associated with
	mqdes, no message shall be removed from  the  queue,  and  mq_receive()
	shall return an error.

	The  mq_timedreceive() function shall receive the oldest of the highest
	priority  messages  from  the  message  queue  specified  by  mqdes  as
	described for the mq_receive() function. However, if O_NONBLOCK was not
	specified when the message queue was opened via the mq_open() function,
	and no message exists on the queue to satisfy the receive, the wait for
	such a message shall be terminated when the specified timeout  expires.
	If O_NONBLOCK is set, this function is equivalent to mq_receive().
	
	The  timeout  expires  when  the absolute time specified by abs_timeout
	passes, as measured by the clock on which timeouts are based (that  is,
	when  the value of that clock equals or exceeds abs_timeout), or if the
	absolute time specified by abs_timeout has already been passed  at  the
	time of the call.

	If  the  Timers  option is supported, the timeout shall be based on the
	CLOCK_REALTIME clock; if the Timers option is not supported, the  time-
	out  shall be based on the system clock as returned by the time() func-
	tion.

	The resolution of the timeout shall be the resolution of the  clock  on
	which  it  is  based.  The timespec argument is defined in the <time.h>
	header.

	Under no circumstance shall the operation fail with a timeout if a mes-
	sage  can  be removed from the message queue immediately.  The validity
	of the abs_timeout parameter need not be checked if a  message  can  be
	removed from the message queue immediately.

	Upon  successful  completion, the mq_receive()    and mq_timedreceive()
	functions shall return the length of the selected message in bytes  and
	the  message  shall  be  removed  from the queue. Otherwise, no message
	shall be removed from the queue, the functions shall return a value  of
	-1, and set errno to indicate the error.
*/

	if((msg=(char *)malloc(attrib.mq_msgsize))==0) {
		perror("Erro em malloc msg(mq_msgsize)");
		// libera a mq antes...
		exit(0);
	}

	if((pid=fork())==-1) {
		perror("Erro no fork");

	}else {		

		if(pid) { // pai
			do {
				printf("Mensagem: ");
				fgets(msg,LEN,stdin);

				// atribui prioridade aleatória de 0 a 10
				prio=rand()%10;

				// int mq_send(mqd_t mqdes, const char *msg_ptr, size_t msg_len, unsigned msg_prio);
				if((n_bytes=mq_send(mq, msg, strlen(msg), prio))==-1) {
					perror("Erro em mq_send");
				}else {
					// printf("Mensagem enviada com sucesso: %d\n",n_bytes); fflush(stdout);
			
					if(mq_getattr(mq, &attrib)==-1) {
						perror("Erro em mq_getattr");
					}else {
						printf("Number of messages currently queued: %ld\n\n",attrib.mq_curmsgs);
					}	
				}
			} while((strncmp(msg,"fim",3)));

		}else { // filho
	
			do {
				// dorme para dar tempo de testar recebimento por prioridade
				sleep(5);

				// ssize_t mq_receive(mqd_t mqdes, char *msg_ptr, size_t msg_len, unsigned *msg_prio);
				if((n_bytes=mq_receive(mq, msg, attrib.mq_msgsize, &prio))==-1) {	
					perror("Erro em mq_receive");
				}else {
					msg[n_bytes]='\0';
					printf("Mensagem recebida: (size: %d) (prio: %u) %s\n",n_bytes,prio,msg); fflush(stdout);
				}
			} while((strncmp(msg,"fim",3)));
			printf("Filho terminando...\n");
		} 

	} // else

/*
	// int mq_close(mqd_t mqdes);

	The  mq_close()  function shall remove the association between the mes-
	sage queue descriptor, mqdes, and its message  queue.  The  results  of
	using  this  message queue descriptor after successful return from this
	mq_close(), and until the return of this message queue descriptor  from
	a subsequent mq_open(), are undefined.

	If  the process has successfully attached a notification request to the
	message queue via this mqdes, this attachment shall be removed, and the
	message  queue is available for another process to attach for notifica-
	tion.
*/
/*
	// int mq_close(mqd_t mqdes);
	if(mq_close(mq)==-1)
		perror("Erro em mq_close");
*/
/*
	// int mq_unlink(const char *name);

	The  mq_unlink()  function  shall remove the message queue named by the
	pathname name. After a successful call to mq_unlink() with name, a call
	to  mq_open()  with  name  shall fail if the flag O_CREAT is not set in
	flags. If one or more  processes  have  the  message  queue  open  when
	mq_unlink()  is called, destruction of the message queue shall be post-
	poned until all references to the message queue have been closed.

	Calls to mq_open() to recreate the message queue  may  fail  until  the
	message  queue  is actually removed. However, the mq_unlink() call need
	not block until all references have been closed; it may return  immedi-
	ately.

	Upon  successful completion, the function shall return a value of zero.
	Otherwise, the named message queue shall be unchanged by this  function
	call,  and  the  function  shall  return a value of -1 and set errno to
	indicate the error.
*/
	// apenas o pai deve fazer o unlink
	if(pid) {
		printf("Pai esperando filho (%d)...\n",pid);
		// if((waitpid(-1,&status,0))==-1)
		if((waitpid(pid,&status,0))==-1)
			perror("Erro em waitpid");

		// int mq_unlink(const char *name);
		if(mq_unlink(fila_msg)==-1)
			perror("Erro em mq_unlink");
	}

	return(0);
}



