/*------------------------------
* client.c
* Description: HTTP client program
* CSC 361
* Instructor: Kui Wu
-------------------------------*/
#include <stdio.h>  // for printf
#include <stdlib.h> // to suppress warnings
#include <string.h> // for parsing the URI
#include <fcntl.h>  // open
#include <unistd.h> // close
#include <sys/socket.h> // socket functions
#include <sys/types.h>  // socket data structures

/* define maximal string and reply length, this is just an example.*/
/* MAX_RES_LEN should be defined larger (e.g. 4096) in real testing. */
#define MAX_STR_LEN 255
#define MAX_RES_LEN 255
#define DEBUG 1

void parse_URI(char *uri, char *hostname, int *port, char *identifier);
void perform_http(int sockid, char *identifier);
int open_connection(char *hostname, int port);
void debug(char * out);

/* --------- Main() routine ------------
 * three main task will be excuted:
 * accept the input URI and parse it into fragments for further operation
 * open socket connection with specified sockid ID
 * use the socket id to connect sopecified server
 * don't forget to handle errors
 */

int main(int argc, char **argv)
{
    char uri[MAX_STR_LEN];
    char hostname[MAX_STR_LEN];
    char identifier[MAX_STR_LEN];
    int sockid, port;
    if (argv[1] != NULL) {
      strcpy(uri, argv[1]);
    } else {
      puts("please enter a uri:");
      scanf("%s", uri);
    }

    printf("\nOpenning URI: %s\n", uri);

    parse_URI(uri, hostname, &port, identifier);
    printf("Hostname: %s\nport: %d\nidentifier: %s\n", hostname, port, identifier);
    sockid = open_connection(hostname, port);
    perform_http(sockid, identifier);
    return 0;
}

/*------ Parse an "uri" into "hostname" and resource "identifier" --------*/

void parse_URI(char *uri, char *hostname, int *port, char *identifier)
{
  char * d1 = ":";
  char * d2 = "/";
  char * d3 = ".";
  
  // resist mangling the input uri
  char * uri_cpy = (char *)malloc(sizeof(char) * strlen(uri));
  strcpy(uri_cpy, uri);

  // make sure we are parsing a http uri
  char * protocol = strtok(uri_cpy, d1);
  if (strcmp(protocol, "http") != 0) {
    puts("No protocol or bad protocol entered. Exiting program");
    exit(0);
  }

  char * domain = strtok(NULL, d2);

  char * id = strtok(NULL, d1);
  
  strcpy(identifier, id); 
  
  // get port from uri string
  char * port_str = strtok(NULL, d1);
  if (port_str != NULL) {
    *port = atoi(port_str);
    if (*port == 0) *port = 80; 
  } else {
    *port = 80;
  }

  char * d_cpy = (char *)malloc(sizeof(char) * strlen(domain));
  strcpy(d_cpy, domain);

  // check for www in front of domain
  char * top_d = strtok(d_cpy, d3);
  if (strcmp(top_d, "www") == 0) {
    strcpy(hostname, strtok(NULL, d3));
  } else {
    strcpy(hostname, top_d);
  }
  
  if (DEBUG) {
    printf("INFO: Host %s\n Identifier %s\n Port %d\n", hostname, identifier, *port);
  }
}

/*------------------------------------*
* connect to a HTTP server using hostname and port,
* and get the resource specified by identifier
*--------------------------------------*/
void perform_http(int sockid, char *identifier)
{
  /* connect to server and retrieve response */

   close(sockid);
}

/*---------------------------------------------------------------------------*
 *
 * open_conn() routine. It connects to a remote server on a specified port.
 *
 *---------------------------------------------------------------------------*/

int open_connection(char *hostname, int port)
{

  //int sockfd;
  /* generate socket
   * connect socket to the host address
   */
  return 0;
}

void debug(char * out) 
{
  if (DEBUG) puts(out);
}
