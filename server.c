/*------------------------------
* server.c
* Description: HTTP server program
* CSC 361
* Instructor: Kui Wu
-------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#define MAX_STR_LEN 120         /* maximum string length */
#define SERVER_PORT_ID 9898     /* server port number */
#define DEBUG 1
void cleanExit();

/*---------------------main() routine--------------------------*
 * tasks for main
 * generate socket and get socket id,
 * max number of connection is 3 (maximum length the queue of pending connections may grow to)
 * Accept request from client and generate new socket
 * Communicate with client and close new socket after done
 *---------------------------------------------------------------------------*/
void cleanExit();
void perform_http(int);

int main(int argc, char **argv)
{
    //int sockfd, newsockid, clilen, n;
    //struct sockadd_in, serv_addr, cli_addr;
    //int port = 80;
    //if (sockfd = socket(AF_INET, SOCK_STREAM, 0) < ) {
    //  printf("ERROR: Failed to get a socket.\n");
    //  exit(1);
    //}

    //memset((char *)serv_addr, 0, sizeof(serv_addr)); 

    //serv_addr.sin_family = AF_INET;
    //serv_addr.sin_addr.s_addr = INADDR_ANY;
    //serv_addr.sin_port = htons(port);
    //
    //if (bind(sockfd, (struct sock_addr *) &serv_addr, sizeof(serv_addr)) < 0) {
    //  printf("ERROR: Recieved error");
    //while (1)
    //{
    //  close(newsockid);
    //}
    //return 0;
}

/*---------------------------------------------------------------------------*
 *
 * cleans up opened sockets when killed by a signal.
 *
 *---------------------------------------------------------------------------*/

void cleanExit()
{
    exit(0);
}

/*---------------------------------------------------------------------------*
 *
 * Accepts a request from "sockid" and sends a response to "sockid".
 *
 *---------------------------------------------------------------------------*/

void perform_http(int sockid)
{

}
