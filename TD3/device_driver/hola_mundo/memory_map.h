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



// typedef struct {
//   int reserverd[17];
//   int i2c_clkctrl;
// } CM_PER_T;
//
// #define CM_PER_CLK    (CM_PER_T *)CM_PER_BASE
//
// int i2c_enable_clk(CM_PER_CLK_T * cmper);
//
// int i2c_enable_clk(CM_PER_CLK_T * cmper) {
//   cmper->i2c_clkctrl = CM_PER_I2C_CLK_EN;
//
//   return 1;
// }
