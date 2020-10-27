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
#include <unistd.h>
#include <netdb.h>

#include "semaphore.h"
#include "DS3231.h"

#define MAX_PENDING_CONNECTIONS   2
#define MAX_CONNECTIONS   10

#define MAX_STRING    50
#define TIME_OUT      1000

typedef struct {
  int child_cont;
  pid_t ppid;
  pid_t pid[MAX_CONNECTIONS];
} process_t;

typedef struct {
  int sock_main;
  int sock_client;
  struct sockaddr_in address;
  socklen_t sock_size;
  process_t process;
} peer_t;

typedef void (*sighandler_t)(int);
