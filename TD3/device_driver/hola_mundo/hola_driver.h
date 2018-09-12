#ifndef _HOLA_DRIVER_H_
#define _HOLA_DRIVER_H_

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h> 
#include <linux/errno.h>
#include <asm/uaccess.h>

#define FIRST_MINOR	0
#define NUMBER_DEV	1

static int hello_init(void);
static void hello_exit(void);

int mi_open(struct inode * node, struct file *fd);
ssize_t mi_write(struct file *fd, const char __user *userBuff, size_t len, loff_t *offset); // offset sin usar (NULL)
ssize_t mi_read(struct file *fd, char __user *userBuff, size_t len, loff_t *offset);


#endif
