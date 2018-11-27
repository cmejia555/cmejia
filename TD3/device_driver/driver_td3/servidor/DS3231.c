#include "DS3231.h"

static uint8_t rawdata_to_time (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t time_to_rawdata (uint8_t val) { return val + 6 * (val / 10); }

//receive_from_client()
//send_to_client()
bool DS3231_init(int *fd) {
  *fd = open("/dev/to_upper", O_RDWR);
  return (*fd != -1);
}
void DS3231_finish(int fd) {
  close(fd);
}

TIME_T DS3231_time(int fd) {
  TIME_T time_now;
  int size;
  uint8_t txBuff[1] = {DS3231_INIT_REG};
  uint8_t rxBuff[7];

  size = write(fd, txBuff, 1);
  printf("Enviados size = %d\n", size);

  size = read(fd, rxBuff, 7);
  printf("Recibidos size = %d\n", size);

  time_now.ss = rawdata_to_time(rxBuff[0]);
  time_now.mm = rawdata_to_time(rxBuff[1]);
  time_now.hh = rawdata_to_time(rxBuff[2]);
  time_now.day = rawdata_to_time(rxBuff[3]);
  time_now.date = rawdata_to_time(rxBuff[4]);
  time_now.month = rawdata_to_time(rxBuff[5]);
  time_now.year = rawdata_to_time(rxBuff[6]) + 2000;

  return time_now;
}

int DS3231_adjust_time(int fd) {
  int size;
  time_t t;
  struct tm *tm;
  uint8_t txBuff[8];

  t = time(NULL);
  tm = localtime(&t);
  txBuff[0] = DS3231_INIT_REG; // direccion inicial del registro a escribir
  txBuff[1] = time_to_rawdata((uint8_t)tm->tm_sec);
  txBuff[2] = time_to_rawdata((uint8_t)tm->tm_min);
  txBuff[3] = time_to_rawdata((uint8_t)tm->tm_hour);
  txBuff[4] = time_to_rawdata((uint8_t)(tm->tm_wday + 1));
  txBuff[5] = time_to_rawdata((uint8_t)tm->tm_mday);
  txBuff[6] = time_to_rawdata((uint8_t)(tm->tm_mon + 1));
  txBuff[7] = time_to_rawdata((uint8_t)(tm->tm_year - 2000));

  size = write(fd, txBuff, 8);
  printf("Enviados size = %d\n", size);

  return (size > 0);
}

bool DS3231_isStopped(int fd) {
  int size;
  uint8_t txBuff[1] = {DS3231_STATUSREG};
  uint8_t rxBuff[1];

  size = write(fd, txBuff, 1);
  printf("Enviados size = %d\n", size);

  size = read(fd, rxBuff, 1);
  printf("Recibidos size = %d %d\n", size, rxBuff[0]);

  return (rxBuff[0] >> 7);
}
