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


static struct of_device_id i2c_of_match[] = {
	{
		.compatible = "i2c_driver_td3,omap4-i2c",
	},
	{},
};

MODULE_DEVICE_TABLE(of, i2c_of_match);
static struct platform_driver platDrv = {
	.probe = mi_probe,
	.remove = mi_remove,
	.driver = {
		.name = "TD3_I2C",
		.of_match_table = of_match_ptr(i2c_of_match),
	}
};

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
	printk(KERN_ALERT "Numero mayor asignado = %d\n", MINOR(mi_dispo));

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

	platform_driver_register(&platDrv);

	return 0;
}

static void hello_exit(void) {
	printk(KERN_ALERT "Liberando recursos\n");

	platform_driver_unregister(&platDrv);
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


void __iomem *map;
static int mi_probe(struct platform_device *drv) {
	printk(KERN_ALERT "Dispositivo conectado\n");

	// Configuracion del clock del i2c
	chip_config_register(CM_PER_I2C2_CLKCTRL, I2C2_CLK_EN);
	// Configuracion del pinmux del i2c
	chip_config_register(CONTROL_MODULE_I2C2_SCL, I2C2_PINMUX_EN);
	chip_config_register(CONTROL_MODULE_I2C2_SDA, I2C2_PINMUX_EN);

	map = of_iomap((drv->dev).of_node, 0);
	printk(KERN_ALERT "Direccion I2C %p\n",(void *)map);
	return 0;
}

static int mi_remove(struct platform_device *drv) {
	printk(KERN_ALERT "Dispositivo desconectado\n");
	iounmap(map);
	return 0;
}


void chip_config_register(u32 addr, u32 value) {
	void * map = ioremap(addr, 1);
	printk(KERN_ALERT "Address config: = %p\n",map);

	iowrite32(value, map);

	iounmap(map);
	//value = ioread32(map);
	//printk(KERN_ALERT "Valor registro = %d", value);

	return;
}

// void i2c_clk_enable(unsigned int addr) {
// 	unsigned int value;
// 	void *cmper = ioremap(addr, 1); // obtengo la zona de memoria de addr
// 	printk(KERN_ALERT "Direccion CM_PER %p\n",cmper);
//
// 	iowrite32(I2C2_CLK_EN, cmper); // escribo el valor en el registro
//
// 	value = ioread32(cmper);
// 	printk(KERN_ALERT "Valor registro = %d", value);
//
//   return;
// }
