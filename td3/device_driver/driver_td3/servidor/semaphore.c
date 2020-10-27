#include "semaphore.h"

union semun {
  int              val;    /* Value for SETVAL */
  struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
  unsigned short  *array;  /* Array for GETALL, SETALL */
  struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux specific) */
};

int semaphore_create(void) {
  key_t key;
  int semid;
  union semun arg;

  key = ftok(FILE_PATH, 'K');
  if(key == -1)
    return -1;

  semid = semget(key, 1, 0666 | IPC_CREAT);
  if( semid == -1 )
    return -1;

  arg.val = 1;
  if( semctl(semid, 0, SETVAL, arg) == -1 ) {
    return -1;
  }

  return semid;
}

int semaphore_destroy(int semid) {
  union semun arg;
  return semctl(semid, 0, IPC_RMID, arg);
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
