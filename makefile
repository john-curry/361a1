#makefile for project 1 works both in linux and unix system now
CC=gcc
FLAGS=-Wall -pedantic -g -std=c11
.c.o:
	$(CC) $(FLAGS) -c $?

# compile client and server
all: mClient mServer
	ctags *.c

# compile client only 
mClient: client.o  util.o
	$(CC) $(FLAGS) -o mClient client.o  util.o  -lnsl

#compile server only
mServer: server.o util.o
	$(CC) $(FLAGS) -o mServer server.o util.o  -lnsl

clean:
	rm *.o mClient mServer

debug: all
	cat client.c | sed 's/DEBUG [0-1]/DEBUG 1/'
	cat server.c | sed 's/DEBUG [0-1]/DEBUG 1/'

test2:
	./mClient http://www.git.local:8888

test:
	./mClient http://www.example.com/curse.html:80
	./mClient smtp://www.whatever.com
	./mClient http://www.example.com/curse.html:8080
	./mClient http://www.example.com/curse.html
	./mClient http://example.com/curse:90
	./mClient http://example.com/curse.html
	./mClient http://example.com/curse.html:90
