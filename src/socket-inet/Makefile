
PROGS=serv_udp cli_udp cli_udp2 serv_tcp cli_tcp select sockopt cli_bcast

CC=gcc -Wall

all:	$(PROGS)

clean:
	rm -f $(PROGS)

cli_udp:	cli_udp.c
	$(CC) cli_udp.c -o cli_udp

cli_udp2:	cli_udp2.c
	$(CC) cli_udp2.c -o cli_udp2

serv_udp:	serv_udp.c
	$(CC) serv_udp.c -o serv_udp

cli_tcp:	cli_tcp.c
	$(CC) cli_tcp.c -o cli_tcp

serv_tcp:	serv_tcp.c
	$(CC) serv_tcp.c -o serv_tcp

cli_bcast:	cli_bcast.c
	$(CC) cli_bcast.c -o cli_bcast

select:	select.c
	$(CC) select.c -o select

sockopt:	sockopt.c
	$(CC) sockopt.c -o sockopt
