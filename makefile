#makefile for project 1 works both in linux and unix system now
CC=gcc
FLAGS=-pthread -Wall -pedantic -g -std=c1x
LINKS=-lnsl -lpthreads
CLIENT=SimpClient
SERVER=SimpServer
OBJCLIENT=client.o
OBJSERVER=server.o
OUT=$(SERVER) $(CLIENT)

# compile client and server
all:$(OUT)
	ctags *.c

$(CLIENT): $(OBJCLIENT)
	$(CC) $(FLAGS) -o $@ $^ $(FLAGS)

$(SERVER): $(OBJSERVER)
	$(CC) $(FLAGS) -o $@ $^ $(FLAGS)

%.o: %.c
	$(CC) $(FLAGS) -c $^

clean:
	rm $(OBJCLIENT) $(OBJSERVER) $(OUT)

configuredebug: all
	cat client.c | sed 's/DEBUG [0-1]/DEBUG 1/'
	cat server.c | sed 's/DEBUG [0-1]/DEBUG 1/'

cdebug:
	gdb --args $(CLIENT) http://www.example.com:80/index.html

sdebug:
	gdb --args $(SERVER) 8000 .

crun: 
	./$(CLIENT) http://www.example.com:80/index.html

srun:
	./$(SERVER) 8000 .
