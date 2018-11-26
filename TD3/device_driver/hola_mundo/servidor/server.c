#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>


#define MAX_PENDING_CONNECTIONS   2


bool LM75_init(int *fd);
void LM75_finish(int fd);
int LM75_read_temp(int fd, char buf[], int len);
int semaphore_init(int *id);
int semaphore_destroy(int id);
void lock(int semid);
void unlock(int semid);
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
  perror("Error en bind()\n");
  exit(EXIT_FAILURE);
  }

  if( listen(sock_fd, MAX_PENDING_CONNECTIONS) == -1 ) {
  perror("Error en listen()\n");
  exit(EXIT_FAILURE);
  }

  if( semaphore_init(&semid) == -1 ) {
    perror("semaphore");
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

#define MAX_STRING    20
#define TIME_OUT      10

int child_process(int fd, int semid)
{
  char buffer[] = "FIN";
  char data[30];
  int request_time = 0;
  ssize_t msgLen;
  int count = 0;


  /*flag = fcntl(fd, F_GETFL);
  flag |= O_NONBLOCK;
  fcntl(fd, F_SETFL, flag);*/
  msgLen = recv(fd, &request_time, sizeof(int), 0);
  if(msgLen == -1) {
    perror("Error recv()");
    exit(EXIT_FAILURE);
  }
  printf("Recibido del hijo: time = %d  size = %d\n", request_time, (int)msgLen);

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
      lock(semid);
      LM75_read_temp(driver, data, 30);
      unlock(semid);
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

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux specific) */
};

int semaphore_init(int *id) {
  key_t key;
  union semun arg;

  key = ftok("./server", 'K');
  if(key == -1)
    return -1;

  *id = semget(key, 1, 0666 | IPC_CREAT);
  if( *id == -1 )
    return -1;

  arg.val = 1;
  if( semctl(*id, 0, SETVAL, arg) == -1 ) {
    return -1;
  }

  return 1;
}

int semaphore_destroy(int id) {
  union semun arg;
  return semctl(id, 0, IPC_RMID, arg);
}

void lock(int semid) {
  struct sembuf sem_operations[1];
  sem_operations[0].sem_num = 0;
  sem_operations[0].sem_op = -1;
  sem_operations[0].sem_flg = 0;

  semop(semid, sem_operations, 1);
}

void unlock(int semid) {
  struct sembuf sem_operations[1];
  sem_operations[0].sem_num = 0;
  sem_operations[0].sem_op = 1;
  sem_operations[0].sem_flg = 0;

  semop(semid, sem_operations, 1);
}
