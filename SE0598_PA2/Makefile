CC=gcc
CFLAGS=-Wall -Wextra -std=c11

all: server client

server: TCP_Server.c
	$(CC) $(CFLAGS) -o server TCP_Server.c

client: TCP_Client.c
	$(CC) $(CFLAGS) -o client TCP_Client.c

clean:
	rm -f server client
