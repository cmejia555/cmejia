#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/fs.h>
#include <linux/cdev.h> 
#include <linux/errno.h>
#include <asm/uaccess.h>

MODULE_LICENSE("Dual BSD/GPL");

#define FIRST_MINOR	0
#define NUMBER_DEV	1

int mi_open(struct inode *, struct file *);
ssize_t mi_write (struct file *, const char __user *, size_t, loff_t *);
ssize_t mi_read (struct file *, char __user *, size_t, loff_t *); //loff sin usar

int mi_open(struct inode * node, struct file *fd) {
	printk(KERN_ALERT "OPEN success");
	return 0;
}

ssize_t mi_write(struct file *fd, const char __user *bufUser, size_t len, loff_t *offset){
	return 0;
}

ssize_t mi_read(struct file *fd, char __user *bufUser, size_t len, loff_t *offset){
	return 0;
}

static dev_t mi_dispo;
static struct cdev *pCdev;
static struct file_operations toUpper_ops = {
	.owner = THIS_MODULE,
	.open = mi_open,
	.read = mi_read,
	.write = mi_write,
};

static struct class *pClase;
static struct device *pDevice_sys;

static int hello_init(void) {
	int error;
	printk(KERN_ALERT "Primer driver");
	error = alloc_chrdev_region(&mi_dispo, FIRST_MINOR, NUMBER_DEV, "toUpper_TD3");	

	if(error < 0) {
		printk(KERN_ALERT "ERROR de alloc_chrdev_region");
		return -1;
	}

	pCdev = cdev_alloc();
	if(pCdev == NULL) {
		printk(KERN_ALERT "ERROR cdev_alloc");
		return -1;
	}

	pCdev->ops = &toUpper_ops;
	pCdev->owner = THIS_MODULE;
	pCdev->dev = mi_dispo;

	cdev_add(pCdev, FIRST_MINOR, NUMBER_DEV);

	pClase = class_create(THIS_MODULE, "TD3");
	pDevice_sys = device_create(pClase, NULL, mi_dispo, NULL, "toUpper");

	return 0;
}
static void hello_exit(void) {
	printk(KERN_ALERT "Goodbye, cruel world\n");
	unregister_chrdev_region(mi_dispo, NUMBER_DEV); // libera los numeros mayor y menor

}
module_init(hello_init);
module_exit(hello_exit);
