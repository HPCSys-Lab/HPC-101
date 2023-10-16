
       It is possible to do non-blocking IO on sockets by setting the O_NONBLOCK flag on  a  socket  file
       descriptor using fcntl(2).  Then all operations that would block will (usually) return with EAGAIN
       (operation should be retried later); connect(2) will return EINPROGRESS error.  The user can  then
       wait for various events via poll(2) or select(2).

       +--------------------------------------------------------------------+
       |                            I/O events                              |
       +-----------+-----------+--------------------------------------------+
       |Event      | Poll flag | Occurrence                                 |
       +-----------+-----------+--------------------------------------------+
       |Read       | POLLIN    | New data arrived.                          |
       +-----------+-----------+--------------------------------------------+
       |Read       | POLLIN    | A connection setup has been completed (for |
       |           |           | connection-oriented sockets)               |
       +-----------+-----------+--------------------------------------------+
       |Read       | POLLHUP   | A disconnection request has been initiated |
       |           |           | by the other end.                          |
       +-----------+-----------+--------------------------------------------+
       |Read       | POLLHUP   | A  connection  is broken (only for connec- |
       |           |           | tion-oriented protocols).  When the socket |
       |           |           | is written SIGPIPE is also sent.           |
       +-----------+-----------+--------------------------------------------+
       |Write      | POLLOUT   | Socket  has  enough  send buffer space for |
       |           |           | writing new data.                          |
       +-----------+-----------+--------------------------------------------+
       |Read/Write | POLLIN|   | An outgoing connect(2) finished.           |
       |           | POLLOUT   |                                            |
       +-----------+-----------+--------------------------------------------+
       |Read/Write | POLLERR   | An asynchronous error occurred.            |
       +-----------+-----------+--------------------------------------------+
       |Read/Write | POLLHUP   | The other end has shut down one direction. |
       +-----------+-----------+--------------------------------------------+
       |Exception  | POLLPRI   | Urgent data arrived.  SIGURG is sent then. |
       +-----------+-----------+--------------------------------------------+

       An alternative to poll() and select() is to let the kernel inform the application about events via
       a SIGIO signal. For that the FASYNC flag must be set on a socket file descriptor via fcntl(2)  and
       a  valid  signal handler for SIGIO must be installed via sigaction(2).  See the SIGNALS discussion
       below.


