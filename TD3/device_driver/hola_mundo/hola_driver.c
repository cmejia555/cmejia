/**
 * @file hola_driver.c
 * author Cesar Mejia
 * version 1.0
 * date 11/09/2018
 * brief Primer driver desarrollado
 */

#include "hola_driver.h"

MODULE_LICENSE("Dual BSD/GPL");

static dev_t mi_dispo;
static struct cdev *pCdev;
static struct file_operations toUpper_ops = {
	.owner = THIS_MODULE,
	.open = mi_open,
	.read = mi_read,
	.write = mi_write,
	//.realese = mi_close,
};

static struct class *pClase;
static struct device *pDeviceSys;

static char *string;
static unsigned long sizeString = 0;

module_init(hello_init);
module_exit(hello_exit);


static int hello_init(void) {
	int error;
	int i;

	printk(KERN_ALERT "Configurando el driver\n");
	
	error = alloc_chrdev_region(&mi_dispo, FIRST_MINOR, NUMBER_DEV, "first_driver_TD3"); // registra una serie de char device
	if(error < 0) {
		printk(KERN_ALERT "ERROR function alloc_chrdev_region()\n");
		return -1;
	}
	printk(KERN_ALERT "Numero mayor asignado = %d\n", MAJOR(mi_dispo));

	pCdev = cdev_alloc(); // asigna una estructura cdev
	if(pCdev == NULL) {
		printk(KERN_ALERT "ERROR function cdev_alloc()\n");
		//unregister_chrdev_region(mi_dispo, NUMBER_DEV);
		return -1;
	}

	pCdev->ops = &toUpper_ops;
	pCdev->owner = THIS_MODULE;
	pCdev->dev = mi_dispo;

	error = cdev_add(pCdev, mi_dispo, NUMBER_DEV); // agrega un char device al sistema
	if( error < 0) {
		printk(KERN_ALERT "ERROR function cdev_add\n");
		return -1;
	}

	pClase = class_create(THIS_MODULE, "HOLA_TD3"); // crea una clase para ser usado en device_create
	pDeviceSys = device_create(pClase, NULL, mi_dispo, NULL, "to_upper"); // crea un device y lo registra en el system file

	string = kmalloc(MAX_SIZE_STRING, GFP_KERNEL);
	if (string == NULL) {
		printk(KERN_ALERT "ERROR en kmalloc()\n");
		return -1;
	}

	for(i = 0; i < MAX_SIZE_STRING; i++)
		*(string + i) = 0; 

	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Liberando recursos\n");

	kfree(string);
	device_destroy(pClase, mi_dispo);
	class_destroy(pClase);
	cdev_del(pCdev);
	unregister_chrdev_region(mi_dispo, NUMBER_DEV); // libera los numeros mayor y menor

}


int mi_open(struct inode * node, struct file *fd) {
	printk(KERN_ALERT "OPEN success\n");
	return 0;
}

ssize_t mi_write(struct file *fd, const char __user *userBuff, size_t len, loff_t *offset) {
	int i;
	unsigned long bytesNotCopy = 0;

	if(len >= MAX_SIZE_STRING) { // se trunca la memoria (-1 por el \0 final)
		len = MAX_SIZE_STRING - 1;
	}
	bytesNotCopy = copy_from_user(string, userBuff, len);
	if(bytesNotCopy) { // Si es != 0 hubo error en la copia
		printk(KERN_ALERT "Bytes sin copiar a buffer de kernerl = %ld\n", bytesNotCopy);
		len -= bytesNotCopy; // resto a len los bytes que no se copiaron
	}
	sizeString = len; // Guarda el tamano copiado

	/* Se convierte el string a mayusculas: toUpper */
	for(i = 0; i < len; i++) {
		if( *(string + i) >= 'a' && *(string + i) <= 'z') {
			*(string + i) -= 'a' - 'A';
		}
	}
	*(string + i) = 0; // Agrega fin de cadena

	return len;
}

ssize_t mi_read(struct file *fd, char __user *userBuff, size_t len, loff_t *offset) {
	unsigned long bytesNotCopy;

	if(len < sizeString)
		return ENOMEM;

	bytesNotCopy = copy_to_user(userBuff, string, sizeString + 1); // +1 por el fin de cadena
	if(bytesNotCopy) { // Si es != 0 hubo error en la copia
		printk(KERN_ALERT "Bytes sin copiar a buffer de usuario = %ld\n", bytesNotCopy);
	}

	return (sizeString - bytesNotCopy);
}


