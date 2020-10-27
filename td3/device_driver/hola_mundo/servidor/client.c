#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h>
#include <errno.h>

#define BUFLEN 100
#define END_TRANSMITION   "FIN"

int sock;

void signal_handler(int signal_num) {
  if( signal_num == SIGINT ) {
    close(sock);
    exit(EXIT_SUCCESS);
  }
}

int main(int argc, char *argv[])
{
  struct sockaddr_in server;
  struct hostent *ip;
  ssize_t msgLength;
  char data[BUFLEN];
  int request_time;

  if (argc < 4) {
    printf("Enter ./client IP PORT TIME\n");
    exit(EXIT_FAILURE);
  }

  signal(SIGINT, signal_handler);

  sscanf(argv[3], "%d", &request_time);

  server.sin_family= PF_INET;
  ip = gethostbyname(argv[1]);
  memcpy(&(server.sin_addr), ip->h_addr_list[0], ip->h_length);
  server.sin_port = htons(atoi(argv[2]));
  bzero(&(server.sin_zero),8);

  if( (sock = socket(PF_INET, SOCK_STREAM, 0)) == -1 ) {
    perror("Error socket()");
    exit(EXIT_FAILURE);
  }

  if( connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1 ) {
    perror("NO SE PUDO ESTABLECER CONEXION CON EL SERVER");
    exit(EXIT_FAILURE);
  }

  send(sock, &request_time, sizeof(int), 0); // envio el tiempo de peticion
  while (1) {
    memset(data, 0, BUFLEN);
    msgLength = recv(sock, data, BUFLEN, 0);
    if( msgLength == -1 ) { // -1: error en recv
      perror("Error recv()");
      close(sock);
      exit(EXIT_FAILURE);
    }
    else if(msgLength == 0) { // 0: server cerro la conexion del socket
      printf("CONEXION INTERRUMPIDA. SERVER OFFLINE\n");
      close(sock);
      exit(EXIT_FAILURE);
    }
    printf("SERVIDOR: %s %d\n", data, msgLength);
    if( strcmp(data, END_TRANSMITION) == 0 ) {
      printf("SERVER termino transmision\n");
      break;
    }
  }

  close(sock);
  exit(EXIT_SUCCESS);
}
