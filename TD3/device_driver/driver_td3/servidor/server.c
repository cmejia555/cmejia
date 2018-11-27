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

int child_process(int fd, int semid);

int driver;

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

  int semid;

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
  perror("Error en bind()");
  exit(EXIT_FAILURE);
  }

  if( listen(sock_fd, MAX_PENDING_CONNECTIONS) == -1 ) {
  perror("Error en listen()");
  exit(EXIT_FAILURE);
  }

  if( semaphore_init(&semid) == -1 ) {
    perror("semaphore");
    exit(EXIT_FAILURE);
  }

  DS3231_init(&driver);
  DS3231_adjust_time(driver);
  /*if( DS3231_isStopped(driver) ) {
    printf("Adjust DS3231\n");
    //DS3231_adjust_time(driver);
  }
  else {
    printf("NOT\n");
  }*/

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
        child_process(sock_client, semid);
        break;
      default: // Proceso padre
        printf("Conexion establecida con cliente IP %s y PORT %d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        close(sock_client);
        break;
    }
    printf("Hijos activos: %d\n", child_cont);
  }

  exit(EXIT_FAILURE);
}

#define MAX_STRING    50
#define TIME_OUT      2

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
    perror("Error recv()");
    exit(EXIT_FAILURE);
  }
  printf("Recibido del hijo: time = %d  size = %d\n", request_time, (int)msgLen);

  while(1) {
    sleep(1);
    count_time++;
    if( count_time == TIME_OUT ) {
      if( send(fd, buf_end, strlen(buf_end), 0) > 0 ) {
        printf("Se alcanzo el TIME_OUT\n");
      }
      break;
    }
    // usar flag MSG_NOSIGNAL, en send(), si no se captura SIGPIPE
    if( !(count_time % request_time) ) { // envio datos del sensor al cliente cada request_time
      lock(semid);
      rtc = DS3231_time(driver);
      unlock(semid);
      sprintf(data, "Day %.2d, Date %.2d/%.2d/%.4d, Hour %.2d:%.2d:%.2d", rtc.day, rtc.date, rtc.month, rtc.year,
      rtc.hh, rtc.mm, rtc.ss);
      if( send(fd, data, strlen(data), 0) == -1 ) { // error al enviar datos
        break;
      }
    }
  }
  close(fd);
  //DS3231_finish(driver);
  exit(EXIT_SUCCESS);
}
