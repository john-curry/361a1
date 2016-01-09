#makefile for project 1 works both in linux and unix system now
CC=clang
FLAGS=-Wall -pedantic -g
.c.o:
	$(CC) $(FLAGS) -c $?

# compile client and server
all: client server

# compile client only 
client: client.o  util.o
	$(CC) $(FLAGS) -o client client.o  util.o  -lnsl

#compile server only
server: server.o util.o
	$(CC) $(FLAGS) -o server server.o util.o  -lnsl
