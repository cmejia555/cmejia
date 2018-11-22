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

#define BUFLEN 81

int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in server;
  struct hostent *ip;
  int msgLength;

  char buf[BUFLEN];

  if (argc < 3) {
    printf("Enter ./client IP PORT\n");
    exit(EXIT_FAILURE);
  }

  server.sin_family= AF_INET;
  ip = gethostbyname(argv[1]);
  memcpy(&(server.sin_addr), ip->h_addr_list[0], ip->h_length);
  server.sin_port = htons(atoi(argv[2]));
  bzero(&(server.sin_zero),8);

  if( (sock = socket(PF_INET, SOCK_STREAM, 0)) == -1 ) {
    perror("Error en apertura de socket\n");
    exit(EXIT_FAILURE);
  }

  if( connect(sock, (struct sockaddr *)&server, sizeof(server)) == -1 )
  {
    perror("No se puede conectar.\n");
    exit(EXIT_FAILURE);
  }

  printf("CLIENTE\n");
  //fgets(buf, sizeof(buf)-1, stdin);
  //printf("Enviado %s %d\n", buf, strlen(buf));
  send(sock, "HOLA", strlen("HOLA")+1, 0);

  memset(buf, 0, BUFLEN);
  msgLength = recv(sock, buf, BUFLEN, 0);

  printf("SERVIDOR: %s\n", buf);

  close(sock);

  exit(EXIT_SUCCESS);
}
