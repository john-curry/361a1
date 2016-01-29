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

 /* Function: parse_URI

  * Args: an http uri. Quits if there is no http header specified
          at the beginning of the uri.

  * Return: hostname, port and identifier. Quits if there is no hostname. Port defaults to 80 and identifier defaults to index.html

  * Notes: 
*/

void parse_URI(char *uri, char *hostname, int *port, char *identifier);

/*
  *
  *
  *
*/

void perform_http(int sockid, char *hostname, char *identifier);

int open_connection(char *hostname, int port);
void debug(char * out);
int sockid;
void interupt_handler(int);
