/*
    Author: John Curry
    Student Number: V00755720
    File: server.h
    Description: Multi-threaded HTTP/1.0 GET request only server
*/

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

#define DEBUG 1 
#define MAX_CONNECTIONS 3 // change this for more connections
#define MAX_RES_LEN 10000 // large number

int listen_fd; // global listening file descriptor is global so it can be closed on interupt

// this struct is passed to perform_http through the pthread_create api
typedef struct _thread_args {
  int comm_fd;
  char * directory;
} thread_args;


 /* Function: start_server

  * Args: port number

  * Return: file descriptor of the listening socket

  * Notes: gauranteed to complete when socket is made
*/
int start_server(int);

 /* Function: find_connection

  * Args: file descriptor of the listening socket

  * Return: file descriptor of the communication socket

  * Notes: gauranteed to return when client is found. Hangs otherwise
*/
int find_connection(int l_fd);

 /* Function: perform_http 

  * Args: thread_arg passed as a void*

  * Return: exit status as a void*

  * Notes: method conforms to pthread calling standard as it is passed to pthread_create to make server multithreaded.
*/
void *perform_http(void*);

void mtime(char response[]);
void interupt_handler(int);
void m_free(void*);
void append_file(char* header, FILE*);
void build_header(char response[], char * status);
void clean_exit();
void clean();
