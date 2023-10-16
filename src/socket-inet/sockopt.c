#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
// #include <linux/tcp.h>
#include <endian.h>

int 
main()
{
	int win_size = 128 * 1024;	// 128 kilobytes 
	int sock, tam, val;
	struct timeval tv;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	// These setsockopt()s must happen before the connect() 

	// int setsockopt(int s, int level, int optname, const void *optval, socklen_t optlen);
	// int getsockopt(int s, int level, int optname, void *optval, socklen_t *optlen);

	// Getsockopt and setsockopt() manipulate the options associated with a socket.  Options may
	// exist at multiple protocol levels; they are always present at the uppermost socket level.

	// When  manipulating  socket  options the level at which the option resides and the name of
	// the option must be specified.  To manipulate options at the socket level, level is speci-
	// fied  as SOL_SOCKET.  To manipulate options at any other level the protocol number of the
	// appropriate protocol controlling the option is supplied.  For example, to  indicate  that
	// an  option  is to be interpreted by the TCP protocol, level should be set to the protocol
	// number of TCP; see getprotoent(3).


	// setsockopt(sock,SOL_SOCKET,SO_SNDBUF,(char *) &win_size, sizeof(win_size));
	// setsockopt(sock,SOL_SOCKET,SO_RCVBUF,(char *) &win_size, sizeof(win_size));

	tam=sizeof(win_size);
	getsockopt(sock,SOL_SOCKET,SO_SNDBUF, &win_size, (socklen_t *)&tam);
	printf("SNDBUF: %d\n", win_size);

	tam=sizeof(win_size);
	getsockopt(sock,SOL_SOCKET,SO_RCVBUF, &win_size, (socklen_t *)&tam);
	printf("RCVBUF: %d\n", win_size);

   // SO_RCVTIMEO and SO_SNDTIMEO
	// Specify the receiving or sending timeouts until reporting an error.   The  parameter  is  a
	// struct  timeval.   If  an input or output function blocks for this period of time, and data
	// has been sent or received, the return value of that function will be  the  amount  of  data
	// transferred;  if  no  data has been transferred and the timeout has been reached then -1 is
	// returned with errno set to EAGAIN or EWOULDBLOCK just as if the socket was specified to  be
	// nonblocking.   If  the  timeout  is set to zero (the default) then the operation will never
	// timeout.

	// struct timeval {
	//		time_t         tv_sec;	// seconds
	//		suseconds_t    tv_usec;	// microseconds
	// };

	tam=sizeof(tv);
	getsockopt(sock,SOL_SOCKET,SO_RCVTIMEO, &tv, (socklen_t *)&tam);
	printf("SO_RCVTIMEO: %d,%d\n",(int)tv.tv_sec,(int)tv.tv_usec);

	tam=sizeof(tv);
	getsockopt(sock,SOL_SOCKET,SO_SNDTIMEO, &tv, (socklen_t *)&tam);
	printf("SO_SNDTIMEO: %d,%d\n",(int)tv.tv_sec,(int)tv.tv_usec);

   // SO_PRIORITY
	// Set the protocol-defined priority for all packets to be sent on this  socket.   Linux  uses
	// this  value to order the networking queues: packets with a higher priority may be processed
	// first depending on the selected device queueing discipline.  For ip(7), this also sets  the
	// IP  type-of-service (TOS) field for outgoing packets.  Setting a priority outside the range
	// 0 to 6 requires the CAP_NET_ADMIN capability.

	tam=sizeof(val);
	getsockopt(sock,SOL_SOCKET,SO_PRIORITY, &val, (socklen_t *)&tam);
	printf("SO_PRIORITY: %d\n", val);


	// struct protoent *getprotoent(void);
	// struct protoent *getprotobyname(const char *name);
	// struct protoent *getprotobynumber(int proto);
	// void setprotoent(int stayopen);
	// void endprotoent(void);

	// The getprotoent() function reads the next line from the file /etc/protocols and returns a
	// structure  protoent  containing  the broken out fields from the line.  The /etc/protocols
	// file is opened if necessary.
	// The getprotobyname() function returns a protoent structure for the line from  /etc/proto-
	// cols that matches the protocol name name.
	// The  getprotobynumber()  function  returns a protoent structure for the line that matches
	// the protocol number number.
	// The setprotoent() function opens and rewinds the /etc/protocols  file.   If  stayopen  is
	// true (1), then the file will not be closed between calls to getprotobyname() or getproto-
	// bynumber().
	// The endprotoent() function closes /etc/protocols.
	// The protoent structure is defined in <netdb.h> as follows:
	// struct protoent {
	// 	char    *p_name;        // official protocol name
	// 	char    **p_aliases;    // alias list 
	// 	int     p_proto;        // protocol number 
	// }

	// TCP_MAXSEG
	// The maximum segment size for outgoing TCP packets.  If this option is set  before  con-
	// nection  establishment, it also changes the MSS value announced to the other end in the
	// initial packet.  Values greater than the (eventual) interface MTU have no effect.   TCP
	// will also impose its minimum and maximum bounds over the value provided.
	// the option level argument should be set to IPPROTO_TCP

/*
	tam=sizeof(val);
	// getsockopt(sock,IPPROTO_TCP,TCP_MAXSEG, &val, (socklen_t *)&tam);
	getsockopt(sock,IPPROTO_TCP,TCP_MAXSEG, &val, (socklen_t *)&tam);
	printf("TCP_MAXSEG: %d\n", val);
*/
	return 0;
}


