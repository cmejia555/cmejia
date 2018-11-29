#include "common.h"

#define END_TRANSMITION   "FIN"

void signal_handler(int signal_num);

static void setup_handler(sighandler_t handler) {
  signal(SIGPIPE, handler);
  signal(SIGINT, handler);
  signal(SIGTERM, handler);
}

peer_t *client;

static void properly_shutdown(const char *msg, int status) {
  printf("Error: %s\n", msg);
  close(client->sock_main);
  exit(status);
}

int main(int argc, char *argv[])
{
  struct hostent *ip;
  ssize_t msgLength;
  char data[MAX_STRING];
  int request_time;

  if (argc < 4) {
    printf("Enter ./client IP PORT TIME\n");
    exit(EXIT_FAILURE);
  }
  client = (peer_t *) malloc(sizeof(peer_t));

  setup_handler(signal_handler);

  sscanf(argv[3], "%d", &request_time);

  client->address.sin_family= PF_INET;
  ip = gethostbyname(argv[1]);
  memcpy(&(client->address.sin_addr), ip->h_addr_list[0], ip->h_length);
  client->address.sin_port = htons(atoi(argv[2]));
  bzero(&(client->address.sin_zero),8);

  if( (client->sock_main = socket(PF_INET, SOCK_STREAM, 0)) == -1 ) {
    properly_shutdown("socket()", EXIT_FAILURE);
  }

  if( connect(client->sock_main, (struct sockaddr *)&client->address, sizeof(struct sockaddr_in)) == -1 ) {
    properly_shutdown("NO SE PUDO ESTABLECER CONEXION CON EL SERVER", EXIT_FAILURE);
  }

  send(client->sock_main, &request_time, sizeof(int), 0); // envio el tiempo de peticion
  while (1) {
    memset(data, 0, MAX_STRING);
    msgLength = recv(client->sock_main, data, MAX_STRING, 0);
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

void signal_handler(int signal_num) {
  if( signal_num == SIGINT ) {
    properly_shutdown("Signal SIGINT", EXIT_SUCCESS);
  }
  if( signal_num == SIGPIPE ) {
    properly_shutdown("Signal SIGPIPE", EXIT_FAILURE);
  }
  if( signal_num == SIGTERM ) {
    properly_shutdown("Signal SIGTERM", EXIT_FAILURE);
  }
}
