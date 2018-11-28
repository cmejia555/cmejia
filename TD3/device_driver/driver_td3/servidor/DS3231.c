#include "DS3231.h"

#define DAY(n)  (days[n])

static uint8_t bcd2bin (uint8_t val) { return val - 6 * (val >> 4); }
static uint8_t bin2bcd (uint8_t val) { return val + 6 * (val / 10); }

static int read_registers(int fd, const uint8_t reg, uint8_t *buf, uint32_t cant_reg) {
  int size;
  size = write(fd, &reg, 1);
  size = read(fd, buf, cant_reg);

  return size;
}

static void clear_OSF(int fd) {
  uint8_t status;
  uint8_t txBuff[2];
  uint8_t rxBuff;

  read_registers(fd, DS3231_STATUSREG, &rxBuff, 1);

  status = rxBuff & ~0x80;
  txBuff[0] = DS3231_STATUSREG;
  txBuff[1] = status;
  write(fd, txBuff, 2);
}

uint8_t *days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

int driver;
//receive_from_client()
//send_to_client()
bool DS3231_init(void) {
  driver = open("/dev/to_upper", O_RDWR);
  return (driver == -1);
}
void DS3231_finish(void) {
  close(driver);
}

TIME_T DS3231_time(void) {
  TIME_T time_now;
  uint8_t rxBuff[7];

  read_registers(driver, DS3231_INIT_REG, rxBuff, 7);
  //printf("FECHA %s %s\n", __DATE__, __TIME__);

  time_now.ss = bcd2bin(rxBuff[0]);
  time_now.mm = bcd2bin(rxBuff[1]);
  time_now.hh = bcd2bin(rxBuff[2]);
  time_now.day = DAY( bcd2bin(rxBuff[3]) );
  time_now.date = bcd2bin(rxBuff[4]);
  time_now.month = bcd2bin(rxBuff[5]);
  time_now.year = bcd2bin(rxBuff[6]) + 2000;

  return time_now;
}

int DS3231_adjust_time(const uint8_t *timer_str) {
  int size;
  time_t t;
  struct tm *tm;
  uint8_t txBuff[8];

  t = time(NULL);
  tm = localtime(&t);

  txBuff[0] = DS3231_INIT_REG; // direccion inicial del registro a escribir
  txBuff[1] = bin2bcd((uint8_t)tm->tm_sec);
  txBuff[2] = bin2bcd((uint8_t)tm->tm_min);
  txBuff[3] = bin2bcd((uint8_t)tm->tm_hour);
  txBuff[4] = bin2bcd((uint8_t)(tm->tm_wday));
  txBuff[5] = bin2bcd((uint8_t)tm->tm_mday);
  txBuff[6] = bin2bcd((uint8_t)(tm->tm_mon + 1));
  txBuff[7] = bin2bcd((uint8_t)(tm->tm_year - 100)); // -100 por como se ajusta el año en la struct tm

  size = write(driver, txBuff, 8);

  clear_OSF(driver);

  return (size > 0);
}

bool DS3231_lostPower(void) {
  uint8_t rxBuff;
  read_registers(driver, DS3231_STATUSREG, &rxBuff, 1);
  printf("OSF: %d\n", rxBuff);

  return (rxBuff >> 7);
}
