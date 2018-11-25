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
#include <fcntl.h>
#include <stdbool.h>

#define MAX_PENDING_CONNECTIONS   2

int driver;
bool LM75_init(int *fd);
void LM75_finish(int fd);
int LM75_read_temp(int fd, char buf[], int len);

int child_process(int fd, struct sockaddr_in client);

static int child_cont = 0;

void signal_handler(int signal_num) {
  if( signal_num == SIGPIPE ) {
    printf("CLIENTE interrumpio conexion\n");
  }
  
  if( signal_num == SIGCHLD ) {
    //printf("Soy el HANDLER: %d\n", getpid());
    while( (waitpid(-1, NULL, WNOHANG)) > 0 ) {
      child_cont--;
    }
  }
}

int main(int argc, char *argv[])
{
  pid_t pid;
  int sock_fd, sock_client;
  struct sockaddr_in server, client;
  socklen_t client_len;

  if(argc < 2) {
    printf("Enter: ./server PORT\n");
    exit(EXIT_FAILURE);
  }

  signal(SIGPIPE, signal_handler);
  signal(SIGCHLD, signal_handler);

  sock_fd = socket(PF_INET, SOCK_STREAM, 0);
  if ( sock_fd == -1 ) {
    perror("Error en apertura de socket\n");
    exit(EXIT_FAILURE);
  }

  server.sin_family= PF_INET;
  server.sin_port = htons(atoi(argv[1]));
  server.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server.sin_zero),8);

  if( bind(sock_fd, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1 ) {
  perror("Error en bind()\n");
  exit(EXIT_FAILURE);
  }

  if( listen(sock_fd, MAX_PENDING_CONNECTIONS) == -1 ) {
  perror("Error en listen()\n");
  exit(EXIT_FAILURE);
  }

  LM75_init(&driver);

  client_len = sizeof(struct sockaddr_in);
  while(1) {
    printf("SERVIDOR EN ESPERA...\n");
    if( (sock_client = accept(sock_fd, (struct sockaddr *)&client, &client_len)) == -1 ) {
      perror("Error en accept()");
      close(sock_fd);
      exit(EXIT_FAILURE);
    }

    child_cont++; // incrementa la cantidad de hijos
    pid = fork();
    switch (pid) {
      case -1: // error en fork()
        perror("Error en fork()");
        close(sock_client);
        close(sock_fd);
        exit(EXIT_FAILURE);
        break;
      case 0: // Proceso hijo
        close(sock_fd);
        printf("HIJO PID: %d\n", getpid());
        child_process(sock_client, client);
        break;
      default: // Proceso padre
        close(sock_client);
        break;
    }
    printf("Hijos activos: %d\n", child_cont);
  }

  exit(EXIT_FAILURE);
}

#define MAX_STRING    20
#define TIME_OUT      10

int child_process(int fd, struct sockaddr_in client)
{
  char buffer[] = "FIN";
  char data[30];
  int request_time = 0;
  ssize_t msgLen;
  int flag;
  int count = 0;

  printf("Conexion establecida con cliente IP %s y PORT %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));

  /*flag = fcntl(fd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(fd, F_SETFL, flag);*/
  msgLen = recv(fd, &request_time, sizeof(int), 0);
  if(msgLen == -1) {
    perror("Error recv()");
    exit(EXIT_FAILURE);
  }
  printf("Recibido del hijo: time = %d  size = %d\n", request_time, msgLen);

  while(1) {
    sleep(1);
    count++;
    if( count == TIME_OUT ) {
      if( send(fd, buffer, strlen(buffer), 0) > 0 ) {
        printf("Se alcanzo el TIME_OUT\n");
      }
      break;
    }
    // usar flag MSG_NOSIGNAL, en send(), si no se captura SIGPIPE
    if( !(count % request_time) ) { // envio datos del sensor al cliente cada request_time
      LM75_read_temp(driver, data, 30);
      if( send(fd, data, strlen(data), 0) == -1 ) { // error al enviar datos
        break;
      }
    }
  }
  close(fd);
  LM75_finish(driver);
  exit(EXIT_SUCCESS);
}



//receive_from_client()
//send_to_client()
bool LM75_init(int *fd) {
  *fd = open("/dev/to_upper", O_RDWR);
  return (*fd != -1);
}
void LM75_finish(int fd) {
  close(fd);
}

int LM75_read_temp(int fd, char buf[], int len) {
  int size;
  char string_env[] = "HolA mundo";

  size = write(fd, string_env, strlen(string_env));
  printf("str_env = %s, size = %d\n", string_env, size);

  size = read(fd, buf, len);
  printf("str_rcv = %s, size = %d\n", buf, size);
  /*int _temp = 0;
  if( read(fd, &_temp, 1) == -1 )
    return -1;

  return _temp;*/
  return 0;
}
