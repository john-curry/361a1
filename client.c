/*------------------------------
* client.c
* Description: HTTP client program
* CSC 361
* Instructor: Kui Wu
-------------------------------*/

#include <stdio.h>  // for printf
#include <stdlib.h> // to suppress warnings
#include <string.h> // for parsing the URI
#include <strings.h> // bzero
#include <fcntl.h>  // open
#include <unistd.h> // close
#include <stdbool.h>
#include <errno.h>
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

void parse_URI(char *uri, char *hostname, int *port, char *identifier);
void perform_http(int sockid, char *hostname, char *identifier);
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

    printf("Hostname: %s port: %d identifier: %s\n", hostname, port, identifier);

    sockid = open_connection(hostname, port);
    
    printf("Hostname: %s\nport: %d\nidentifier: %s\n", hostname, port, identifier);

    perform_http(sockid, hostname, identifier);

    return 0;
}

/*------ Parse an "uri" into "hostname" and resource "identifier" --------*/

void parse_URI(char *uri, char *hostname, int *port, char *identifier)
{
  char * d1 = ":";
  char * d2 = "/";
    
  // resist mangling the input uri
  char * uri_cpy = (char *)malloc(sizeof(char) * strlen(uri));
  strcpy(uri_cpy, uri);

  // make sure we are parsing an http uri
  char * protocol = strtok(uri_cpy, d1); // read up to the first colon "/"

  if (strcmp(protocol, "http") != 0) {
    puts("No protocol or bad protocol entered. Exiting program");
    exit(0);
  }

  char * host = strtok(NULL, d2); // read up to the first slash "/"
  
  while (host[0] == '/') host++; // get rid of the leading slash slash "//"

  char * id = strtok(NULL, d1); // the id is beteen the end of the host and the first colon ":"
  // if there is no id then there will be no '/' and just a straight colon ':' 
  // the port is from the id to the end of the line or from the last ':' to the end of line
  
  if (id == NULL) {
  }

  char * port_str = strtok(NULL, d1);
  if (port_str == NULL) {
    *port = 80;
  } else {
    *port = atoi(port_str);
    if (*port == -1) {
      *port = 80;
    }
  }
  if (id != NULL) {
    strcpy(identifier, id);
  } else {
    identifier = NULL;
  }
  if (host != NULL) strcpy(hostname, host);

  if (DEBUG) {
   // printf("INFO: Host %s\n Identifier %s\n Port %d\n", hostname, identifier, *port);
  }
}

/*------------------------------------*
* I am assuming that the we have already called
* connect on the socket and we are connected to 
* the server.
*--------------------------------------*/
void perform_http(int sockfd, char* hostname, char *identifier)
{  
    char sendline[MAX_STR_LEN];
    char recvline[MAX_STR_LEN];
    
    //while (hostname[0] == 'w' || hostname[0] == '.') hostname++;
    char * header;
    char * method = "GET ";
    char * http_version = " HTTP/1.0\r\n\r\n";
    char * host_field = "Host: ";
    char * host_field_with_arguement = malloc(sizeof(char) * (strlen(host_field) + strlen(hostname) + 1));

    char * connection_field = "\nConnection: Keep-Alive\n";

    strcpy(host_field_with_arguement, host_field);
    strcat(host_field_with_arguement, hostname);

    if (identifier != NULL) {
      header = (char *)malloc(sizeof(char) * (
        strlen(method) +
        strlen(identifier) +
        strlen(http_version) + 
        strlen(host_field) +
        strlen(connection_field) + 1)
      );

      strcpy(header, method); 
      strcat(header, identifier);
      strcat(header, http_version);
      strcat(header, host_field_with_arguement);
      strcat(header, connection_field);
    } else {
      header = (char *)malloc(sizeof(char) * (
        strlen(method) +
        strlen(http_version) + 
        strlen(host_field) + 1)
      );
      strcpy(header, method); 
      strcat(header, http_version);
      strcat(header, host_field_with_arguement);
    }

    bool done_recieving = false;
    bool header_sent = false;
    if (DEBUG) { puts("INFO: sending header."); }

    while(!done_recieving) {
        int bytes_read = 0;
        bzero( sendline, MAX_STR_LEN);
        bzero( recvline, MAX_STR_LEN);

        if (!header_sent) {
          header_sent = true;
          puts("---Request begin---");
          puts(header);
          write(sockfd,header,strlen(header)+1);
          puts("---Request end---");
          puts("HTTP request sent, awaiting response...");
        }

        bytes_read = read(sockfd,recvline,100);

        if (bytes_read == 0) {
          done_recieving = true;
        }

        printf("%s",recvline);
    }
    puts("--Done recieving---");
   close(sockfd);
}

/*---------------------------------------------------------------------------*
 *
 * open_conn() routine. It connects to a remote server on a specified port.
 *
 *---------------------------------------------------------------------------*/

int open_connection(char *hostname, int port)
{
  int sockfd;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  if (DEBUG) { puts("INFO: getting sockfd\n"); }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    puts("ERROR: Could not open socket\n");
    exit(1);
  }

  if (DEBUG) { printf("INFO: calling gethostbyname for host %s.\n", hostname); }

  server = gethostbyname(hostname);
  
  if (server == NULL) {
    printf("ERROR: Recieved error *** %s ***\n", strerror(h_errno));
    exit(1);
  }

  if (DEBUG) { printf("INFO: connecting to server %s on port %d\n", server->h_name, port); }

  if (DEBUG) { puts("INFO: managing memory..\n"); }
  memset((char *) &serv_addr, 0, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  memcpy((char *)&serv_addr.sin_addr.s_addr,
         (char *)server->h_addr_list[0],
         server->h_length);

  serv_addr.sin_port = htons(port);

  if (DEBUG) { puts("INFO: connecting to server.\n"); }

  if (connect(sockfd, (struct sockaddr *) & serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR: Could not connect to a server. Recieved error *** %s ***\n", strerror(errno));
    exit(1);
  }
  if (DEBUG) { puts("INFO: Connected to server."); }
  return sockfd;
}

void debug(char * out) 
{
  if (DEBUG) puts(out);
}
