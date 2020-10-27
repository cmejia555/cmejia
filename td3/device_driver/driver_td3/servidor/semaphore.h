#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define FILE_PATH     "./server"

int semaphore_create(void);
int semaphore_destroy(int semid);
void lock(int semid);
void unlock(int semid);
