
/* Registro clock del periferico */
#define CM_PER_BASE           0x44E00000
#define CM_PER_I2C_OFFSET     0x44
#define CM_PER_I2C2_CLKCTRL   CM_PER_BASE | CM_PER_I2C_OFFSET
#define I2C2_CLK_EN   (2 << 0)

/* Registro de control de pines */
#define CONTROL_MODULE_BASE   0x44E10000
#define CONF_I2C2_SDA_OFFSET  0x978 // uart1_ctsn
#define CONF_I2C2_SCL_OFFSET  0x97C // uart1_rtsn
#define CONTROL_MODULE_I2C2_SDA       CONTROL_MODULE_BASE | CONF_I2C2_SDA_OFFSET
#define CONTROL_MODULE_I2C2_SCL       CONTROL_MODULE_BASE | CONF_I2C2_SCL_OFFSET
#define I2C2_PINMUX_EN        0x33//(3<<0)
// abilitar pull up de ser necesario

#define I2C_REG_CON         0xA4
#define I2C_REG_PSC         0xB0
#define I2C_REG_SCLL        0xB4
#define I2C_REG_SCLH        0xB8
#define I2C_REG_OA          0xA8
#define I2C_REG_SYSC        0x10
#define I2C_REG_SA          0xAC

#define I2C_REG_IRQENABLE_SET          0x2C
#define I2C_REG_CNT         0x98
#define I2C_REG_DATA        0x9C
#define I2C_REG_IRQSTATUS_RAW 0x24
#define I2C_REG_IRQSTATUS 0x28

#define I2C_CLEAR_RRDY    (1 << 3)

#define I2C_DISABLE       0x00 // i2c moduele not enable
#define I2C_ENABLE        0x8000 // i2c module enable
#define I2C_PRESCALE      0x0B // divide by 4 B0
#define I2C_LOW_TIME      0x0D // low time = (value + 7) * ICLK B4
#define I2C_HIGH_TIME     0x0F // high time = (value + 5) * ICLK B8
#define I2C_OWN_ADDRESS   0x36
#define I2C_SLAVE_ADDRESS 0x49


#define I2C_CON_MST       (1<<10)
#define I2C_CON_TRX       (1<<9)
//#define I2C_MASTER_RECEIVER   0x8401
#define I2C_MASTER_RECEIVER   0x8403
#define I2C_MASTER_TRANSMITTER   0x8601
#define I2C_CON_STT       (1<<0) // bit start set

#define I2C_INIT_TX       (1<<0)
#define I2C_IRQ_SET     ((1 << 6) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0))

#define I2C_IRQ_RRDY    (1 << 3)
#define I2C_IRQ_ARDY    (1 << 2)

#define I2C_REG_BUFF 0x94
#define I2C_CLEAR_FIFO  (1 << 14)


/*typedef struct {
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
*/
