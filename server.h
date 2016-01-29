
#define DEBUG 1 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h> 
#include <time.h> 
#include <stdbool.h>
#include <signal.h>
#include <pthread.h>
#include <errno.h>

typedef struct _thread_args {
  int comm_fd;
  char * directory;
} thread_args;

int start_server(int);
int find_connection(int l_fd);
void *perform_http(void*);
void mtime(char response[]);
const int MAX_RES_LEN = 10000; // large number
void interupt_handler(int);
void m_free(void*);
void append_file(char*, FILE*);
void build_header(char response[], char * status);
int listen_fd;
void clean_exit();
