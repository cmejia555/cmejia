#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>

#define DS3231_INIT_REG    0x00
#define DS3231_STATUSREG    0x0F

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

typedef struct _TIME {
  uint8_t ss;
  uint8_t mm;
  uint8_t hh;
  uint8_t day;
  uint8_t date;
  uint8_t month;
  uint16_t year;
} TIME_T;

bool DS3231_init(int *fd);
void DS3231_finish(int fd);
TIME_T DS3231_time(int fd);
bool DS3231_isStopped(int fd);
int DS3231_adjust_time(int fd);
