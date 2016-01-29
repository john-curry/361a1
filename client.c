/*------------------------------
* client.c
* Description: HTTP client program
* CSC 361
* Instructor: Kui Wu
-------------------------------*/

#include "client.h"

int main(int argc, char **argv)
{
    signal(SIGINT, interupt_handler);

    char uri[MAX_STR_LEN];
    char hostname[MAX_STR_LEN];
    char identifier[MAX_STR_LEN];
    int port;

    if (argc != 1) {
      strcpy(uri, argv[1]);
    } else {
      puts("ERROR: SimpClient requires a uri as a arguement.\n");
      exit(1);
    }

    printf("\nOpenning URI: %s\n", uri);

    parse_URI(uri, hostname, &port, identifier);

    printf("Hostname: %s port: %d identifier: %s\n", hostname, port, identifier);

    sockid = open_connection(hostname, port);
    
    printf("Hostname: %s\nport: %d\nidentifier: %s\n", hostname, port, identifier);

    perform_http(sockid, hostname, identifier);

    return 0;
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
    assert(identifier != NULL); 
    assert(hostname != NULL); 

    char * header;
    char * method = "GET ";
    char * http_version = " HTTP/1.0\r\n\r\n";
    //char * host_arg = "Host: "; 
    //char * end = "\r\n \r\n \r\n";
     header = (char *)malloc(sizeof(char) * (
      strlen(method) +
      strlen("http://") +
      strlen(hostname) +
      strlen("/") +
      strlen(identifier) +
      strlen(http_version) + 1
    ));

    strcpy(header, method); 
    strcat(header, "http://");
    strcat(header, hostname); 
    strcat(header, "/");
    strcat(header, identifier);
    strcat(header, http_version);
    //strcat(header, host_arg);
    //strcat(header, hostname);
    //strcat(header, end);

    bool done_recieving = false;
    bool _sent = false;
    if (DEBUG) { puts("INFO: sending header.\n"); }

    while(!done_recieving) {
        bzero( sendline, MAX_STR_LEN);

        if (!_sent) {
          _sent = true;
          puts("---Request begin---\n");
          puts(header);
          write(sockfd,header,strlen(header)+1);
          puts("---Request end---\n");
          puts("HTTP request sent, awaiting response...\n");
        }

        int bytes_read = -1;
        while (bytes_read != 0) {

          bzero( recvline, MAX_STR_LEN);
          bytes_read = read(sockfd,recvline,100);

          if(bytes_read < 0) {
           printf("ERROR: Recieving bytes from server %s\n",strerror(errno)); 
           break;
          } else {
          printf("%s",recvline);
          }
        }

        if (_sent) {
          done_recieving = true;
        }
   }
   //free(host_field_with_arguement);
   free(header);
   puts("--Done recieving---\n");
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

void interupt_handler(int p) {
  close(sockid);
}

void debug(char * out) 
{
  if (DEBUG) puts(out);
}
