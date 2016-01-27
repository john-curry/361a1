/*Required Headers*/
 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h> 
#include <time.h> 
char * mtime() {
  struct tm *tm;
  time_t t;
  char str_time[100];
  char str_date[100];
  t = time(NULL);
  tm = localtime(&t);

  strftime(str_time, sizeof(str_time), "%H:%M:%S GMT ", tm);
  strftime(str_date, sizeof(str_date), "Date: %a, %d %b %Y", tm);
  strcat(str_date, str_time);
  //printf("Got time %s\n", str_date);
  char * ret = (char *)malloc(strlen(str_date) * sizeof(char));
  strcpy(ret, str_date);
  strcat(ret, "\n");
  return ret;
}
int main(int argc, char** argv) {

    if (argc < 1) {
      perror("We need a port number to operate");
      exit(1);
    }
 
    char response[100];
    char recieved[100];

    char * http_response = "HTTP/1.1 ";
    char * ok_response = "200 OK\n";
    char * serv_info = "Server: SimServer/0.0.1 (Linux)\n";
    char * current_time = mtime();

    int listen_fd, comm_fd;
 
    struct sockaddr_in servaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
 
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    listen(listen_fd, 10);
 
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);
 
    while(1) {
 
        bzero( recieved, 100);
        bzero( response, 100);
 
        read(comm_fd,recieved,100);
        
        if (strncmp("GET", recieved, 3) == 0) {
          strcpy(response, http_response);
          strcat(response, ok_response);
          strcat(response, current_time);
          strcat(response, serv_info);
          strcat(response, "\r\n\r\n");

          write(comm_fd, response, strlen(response)+1);
          printf("Revieved a GET responding with- %s\n",response);
        } else {
          write(comm_fd, recieved, strlen(recieved)+1);
          printf("Echoing back - %s\n", recieved);
        }

 
 
    }
}
