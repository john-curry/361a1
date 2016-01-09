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
#define MAX_STR_LEN 120
#define MAX_RES_LEN 120
#define DEBUG 0

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

    printf("\nOpenning URI: %s.\n", uri);

    parse_URI(uri, hostname, &port, identifier);
    sockid = open_connection(hostname, port);
    perform_http(sockid, identifier);
    return 0;
}

/*------ Parse an "uri" into "hostname" and resource "identifier" --------*/

void parse_URI(char *uri, char *hostname, int *port, char *identifier)
{
  const char delim[2] = ":";
   
  // make sure we are parsing a http uri
  if (strcmp(strtok(uri, delim), "http") != 0) {
    puts("not an http protocol\n");
    return;
  }

  // get url from uri
  char * url = strtok(NULL, delim);
  
  char * port_str = strtok(NULL, delim);

  if (port_str == NULL) {
    *port = 80;
  } else {
    *port = atoi(port_str);
    if (*port == 0) *port = 80; // default to port 80 if no port is given
  }
   
  if (DEBUG) printf("URL: %s\n", url);

  // parse url
  if (url != NULL) {
    const char delim_two[2] = "/";

    // get domain name from url 
    char * domain = strtok(url, delim_two);

    identifier = strtok(NULL, delim_two);
     
    if (DEBUG) printf("Domain: %s\n", domain);

    if (domain != NULL) {
      const char delim_three[2] = ".";

      char * top_level = strtok(domain, delim_three);

      if (DEBUG) printf("Top level domain: %s\n", top_level);

      if (top_level == NULL) {
        puts("something went wrong with out domain name\n");
        return;
      }

      // check to see of the hostname is preceded by www
      if (strcmp(top_level, "www") == 0 ) {
        hostname = strtok(NULL, delim_three);
      } else {
        hostname = top_level;
      }
    } else { 
      puts("no domain name found\n"); 
      return;
    }
    
  } else { 
    puts("no url name found.\n"); 
    return;
  }

  if (DEBUG) {
    printf("Host %s\n Identifier %s\n Port %d\n", hostname, identifier, *port);
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
