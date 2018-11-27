#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define FILE_PATH     "./server"

int semaphore_init(int *id);
int semaphore_destroy(int id);
void lock(int semid);
void unlock(int semid);
