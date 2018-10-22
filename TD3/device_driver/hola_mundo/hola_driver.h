#ifndef _HOLA_DRIVER_H_
#define _HOLA_DRIVER_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/errno.h>
#include <linux/uaccess.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/delay.h>
#include <linux/interrupt.h>

#include "memory_map.h"

#define FIRST_MINOR	0
#define NUMBER_DEV	1
#define MAX_SIZE_STRING 200

static int hello_init(void);
static void hello_exit(void);

int mi_open(struct inode * node, struct file *fd);
int mi_close(struct inode * node, struct file *fd);
ssize_t mi_write(struct file *fd, const char __user *userBuff, size_t len, loff_t *offset); // offset sin usar (NULL)
ssize_t mi_read(struct file *fd, char __user *userBuff, size_t len, loff_t *offset);

static int mi_probe(struct platform_device *drv);
static int mi_remove(struct platform_device *drv);

//void i2c_clk_enable(unsigned int addr);
void chip_config_register(u32 addr, u32 value);
void i2c_init(void __iomem * addr);

irqreturn_t mi_handler(int irq, void *dev_id);

#endif
