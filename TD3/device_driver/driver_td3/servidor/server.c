#include "common.h"

int child_process(int fd, int semid);
void signal_handler(int signal_num);

int semid;
peer_t *server;

static void properly_shutdown(const char *msg, int status) { // Finaliza el proceso
  printf("Error: %s\n", msg);
  close(server->sock_main);
  close(server->sock_client);
  free(server);
  exit(status);
}

static void setup_handler(sighandler_t handler) {
  signal(SIGPIPE, handler);
  signal(SIGCHLD, handler);
  signal(SIGINT, handler);
  signal(SIGTERM, handler);
}

int main(int argc, char *argv[])
{
  pid_t pid;

  if(argc < 2) {
    printf("Enter: ./server PORT\n");
    exit(EXIT_FAILURE);
  }
  server = (peer_t *)malloc(sizeof(peer_t));

  if( DS3231_init() ) {
    properly_shutdown("Driver open()", EXIT_FAILURE);
  }
  if( DS3231_lostPower() ) {
    printf("Adjust DS3231\n");
    DS3231_adjust_time(NULL);
  }

  setup_handler(signal_handler);

  server->sock_main = socket(PF_INET, SOCK_STREAM, 0);
  if ( server->sock_main == -1 ) {
    properly_shutdown("socket()", EXIT_FAILURE);
  }

  server->address.sin_family = PF_INET;
  server->address.sin_port = htons(atoi(argv[1]));
  server->address.sin_addr.s_addr = INADDR_ANY;
  bzero(&(server->address.sin_zero), 8);

  if( bind(server->sock_main, (struct sockaddr*)&server->address, sizeof(struct sockaddr_in)) == -1 ) {
    properly_shutdown("bind()", EXIT_FAILURE);
  }

  if( listen(server->sock_main, MAX_PENDING_CONNECTIONS) == -1 ) {
    properly_shutdown("listen()", EXIT_FAILURE);
  }

  semid = semaphore_create();
  if( semid == -1 ) {
    properly_shutdown("semaphore_create()", EXIT_FAILURE);
  }

  server->sock_size = sizeof(struct sockaddr_in);
  while(1) {
    printf("SERVIDOR EN ESPERA...\n");
    if( (server->sock_client = accept(server->sock_main, (struct sockaddr *)&server->address, &server->sock_size)) == -1 ) {
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
        child_process(server->sock_client, semid);
        break;
      default: // Proceso padre
        server->process.pid[server->process.child_cont] = pid;
        server->process.child_cont++; // incrementa la cantidad de hijos
        printf("Conexion establecida con cliente IP %s y PORT %d\n", inet_ntoa(server->address.sin_addr), ntohs(server->address.sin_port));
        close(server->sock_client);
        server->process.ppid = getpid();
        break;
    }
    printf("Hijos activos: %d\n", server->process.child_cont);
  }

  properly_shutdown("", EXIT_FAILURE);
}

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
    printf("ERRNO %d\n", errno);
    perror("recv()");
    properly_shutdown("recv()", EXIT_FAILURE);
  }
  printf("Recibido del hijo: request time = %d\n", request_time);

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

void signal_handler(int signal_num) {
  int i;

  if( signal_num == SIGPIPE ) {
    properly_shutdown("CLIENTE interrumpio conexion", EXIT_FAILURE);
  }

  if( signal_num == SIGCHLD ) { // esta signal solo lo ejecuta el padre
    while( (waitpid(-1, NULL, WNOHANG)) > 0 ) {
      server->process.child_cont--;
    }
  }
  if( signal_num == SIGINT ) {
    if ( server->process.ppid == getpid() ) { // solo el el padre cierra esto
      DS3231_finish();
      semaphore_destroy(semid);
    }
    properly_shutdown("Signal SIGINT", EXIT_SUCCESS);
  }
  if( signal_num == SIGTERM ) {
    printf("ME MATARON!!!\n");
    if( server->process.ppid == getpid() ) {
      for(i = 0; i < server->process.child_cont; i++) {
        kill(server->process.pid[i], SIGKILL);
      }
    }
    properly_shutdown("Signal SIGTERM", EXIT_SUCCESS);
  }
}
