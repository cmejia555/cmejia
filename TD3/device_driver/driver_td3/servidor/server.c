#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>

#include "semaphore.h"
#include "DS3231.h"

#define MAX_PENDING_CONNECTIONS   2

typedef struct {
  int sock_main;
  int sock_client;
  struct sockaddr_in server_addr;
  struct sockaddr_in client_adrr;
  socklen_t sock_size;
} peer_t;

int child_process(int fd, int semid);

typedef enum {PARENT = 0, CHILD = 1} process_t;

int sock_main, sock_client;
pid_t ppid;
static int child_cont = 0;
int semid;


static void properly_shutdown(const char *msg, int status) { // Finaliza el proceso
  printf("Error: %s\n", msg);
  close(sock_main);
  close(sock_client);
  exit(status);
}

void signal_handler(int signal_num) {
  if( signal_num == SIGPIPE ) {
    properly_shutdown("CLIENTE interrumpio conexion", EXIT_FAILURE);
  }

  if( signal_num == SIGCHLD ) { // esta signal solo lo ejecuta el padre
    while( (waitpid(-1, NULL, WNOHANG)) > 0 ) {
      child_cont--;
    }
  }
  if( signal_num == SIGINT ) {
    if ( ppid == getpid() ) { // solo el el padre cierra esto
      DS3231_finish();
      semaphore_destroy(semid);
    }
    properly_shutdown("Signal SIGINT", EXIT_SUCCESS);
  }
}


int main(int argc, char *argv[])
{
  pid_t pid;
  struct sockaddr_in server, client;
  socklen_t client_len;

  if(argc < 2) {
    printf("Enter: ./server PORT\n");
    exit(EXIT_FAILURE);
  }

  if( DS3231_init() ) {
    properly_shutdown("Driver open()", EXIT_FAILURE);
  }
  if( DS3231_lostPower() ) {
    printf("Adjust DS3231\n");
    DS3231_adjust_time(NULL);
  }

  signal(SIGPIPE, signal_handler);
  signal(SIGCHLD, signal_handler);
  signal(SIGINT, signal_handler);

  sock_main = socket(PF_INET, SOCK_STREAM, 0);
  if ( sock_main == -1 ) {
    properly_shutdown("socket()", EXIT_FAILURE);
  }

  server.sin_family= PF_INET;
  server.sin_port = htons(atoi(argv[1]));
  server.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server.sin_zero),8);

  if( bind(sock_main, (struct sockaddr*)&server, sizeof(struct sockaddr_in)) == -1 ) {
    properly_shutdown("bind()", EXIT_FAILURE);
  }

  if( listen(sock_main, MAX_PENDING_CONNECTIONS) == -1 ) {
    properly_shutdown("listen()", EXIT_FAILURE);
  }

  semid = semaphore_create();
  if( semid == -1 ) {
    properly_shutdown("semaphore_create()", EXIT_FAILURE);
  }

  client_len = sizeof(struct sockaddr_in);
  while(1) {
    printf("SERVIDOR EN ESPERA...\n");
    if( (sock_client = accept(sock_main, (struct sockaddr *)&client, &client_len)) == -1 ) {
      properly_shutdown("accept()", EXIT_FAILURE);
    }

    pid = fork();
    switch (pid) {
      case -1: // error en fork()
        properly_shutdown("fork()", EXIT_FAILURE);
        break;
      case 0: // Proceso hijo
        //close(sock_main); // cierra la copia del socket principal
        printf("HIJO PID: %d\n", getpid());
        child_process(sock_client, semid);
        break;
      default: // Proceso padre
        child_cont++; // incrementa la cantidad de hijos
        printf("Conexion establecida con cliente IP %s y PORT %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        close(sock_client);
        ppid = getpid();
        break;
    }
    printf("Hijos activos: %d\n", child_cont);
  }

  properly_shutdown("", EXIT_FAILURE);
}

#define MAX_STRING    50
#define TIME_OUT      50

int child_process(int fd, int semid)
{
  int request_time = 0;
  ssize_t msgLen;
  int count_time = 0;
  char buf_end[] = "FIN";
  char data[MAX_STRING];
  TIME_T rtc;

  /*flag = fcntl(fd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(fd, F_SETFL, flag);*/
  msgLen = recv(fd, &request_time, sizeof(int), 0); // wait for request time set
  if(msgLen == -1) {
    properly_shutdown("recv()", EXIT_FAILURE);
  }
  printf("Recibido del hijo: time = %d\n", request_time);

  while(1) {
    sleep(1);
    count_time++;
    if( count_time == TIME_OUT ) {
      if( send(fd, buf_end, strlen(buf_end), 0) > 0 ) {
        properly_shutdown("Se alcanzo el TIME_OUT", EXIT_SUCCESS);
      }
    }
    // usar flag MSG_NOSIGNAL, en send(), si no se captura SIGPIPE
    if( !(count_time % request_time) ) { // envio datos del sensor al cliente cada request_time
      lock(semid);
      rtc = DS3231_time();
      unlock(semid);
      sprintf(data, "%s, %.2d-%.2d-%.4d, %.2d:%.2d:%.2d", rtc.day, rtc.date, rtc.month, rtc.year,
      rtc.hh, rtc.mm, rtc.ss);

      if( send(fd, data, strlen(data), 0) == -1 ) { // error al enviar datos
        properly_shutdown("send()", EXIT_FAILURE);
      }
    }
  }

  properly_shutdown("", EXIT_FAILURE);
}
