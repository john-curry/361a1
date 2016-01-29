/*
    Author: John Curry
    Student Number: V00755720
    File: client.h
    Description: HTTP/1.0 GET requeset sender
*/

#include <stdio.h>  // for printf
#include <stdlib.h> // to suppress warnings
#include <string.h> // for parsing the URI
#include <strings.h> // bzero
#include <fcntl.h>  // open
#include <unistd.h> // close
#include <stdbool.h>
#include <errno.h>
#include <signal.h>
#include <assert.h>
#include <sys/socket.h> // socket functions
#include <sys/types.h>  // socket data structures
#include <netinet/in.h>
#include <netdb.h>

/* define maximal string and reply length, this is just an example.*/
/* MAX_RES_LEN should be defined larger (e.g. 4096) in real testing. */
#define MAX_STR_LEN 255
#define MAX_RES_LEN 255
#define DEBUG 1

// this is here so the interupt handler can close it when something stops the program
int sockid;

 /* Function: parse_URI

  * Args: an http uri. Quits if there is no http header specified
          at the beginning of the uri.

  * Return: hostname, port and identifier. Quits if there is no hostname. Port defaults to 80 and identifier defaults to index.html

  * Notes: 
*/

void parse_URI(char *uri, char *hostname, int *port, char *identifier);

/*
  * Function: perform_http

  * Args: Socket id, hostname of server and identifier of resource we are looking for

  * Return: N/A
*/

void perform_http(int sockid, char *hostname, char *identifier);

/*
  * Function: open_connection

  * Args: hostname of the server and port that we are connecting to on the server

  * Return: the socket file descriptor
*/
int open_connection(char *hostname, int port);

void debug(char * out);
void interupt_handler(int);
