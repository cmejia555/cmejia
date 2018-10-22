#include <linux/types.h>

#define CM_PER_BASE           0x44E00000
#define CM_PER_I2C_OFFSET     0x44
#define CM_PER_I2C2_CLKCTRL   CM_PER_BASE | CM_PER_I2C_OFFSET
#define I2C2_CLK_EN   (2 << 0)


#define CONTROL_MODULE_BASE   0x44E10000
#define CONF_I2C2_SDA_OFFSET  0x978 // uart1_ctsn
#define CONF_I2C2_SCL_OFFSET  0x97C // uart1_rtsn
#define CONTROL_MODULE_I2C2_SDA       CONTROL_MODULE_BASE | CONF_I2C2_SDA_OFFSET
#define CONTROL_MODULE_I2C2_SCL       CONTROL_MODULE_BASE | CONF_I2C2_SCL_OFFSET
#define I2C2_PINMUX_EN        (3<<0)
// abilitar pull up de ser necesario

#define I2C_REG_CON         0xA4
#define I2C_REG_PSC         0xB0
#define I2C_REG_SCLL        0xB4
#define I2C_REG_SCLH        0xB8
#define I2C_REG_OA          0xA8
#define I2C_REG_SYSC        0x10
#define I2C_REG_SA          0xAC
//#define I2C_CON

#define I2C_DISABLE       0x00 // i2c moduele not enable
#define I2C_ENABLE        0x8000 // i2c module enable
#define I2C_PRESCALE      0x03 // divide by 4
#define I2C_LOW_TIME      0x05 // low time = (value + 7) * ICLK
#define I2C_HIGH_TIME     0x07 // high time = (value + 5) * ICLK
#define I2C_OWN_ADDRESS   0x36
#define I2C_SLAVE_ADDRESS 0x40

typedef struct {
  //u32 RESERVED[4];
  u32 SYSC;
  u32 RESERVED_1[5];
  u32 IRQSTATUS_RAW;
  u32 IRQSTATUS;
  u32 IRQENABLE_SET;
  u32 IRQENABLE_CLR;
  u32 WE;
  u32 RESERVED_2[8];
  u32 CNT;
  u32 DATA;
  u32 RESERVED_3[2];
  u32 CON;
  u32 OA;
  u32 SA;
  u32 PSC;
  u32 SCLL;
  u32 SCLh;
  u32 RESERVED_4[7];
} I2C_REGISTERS_T;

#define i2c_registers      ((I2C_REGISTERS_T *) 0x10)
#define i2c                (i2c_registers[0])
// #define CM_PER_CLK    (CM_PER_T *)CM_PER_BASE
//
// int i2c_enable_clk(CM_PER_CLK_T * cmper);
//
// int i2c_enable_clk(CM_PER_CLK_T * cmper) {
//   cmper->i2c_clkctrl = CM_PER_I2C_CLK_EN;
//
//   return 1;
// }
