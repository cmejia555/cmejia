#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#define MAX_PENDING_CONNECTIONS   10

int child_process(int fd, struct sockaddr_in client);

static int child_cont = 0;

void signal_handler(int value) {
  printf("Soy el handelr\n");

  child_cont--;
  while( waitpid(-1, NULL, WNOHANG) != 0 );
  printf("Termino el handler\n");
}

int main(int argc, char *argv[])
{
  signal(SIGCHLD, signal_handler);
  pid_t pid;
  int sock_fd, sock_client;
  struct sockaddr_in server, client;
  socklen_t client_len;

  if(argc < 2) {
    printf("Enter: ./server PORT\n");
    exit(EXIT_FAILURE);
  }
  server.sin_family= AF_INET;
  server.sin_port = htons(atoi(argv[1]));
  server.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server.sin_zero),8);

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if ( sock_fd == -1 ) {
    perror("Error en apertura de socket\n");
    exit(EXIT_FAILURE);
  }

  if( bind(sock_fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1 ) {
  perror("Error en bind()\n");
  exit(EXIT_FAILURE);
  }

  if( listen(sock_fd, MAX_PENDING_CONNECTIONS) == -1 ) {
  perror("Error en listen()\n");
  exit(EXIT_FAILURE);
  }

  client_len = sizeof(struct sockaddr_in);
  while(1) {
    printf("SERVIDOR EN ESPERA...\n");
    if( (sock_client = accept(sock_fd, (struct sockaddr *)&client, &client_len)) == -1 ) {
      perror("Error en accept()");
      exit(EXIT_FAILURE);
    }

    child_cont++; // incrementa la cantidad de hijos

    pid = fork();
    switch (pid) {
      case -1:
        perror("Error en fork()");
        exit(EXIT_FAILURE);
        break;
      case 0: // Proceso hijo
        //close(sock_fd);
        //pid_child = getpid();
        child_process(sock_client, client);
        close(sock_client);
        break;
      default: // Proceso padre
        close(sock_client);
        //wait(NULL);
        break;
    }
    printf("Hijos activos: %d\n", child_cont);
  }
  close(sock_fd);
}


#define MAX_STRING    20
int child_process(int fd, struct sockaddr_in client)
{
  char buffer[MAX_STRING];
  ssize_t msgLen;

  printf("Conexion establecida con cliente IP %s y PORT %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
  msgLen = recv(fd, buffer, MAX_STRING, 0);
  printf("Recibido del hijo %s\n", buffer);

  if( strcmp(buffer, "HOLA") == 0 ) {
    send(fd, "CHAU hijo", strlen("CHAU hijo"), 0);
  }
  else printf("FUERA DE SEND\n");

  exit(EXIT_SUCCESS);
}
