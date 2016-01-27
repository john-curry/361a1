/*Required Headers*/
 
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

int start_server(int);
void perform_http(int comm_fd, DIR * directory);
char * mtime();


int main(int argc, char** argv) {

    if (argc < 2) {
      perror("We need a port number and the directory to operate");
      exit(1);
    }
    
    DIR* directory; 
    
    directory = opendir(argv[2]); 

    if (directory == NULL) {
      puts("ERROR: Could not open directory.\n");
      exit(1);
    }

    int comm_fd = start_server(atoi(argv[1])); 

    perform_http(comm_fd, directory);
 
}

int start_server(int port) {
    int listen_fd, comm_fd;
 
    struct sockaddr_in servaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);
 
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    listen(listen_fd, 3);
 
    comm_fd = accept(listen_fd, (struct sockaddr*) NULL, NULL);

    return comm_fd;
}

void perform_http(int comm_fd, DIR * directory) {
  char response[100];
  char recieved[100];

  struct dirent * in_file = NULL; 
  char * status = NULL;
  bool file_found = false;
  char * http_response = "HTTP/1.0 ";
  char * ok_response = "200 OK.\r\n\r\n";
  char * not_found = "404 Not Found.\r\n\r\n";
  char * not_implemented = "505 Not Implemented.\r\n\r\n";

  char * data = "<!DOCTYPE html><html><head>Someones age is how long you have known them.</head><body></body></html>";
  char * serv_info = "Server: SimServer/0.0.1 (Linux)\n";
  char * current_time = mtime();

  while(1) {
      bzero( recieved, 100);
      bzero( response, 100);

      read(comm_fd,recieved,100);
      char * method = strtok(recieved, " ");
       
      if (strcmp("GET", method) == 0) {
        char * file = strtok(NULL, " "); 
        
        while (NULL != (in_file = readdir(directory))) {
          if (strcmp(in_file->d_name, file) == 0) { 
            status = ok_response;
            file_found = true;
          }
        }
        if (status == NULL) {
          status = not_found;
        }

        printf("Revieved a GET responding with - %s\n",response);
      } else {
        status = not_implemented;
        printf("Revieved inimplented method with - %s\n",response);
      }

      
      strcpy(response, http_response);
      strcat(response, status);
      strcat(response, current_time);
      strcat(response, serv_info);
      strcat(response, "\r\n\r\n");

      if (file_found) {
        // read file and send it over the line
      }
      write(comm_fd, recieved, strlen(recieved)+1);
      file_found = false;
   }
}

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
