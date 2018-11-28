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

static void properly_shutdown(const char *msg, int status) {
  printf("Error: %s\n", msg);
  close(sock);
  exit(status);
}

void signal_handler(int signal_num) {
  if( signal_num == SIGINT ) {
    properly_shutdown("Signal SIGINT", EXIT_SUCCESS);
  }
  if( signal_num == SIGPIPE ) {
    properly_shutdown("Signal SIGPIPE", EXIT_FAILURE);
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
  signal(SIGPIPE, signal_handler);

  sscanf(argv[3], "%d", &request_time);

  server.sin_family= PF_INET;
  ip = gethostbyname(argv[1]);
  memcpy(&(server.sin_addr), ip->h_addr_list[0], ip->h_length);
  server.sin_port = htons(atoi(argv[2]));
  bzero(&(server.sin_zero),8);

  if( (sock = socket(PF_INET, SOCK_STREAM, 0)) == -1 ) {
    properly_shutdown("socket()", EXIT_FAILURE);
  }

  if( connect(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_in)) == -1 ) {
    properly_shutdown("NO SE PUDO ESTABLECER CONEXION CON EL SERVER", EXIT_FAILURE);
  }

  send(sock, &request_time, sizeof(int), 0); // envio el tiempo de peticion
  while (1) {
    memset(data, 0, BUFLEN);
    msgLength = recv(sock, data, BUFLEN, 0);
    if( msgLength == -1 ) { // -1: error en recv
      properly_shutdown("recv()", EXIT_FAILURE);
    }
    else if(msgLength == 0) { // 0: server cerro la conexion del socket
      properly_shutdown("CONEXION INTERRUMPIDA. SERVER OFFLINE", EXIT_FAILURE);
    }
    printf("SERVIDOR: %s\n", data);
    if( strcmp(data, END_TRANSMITION) == 0 ) {
      properly_shutdown("SERVER termino transmision", EXIT_SUCCESS);
    }
  }

  properly_shutdown("", EXIT_SUCCESS);
}
