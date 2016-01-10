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
char *URI_to_URL(char *uri);

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
    sockid = open_connection(hostname, port);
    perform_http(sockid, identifier);
    return 0;
}

int URI_to_port(char *uri);
char *URL_to_domain(char* url);
char *domain_to_host(char* domain);
char *URL_to_identifier(char *URL);

char *URL_to_identifier(char *in) {
  const char delim[2] = "/";
  int length = strlen(in);

  char url[length + 1];

  strcpy(url, in); 

  strtok(url, delim);

  char * identifier = strtok(NULL, delim);

  char * ret = (char*)malloc((strlen(identifier) + 1) * sizeof(char));

  strcpy(ret, identifier);

  return ret;

}

char *URL_to_domain(char* in) {
  const char delim[2] = "/";
  int length = strlen(in);
  char url[length];
  strcpy(url, in);
  char * domain = strtok(url, delim);
  char * ret = (char*)malloc((strlen(domain)) * sizeof(char));
  strcpy(ret, domain);
  return ret;
}
/* 
---------------------------------------------------
Returns the 0 if no port is found in the URI
 --------------------------------------------------
*/
int URI_to_port(char *in) {
  const char delim[2] = ":";

  int length = strlen(in);

  char uri[length];

  int port = 0;

  strtok(uri, delim); // discard protocol

  strtok(NULL, delim); // discard url

  char * port_str = strtok(NULL, delim);

  if (port_str != NULL) {
    port = atoi(port_str);
  }
  if (port == 0 && DEBUG) printf("ERROR: No port found.\n");
  return port;
}
/*
 -------------------------------------------------------------------
  Returns NULL of URL cannot be determined.
  Don't forget to free the return value when you are done!
 -------------------------------------------------------------------
*/
char *URI_to_URL(char *in) {
  const char delim[2] = ":";
  char uri[MAX_STR_LEN];
  
  strcpy(uri, in); 

  if (DEBUG) printf("INFO: URI -> %s\n", uri);

  // make sure we are parsing a http uri
  if (strcmp(strtok(uri, delim), "http") != 0) {
    if (DEBUG) puts("ERROR: Not an http protocol.\n");
    return NULL;
  }

  // get url from uri
  const char * url = strtok(NULL, delim);
  
  if (DEBUG) printf("INFO URL -> %s\n", url);

  char * ret = (char *)malloc(strlen(url) * sizeof(char));

  strcpy(ret, url);

  
  if (DEBUG) printf("INFO: URL -> %s\n", ret);
  return ret;
}

/*------ Parse an "uri" into "hostname" and resource "identifier" --------*/

void parse_URI(char *uri, char *hostname, int *port, char *identifier)
{
  char * url = URI_to_URL(uri); 

  *port = URI_to_port(uri); 

  if (*port == 0) *port = 80;

  if (DEBUG) printf("INFO: URL -> %s\n", url);

  // parse url
  if (url != NULL) {
    char * domain = URL_to_domain(url);
  
    identifier = URL_to_identifier(url);
     
    if (DEBUG) printf("INFO: Domain -> %s\n", domain);
    if (DEBUG) printf("INFO: Identifier -> %s\n", identifier);

    if (domain != NULL) {
      const char delim_three[2] = ".";

      char * top_level = strtok(domain, delim_three);

      if (DEBUG) printf("INFO: Top level domain -> %s\n", top_level);

      if (top_level == NULL) {
        puts("ERROR: Something went wrong with out domain name.\n");
        return;
      }

      // check to see of the hostname is preceded by www
      if (strcmp(top_level, "www") == 0 ) {
        hostname = strtok(NULL, delim_three);
      } else {
        hostname = top_level;
      }
    } else { 
      puts("ERROR: No domain name found.\n"); 
      return;
    }
    
  } else { 
    puts("ERROR: No url name found.\n"); 
    return;
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
