#ifndef _HOLA_DRIVER_H_
#define _HOLA_DRIVER_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "memory_map.h"

#define FIRST_MINOR							0
#define NUMBER_DEV							1
#define MAX_BUF_SIZE						12
#define MAX_REGISTERS_SIZE			MAX_BUF_SIZE

struct I2C_DATA_T{
	int virq;
  int flag_isr;
	void __iomem *mapAddr;
	uint8_t txBuff[MAX_BUF_SIZE];
	uint8_t txSize;
	uint8_t rxBuff[MAX_BUF_SIZE];
	uint8_t rxSize;
};

static int hello_init(void);
static void hello_exit(void);

int mi_open(struct inode * node, struct file *fd);
int mi_close(struct inode * node, struct file *fd);
ssize_t mi_write(struct file *fd, const char __user *userBuff, size_t len, loff_t *offset); // offset sin usar (NULL)
ssize_t mi_read(struct file *fd, char __user *userBuff, size_t len, loff_t *offset);

static int mi_probe(struct platform_device *drv);
static int mi_remove(struct platform_device *drv);


void chip_config_register(u32 addr, u32 value);
void i2c_init(void __iomem * addr);
uint8_t i2c_master_write(void __iomem *addr, uint8_t *buff, uint8_t size);
uint8_t i2c_master_read(void __iomem * addr, uint8_t *buff, uint8_t size);
void i2c_enable_interrupt(void __iomem * addr, uint32_t irq);
void i2c_clear_interrupt(void __iomem * addr, u32 irq);
void i2c_write_data(void __iomem *addr, uint32_t value);
uint8_t i2c_read_data(void __iomem * addr);
void i2c_start_transfer(void __iomem * addr, u32 value);
irqreturn_t mi_handler(int irq, void *dev_id);

#endif
