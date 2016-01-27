#makefile for project 1 works both in linux and unix system now
CC=gcc
FLAGS=-Wall -pedantic -g -std=c1x
LINKS=-lnsl
CLIENT=SimpClient
SERVER=SimpServer
OBJCLIENT=client.o
OBJSERVER=server.o
OUT=$(SERVER) $(CLIENT)

# compile client and server
all:$(OUT)
	ctags *.c

# compile client only 
$(CLIENT): $(OBJCLIENT)
	$(CC) $(FLAGS) -o $@ $^ $(FLAGS)

#compile server only
$(SERVER): $(OBJSERVER)
	$(CC) $(FLAGS) -o $@ $^ $(FLAGS)

%.o: %.c
	$(CC) $(FLAGS) -c $^

clean:
	rm $(OBJCLIENT) $(OBJSERVER) $(OUT)

configuredebug: all
	cat client.c | sed 's/DEBUG [0-1]/DEBUG 1/'
	cat server.c | sed 's/DEBUG [0-1]/DEBUG 1/'

debug:
	gdb --args $(CLIENT) http://www.example.com

run: 
	./$(CLIENT) http://localhost/client.c:8000

test2:
	./mClient http://www.git.local:8888
