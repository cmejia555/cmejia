#define FIRST_MINOR 0
#define NBR_DEV 1

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <asm/io.h>
#include <linux/ioport.h>
#include <linux/delay.h>
#include <linux/interrupt.h>
#include <linux/gpio.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_irq.h>
#include <linux/of_platform.h>
#include <linux/of_address.h>


int toupper_open (struct inode * inode, struct file * file);
ssize_t toupper_write (struct file * file, const char __user * userbuff,size_t tamano, loff_t* offset);
ssize_t toupper_read (struct file * file, char __user * userbuff, size_t tamano, loff_t * offset);

int toupper_open (struct inode * inode, struct file * file){
	printk(KERN_ALERT "Openn!!\n");
return 0;
}
ssize_t toupper_write (struct file * file, const char __user * userbuff,size_t tamano, loff_t* offset){
return 0;
}
ssize_t toupper_read (struct file * file, char __user * userbuff, size_t tamano, loff_t * offset){
return 0;
}


MODULE_LICENSE("Dual BSD/GPL");

static dev_t mi_dispo;
static struct cdev * p_cdev;
struct class * pclase;
struct device * pdevice_sys;


static struct file_operations toupper_ops = {
	.owner = THIS_MODULE,
	.open = toupper_open,
	.read = toupper_read,
	.write = toupper_write
};

static int hello_init(void) {


//TO DO = Error check
	alloc_chrdev_region(&mi_dispo, FIRST_MINOR, NBR_DEV,"toupper_td3");

	printk(KERN_ALERT "Numero mayor asignado = %d\n", MAJOR(mi_dispo));

//TO DO = Error check
	p_cdev = cdev_alloc();
//unregister_chrdev_region en caso de error

	p_cdev->ops = &toupper_ops;
	p_cdev->owner = THIS_MODULE;
	p_cdev->dev = mi_dispo;

	printk(KERN_ALERT "struct cdev allocada\n");


cdev_add (p_cdev, mi_dispo, NBR_DEV);

pclase = class_create (THIS_MODULE, "td3");

pdevice_sys=  device_create (pclase, NULL,mi_dispo , NULL,"toupper");

return 0;
}

static void hello_exit(void) {

unregister_chrdev_region(mi_dispo, NBR_DEV);

}


module_init(hello_init);
module_exit(hello_exit);