/*

SOCKET OPTIONS
	To set or get a TCP socket option, call getsockopt(2) to read or setsockopt(2)  to  write  the
	option with the option level argument set to IPPROTO_TCP.  In addition, most IPPROTO_IP socket
	options are valid on TCP sockets. For more information see ip(7).

TCP_CORK
	If set, don't send out partial frames.  All queued partial frames  are  sent  when  the
	option  is  cleared  again.  This is useful for prepending headers before calling send-
	file(2), or for throughput optimization.  As currently implemented, there is a 200 mil-
	lisecond  ceiling  on the time for which output is corked by TCP_CORK.  If this ceiling
	is reached, then queued data is automatically transmitted.  This option can be combined
	with  TCP_NODELAY  only  since  Linux  2.5.71.   This option should not be used in code
	intended to be portable.

TCP_DEFER_ACCEPT
	Allows a listener to be awakened only when data arrives on the socket.  Takes an  inte-
	ger  value  (seconds),  this  can bound the maximum number of attempts TCP will make to
	complete the connection.  This option should not be used in code intended to be  portable.

TCP_INFO
	Used to collect information about this socket.  The kernel returns a struct tcp_info as
	defined in the file /usr/include/linux/tcp.h.  This option should not be used  in  code
	intended to be portable.

TCP_KEEPCNT
	The  maximum number of keepalive probes TCP should send before dropping the connection.
	This option should not be used in code intended to be portable.

TCP_KEEPIDLE
	The time (in seconds) the connection needs to remain idle  before  TCP  starts  sending
	keepalive  probes, if the socket option SO_KEEPALIVE has been set on this socket.  This
	option should not be used in code intended to be portable.

TCP_KEEPINTVL
	The time (in seconds) between individual keepalive probes.  This option should  not  be
	used in code intended to be portable.

TCP_LINGER2
	The  lifetime of orphaned FIN_WAIT2 state sockets.  This option can be used to override
	the system wide sysctl tcp_fin_timeout on this socket.  This is not to be confused with
	the  socket(7) level option SO_LINGER.  This option should not be used in code intended
	to be portable.

TCP_MAXSEG
	The maximum segment size for outgoing TCP packets.  If this option is set  before  con-
	nection  establishment, it also changes the MSS value announced to the other end in the
	initial packet.  Values greater than the (eventual) interface MTU have no effect.   TCP
	will also impose its minimum and maximum bounds over the value provided.

TCP_NODELAY
	If  set, disable the Nagle algorithm.  This means that segments are always sent as soon
	as possible, even if there is only a small amount of  data.   When  not  set,  data  is
	buffered  until there is a sufficient amount to send out, thereby avoiding the frequent
	sending of small packets, which results in  poor  utilization  of  the  network.   This
	option is overridden by TCP_CORK; however, setting this option forces an explicit flush
	of pending output, even if TCP_CORK is currently set.

TCP_QUICKACK
	Enable quickack mode if set or disable quickack mode if  cleared.   In  quickack  mode,
	acks  are  sent  immediately, rather than delayed if needed in accordance to normal TCP
	operation.  This flag is not permanent, it only enables a switch to  or  from  quickack
	mode.   Subsequent  operation  of the TCP protocol will once again enter/leave quickack
	mode depending on internal protocol processing and factors such as delayed ack timeouts
	occurring  and  data  transfer.   This option should not be used in code intended to be
	portable.

TCP_SYNCNT
	Set the number of SYN retransmits that TCP should send before aborting the  attempt  to
	connect.   It cannot exceed 255.  This option should not be used in code intended to be
	portable.

TCP_WINDOW_CLAMP
	Bound the size of the advertised window to this value.  The kernel  imposes  a  minimum
	size  of SOCK_MIN_RCVBUF/2.  This option should not be used in code intended to be por-
	table.




SOCKET OPTIONS
	These socket options can be set by using setsockopt(2) and read with getsockopt(2) with the socket
	level set to SOL_SOCKET for all sockets:

	SO_KEEPALIVE
		Enable  sending  of keep-alive messages on connection-oriented sockets.  Expects an integer
		boolean flag.

	SO_OOBINLINE
		If this option is enabled, out-of-band data  is  directly  placed  into  the  receive  data
		stream.   Otherwise  out-of-band  data  is  only passed when the MSG_OOB flag is set during
		receiving.

	SO_RCVLOWAT and SO_SNDLOWAT
		Specify the minimum number of bytes in the buffer until the socket layer will pass the data
		to the protocol (SO_SNDLOWAT) or the user on receiving (SO_RCVLOWAT).  These two values are
		initialised to 1.  SO_SNDLOWAT is not changeable on Linux (setsockopt fails with the  error
		ENOPROTOOPT).   SO_RCVLOWAT  is changeable only since Linux 2.4.  The select(2) and poll(2)
		system calls currently do not respect the SO_RCVLOWAT setting on Linux, and mark  a  socket
		readable  when  even a single byte of data is available.  A subsequent read from the socket
		will block until SO_RCVLOWAT bytes are available.

	SO_RCVTIMEO and SO_SNDTIMEO
		Specify the receiving or sending timeouts until reporting an error.   The  parameter  is  a
		struct  timeval.   If  an input or output function blocks for this period of time, and data
		has been sent or received, the return value of that function will be  the  amount  of  data
		transferred;  if  no  data has been transferred and the timeout has been reached then -1 is
		returned with errno set to EAGAIN or EWOULDBLOCK just as if the socket was specified to  be
		nonblocking.   If  the  timeout  is set to zero (the default) then the operation will never
		timeout.

	SO_BSDCOMPAT
		Enable BSD bug-to-bug compatibility.  This is used by the UDP protocol module in Linux  2.0
		and  2.2.   If enabled ICMP errors received for a UDP socket will not be passed to the user
		program.  In later kernel versions, support for this option has been phased out: Linux  2.4
		silently  ignores it, and Linux 2.6 generates a kernel warning (printk()) if a program uses
		this option.  Linux 2.0 also enabled BSD bug-to-bug compatibility  options  (random  header
		changing,  skipping  of  the broadcast flag) for raw sockets with this option, but that was
		removed in Linux 2.2.

	SO_PASSCRED
		Enable or disable the receiving of the SCM_CREDENTIALS control message. For  more  informa-
		tion see unix(7).

	SO_PEERCRED
		Return  the credentials of the foreign process connected to this socket.  This is only pos-
		sible for connected PF_UNIX stream sockets and PF_UNIX stream  and  datagram  socket  pairs
		created  using socketpair(2); see unix(7).  The returned credentials are those that were in
		effect at the time of the call to connect(2) or socketpair(2).  Argument is a ucred  struc-
		ture.  Only valid as a getsockopt().

	SO_BINDTODEVICE
		Bind  this  socket to a particular device like "eth0", as specified in the passed interface
		name. If the name is an empty string or the option length is zero, the socket device  bind-
		ing  is  removed.  The  passed  option  is a variable-length null terminated interface name
		string with the maximum size of IFNAMSIZ.  If a socket is bound to an interface, only pack-
		ets  received  from  that  particular interface are processed by the socket. Note that this
		only works for some socket types, particularly AF_INET sockets. It  is  not  supported  for
		packet sockets (use normal bind(8) there).

	SO_DEBUG
		Enable socket debugging. Only allowed for processes with the CAP_NET_ADMIN capability or an
		effective user ID of 0.

	SO_REUSEADDR
		Indicates that the rules used in validating addresses supplied in  a  bind(2)  call  should
		allow  reuse  of  local  addresses.  For PF_INET sockets this means that a socket may bind,
		except when there is an active listening socket bound to the address.  When  the  listening
		socket  is bound to INADDR_ANY with a specific port then it is not possible to bind to this
		port for any local address.

	SO_TYPE
		Gets the socket type as an integer (like SOCK_STREAM).  Can only be read with getsockopt().

	SO_ACCEPTCONN
		Returns a value indicating whether or not this socket has been marked to accept connections
		with listen().  The value 0 indicates that this is not a  listening  socket,  the  value  1
		indicates that this is a listening socket.  Can only be read with getsockopt().

	SO_DONTROUTE
		Don't  send  via  a gateway, only send to directly connected hosts.  The same effect can be
		achieved by setting the MSG_DONTROUTE flag on a socket send(2) operation. Expects an  inte-
		ger boolean flag.

	SO_BROADCAST
		Set  or  get  the  broadcast flag. When enabled, datagram sockets receive packets sent to a
		broadcast address and they are allowed to send packets to a broadcast address.  This option
		has no effect on stream-oriented sockets.

	SO_SNDBUF
		Sets  or  gets  the maximum socket send buffer in bytes.  The kernel doubles this value (to
		allow space for bookkeeping overhead) when it is set using setsockopt(), and  this  doubled
		value is returned by getsockopt().  The default value is set by the wmem_default sysctl and
		the maximum allowed value is set by the wmem_max sysctl.  The minimum (doubled)  value  for
		this option is 2048.

	SO_RCVBUF
		Sets or gets the maximum socket receive buffer in bytes.  The kernel doubles this value (to
		allow space for bookkeeping overhead) when it is set using setsockopt(), and  this  doubled
		value is returned by getsockopt().  The default value is set by the rmem_default sysctl and
		the maximum allowed value is set by the rmem_max sysctl.  The minimum (doubled)  value  for
		this option is 256.

	SO_LINGER
		Sets or gets the SO_LINGER option. The argument is a linger structure.

		struct linger {
			int   l_onoff;    // linger active 
			int   l_linger;   // how many seconds to linger for 
		};

		When  enabled,  a close(2) or shutdown(2) will not return until all queued messages for the
		socket have been successfully sent or the linger timeout has been reached.  Otherwise,  the
		call  returns  immediately  and  the closing is done in the background.  When the socket is
		closed as part of exit(2), it always lingers in the background.

	SO_PRIORITY
		Set the protocol-defined priority for all packets to be sent on this  socket.   Linux  uses
		this  value to order the networking queues: packets with a higher priority may be processed
		first depending on the selected device queueing discipline.  For ip(7), this also sets  the
		IP  type-of-service (TOS) field for outgoing packets.  Setting a priority outside the range
		0 to 6 requires the CAP_NET_ADMIN capability.

	SO_ERROR
		Get and clear the pending socket error. Only valid as a getsockopt().  Expects an  integer.

	SO_TIMESTAMP
		Enable or disable the receiving of the SO_TIMESTAMP control message.  The timestamp control
		message is sent with level SOL_SOCKET and the cmsg_data field is a struct timeval  indicat-
		ing the reception time of the last packet passed to the user in this call.  See cmsg(3) for
		details on control messages.


IOCTLS
	These following ioctl(2) calls return information in value.  The correct syntax is:

	int value;
	error = ioctl(tcp_socket, ioctl_type, &value);

	ioctl_type is one of the following:

SIOCINQ
	Returns the amount of queued unread data in the receive buffer.  The socket must not be
	in LISTEN state, otherwise an error (EINVAL) is returned.

SIOCATMARK
	Returns  true  (i.e.,  value  is  non-zero) if the inbound data stream is at the urgent mark.

	If the SO_OOBINLINE socket option is set, and SIOCATMARK returns true,  then  the  next
	read from the socket will return the urgent data.  If the SO_OOBINLINE socket option is
	not set, and SIOCATMARK returns true, then the next read from the  socket  will  return
	the  bytes  following  the  urgent  data (to actually read the urgent data requires the
	recv(MSG_OOB) flag).

	Note that a read never reads across the urgent mark.  If an application is informed  of
	the  presence  of  urgent  data via select(2) (using the exceptfds argument) or through
	delivery of a SIGURG signal, then it can advance up to the  mark  using  a  loop  which
	repeatedly  tests  SIOCATMARK  and  performs a read (requesting any number of bytes) as
	long as SIOCATMARK returns false.

SIOCOUTQ
	Returns the amount of unsent data in the socket send queue.  The socket must not be  in
	LISTEN state, otherwise an error (EINVAL) is returned.

*/
