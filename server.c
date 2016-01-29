#include "server.h"

int main(int argc, char** argv) {
    signal(SIGINT, interupt_handler);
    signal(SIGABRT, interupt_handler);
    signal(SIGSEGV, interupt_handler);
    
    if (argc < 2) {
      perror("We need a port number and the directory to operate");
      exit(1);
    }
        
    char *directory = argv[2];

    listen_fd = start_server(atoi(argv[1]));

    if (listen_fd < 0) {
      puts("ERROR: Could not open a port to listen on.\n");
      clean_exit();
    }

    int new_comm_fd; 
    while ((new_comm_fd = find_connection(listen_fd)) > 0) {

      puts("INFO: Connected to client.\n"); 

      pthread_t thread_id;

      thread_args args = { .directory = directory, .comm_fd = new_comm_fd };

      puts("INFO: Creating pthread.\n");

      if (pthread_create(&thread_id, NULL, &perform_http,(void*) &args) != 0) {
        puts("ERROR: Could not create thread.\n");
        close(listen_fd);
        close(new_comm_fd);
        exit(1);
      }
      puts("INFO: DOne creating pthread.\n");
    }
    close(listen_fd); 
}

int start_server(int port) {
    puts("INFO: Starting server.\n");
    int listen_fd;
 
    struct sockaddr_in servaddr;
 
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
 
    bzero( &servaddr, sizeof(servaddr));
 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htons(INADDR_ANY);
    servaddr.sin_port = htons(port);
 
    bind(listen_fd, (struct sockaddr *) &servaddr, sizeof(servaddr));
 
    puts("INFO: listening server.\n");

    listen(listen_fd, MAX_CONNECTIONS);
    
    return listen_fd; 
}

int find_connection(int l_fd) {
    int comm_fd;
    puts("INFO: accepting client.\n");

    comm_fd = accept(l_fd, (struct sockaddr*) NULL, NULL);

    puts("INFO: accepted client.\n");

    return comm_fd;
}

void mtime(char response[]) {
  struct tm *tm;
  time_t t;
  char str_time[100];
  char str_date[100];
  t = time(NULL);
  tm = localtime(&t);

  strftime(str_time, sizeof(str_time), "%H:%M:%S GMT ", tm);
  strftime(str_date, sizeof(str_date), "Date: %a, %d %b %Y", tm);
  strcat(str_date, str_time);
  strcat(response, str_date);
  strcat(response, "\n");
}

void build_header(char response[], char * status) {
  printf("INFO: Building header for status: %s.\n", status);
   
  char * http_response = "HTTP/1.0 ";
  char * serv_info = "Server: SimServer/0.0.1 (Linux)\n";

  printf("INFO: adding http response %s\n", http_response);
  strcpy(response, http_response);
  printf("INFO: adding http status %s\n", status);
  strcat(response, status);
  puts("INFO: adding server time.\n");
  mtime(response);
  printf("INFO: adding server info %s\n", serv_info);
  strcat(response, serv_info);
  strcat(response, "\r\n\r\n");
  puts("INFO: Done building header.\n");
}

void *perform_http(void * vargs) {
  puts("INFO: Doing the http thang.\n");
  thread_args* args = vargs;

  int comm_fd = args->comm_fd;
  DIR * directory = opendir(args->directory);
  if (directory == NULL) {
    printf("ERROR: Trouble opening directory %s\n", strerror(errno));
    close(comm_fd);
    clean_exit();
  }
  if (comm_fd < 1) { 
    close(comm_fd);
    clean_exit();
  }

  char response[MAX_RES_LEN];
  char recieved[MAX_RES_LEN];
  FILE * file;
  struct dirent * in_file = NULL; 
  char * status = NULL;
  bool file_found = false;
  char * ok_response = "200 OK.\r\n\r\n";
  char * not_found = "404 Not Found.\r\n\r\n";
  char * not_implemented = "505 Not Implemented.\r\n\r\n";

      puts("INFO: Starting server.\n");
      bzero( recieved, MAX_RES_LEN);
      bzero( response, MAX_RES_LEN);
      puts("INFO: Reading client server.\n");
      read(comm_fd,recieved,100);
      printf("INFO: Recieved header: %s\n", recieved);

      char * method = strtok(recieved, " ");

      char * file_name = strtok(NULL, " "); 

      char * version = strtok(NULL, "\r\n");

      if (strcmp(version, "HTTP/1.0") != 0) {
        printf("ERROR: %s not supported. Exiting request.\n", version);
        clean();
        close(comm_fd);
        int ret = 1;
        pthread_exit(&ret);
      }
      printf("INFO: Detected version: %s\n", version);

      if (strcmp("GET", method) == 0) {
        puts("INFO: Recieved GET method.\n");

        printf("INFO: Looking for file %s.\n", file_name);
        while ((in_file = readdir(directory)) != NULL) {
          if (strcmp(in_file->d_name, file_name) == 0) { 
            puts("Found file in directory.\n");
            status = ok_response;
            file_found = true;
            file = fopen(in_file->d_name, "r");
            break;
          }
        }
        if (status == NULL || file == NULL) {
          puts("INFO: Cound not find file.\n");
          status = not_found;
        }
      } else {
        status = not_implemented;
        printf("INFO: Cound not understand method %s.\n", method);
      }

      build_header(response, status);
      printf("INFO: Built response with header {\n%s\n}\n", response);
      if (file_found) {
        append_file(response, file);
      }

      puts("INFO: Writing response to socket.\n");
      write(comm_fd, response, strlen(response)+1);
      puts("INFO: Done writing response to socket.\n");

      puts("INFO: Cleaning up...\n");
      puts("INFO: Closing file...\n");
      if (file_found) fclose(file);
      file_found = false;
      puts("INFO: closing directory...\n");
      if (directory != NULL) closedir(directory);
      puts("INFO: Done Cleaning up.\n");
   
   puts("INFO: Closing socket file descriptors.\n");
   close(comm_fd);
   int ret = 42; // void * return is required by pthread
   pthread_exit(&ret);
}

void append_file(char * response, FILE * file) {
  puts("INFO: Reading file.\n");
  char line[255];
  while((fgets(line, sizeof(line), file) != NULL) && strlen(response) < MAX_RES_LEN) {
    strcat(response, line);
  }
  puts("INFO: Done reading file.\n");
}

void m_free(void * ptr) {
  if (ptr != NULL) {
    free(ptr);
  }
}

void clean() {
  if (listen_fd > 0) close(listen_fd);
}

void clean_exit() {
  clean();
  exit(1);
}

void interupt_handler(int param) {
  clean();
  clean_exit();
}
