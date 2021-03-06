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
	.release = mi_close,
	.read = mi_read,
	.write = mi_write,
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

static DECLARE_WAIT_QUEUE_HEAD(mi_queue);
struct I2C_INTERFACE_T i2c;// = {-1, NULL, NULL, NULL};

uint8_t buffer[2];

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
	//printk(KERN_ALERT "Direccion I2C %p\n",(void *)map);
	//*****************************************************
	i2c_init(i2c.mapAddr);
	//i2c_init(map);
	return 0;
}

int mi_close(struct inode * node, struct file *fd) {
	printk(KERN_ALERT "CLOSE success\n");
	return 0;
}

ssize_t mi_write(struct file *fd, const char __user *userBuff, size_t len, loff_t *offset) {
	int i;
	unsigned long bytesNotCopy = 0;

	uint8_t value = 0x00;
	i2c.txBuff[0] = value;

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


	/*bytesNotCopy = copy_from_user(i2c.txBuff, p, 1);
	if ( bytesNotCopy ) {
		printk(KERN_ALERT "ERROR copy: %ld\n", bytesNotCopy);
	}*/
	i2c_master_write(i2c.mapAddr, i2c.txBuff, 1);


	return len;
}

ssize_t mi_read(struct file *fd, char __user *userBuff, size_t len, loff_t *offset) {
	unsigned long bytesNotCopy;

	if(len < sizeString)
		return -ENOMEM;

	bytesNotCopy = copy_to_user(userBuff, string, sizeString + 1); // +1 por el fin de cadena
	if(bytesNotCopy) { // Si es != 0 hubo error en la copia
		printk(KERN_ALERT "Bytes sin copiar a buffer de usuario = %ld\n", bytesNotCopy);
	}
//*****************************************************
	i2c_master_read(i2c.mapAddr, i2c.rxBuff, 3);
	//i2c_master_read(map, i2c_buff, 2);
	return (sizeString - bytesNotCopy);
}

static int mi_probe(struct platform_device *drv) {
	printk(KERN_ALERT "Dispositivo conectado\n");

	// Configuracion del clock del i2c
	chip_config_register(CM_PER_I2C2_CLKCTRL, I2C2_CLK_EN);
	// Configuracion del pinmux del i2c
	chip_config_register(CONTROL_MODULE_I2C2_SCL, I2C2_PINMUX_EN);
	chip_config_register(CONTROL_MODULE_I2C2_SDA, I2C2_PINMUX_EN);

//**********************************************************
	i2c.mapAddr = of_iomap((drv->dev).of_node, 0);
	//map = of_iomap((drv->dev).of_node, 0);
	printk(KERN_ALERT "Direccion I2C %p\n",(void *)i2c.mapAddr);

	i2c.virq = platform_get_irq(drv, 0);
	//virq = platform_get_irq(drv, 0);


	if(i2c.virq < 0) {
		printk(KERN_ALERT "ERROR virq\n");
	 	return -1;
	 }
	 if( request_irq(i2c.virq, mi_handler, IRQF_TRIGGER_RISING, drv->name, NULL) < 0 ) {
	 	printk(KERN_ALERT "ERROR alloc mi_handler\n");
	 	return -1;
	 }
	 printk(KERN_ALERT "VIRQ = %d\n", i2c.virq);

	return 0;
}

static int mi_remove(struct platform_device *drv) {
	printk(KERN_ALERT "Dispositivo desconectado\n");
	free_irq(i2c.virq, NULL);
	iounmap(i2c.mapAddr);
	return 0;
}





void chip_config_register(u32 addr, u32 value) {
	void *mappping = ioremap(addr, 1);
	printk(KERN_ALERT "Address config: = %p\n",mappping);

	iowrite32(value, mappping);

	iounmap(mappping);
	//value = ioread32(map);
	//printk(KERN_ALERT "Valor registro = %d", value);

	return;
}

void i2c_init(void __iomem * addr) {
	//u32 value;
	printk(KERN_ALERT "Configuracion Init\n");
	iowrite32(I2C_DISABLE, addr + I2C_REG_CON);
	iowrite32(I2C_PRESCALE, addr + I2C_REG_PSC);
	iowrite32(I2C_LOW_TIME, addr + I2C_REG_SCLL);
	iowrite32(I2C_HIGH_TIME, addr + I2C_REG_SCLH);
	iowrite32(I2C_OWN_ADDRESS, addr + I2C_REG_OA);
	iowrite32((1<<3), addr + I2C_REG_SYSC); // 0x308
	//iowrite32(0x308, addr + I2C_REG_SYSC); // 0x308
	iowrite32(I2C_SLAVE_ADDRESS, addr + I2C_REG_SA);
	//iowrite32(I2C_ENABLE, addr + I2C_REG_CON);

	iowrite32(0x00, addr + I2C_REG_IRQENABLE_SET);
	iowrite32(I2C_CLEAR_FIFO, addr + I2C_REG_BUFF);

	iowrite32(I2C_ENABLE, addr + I2C_REG_CON);

	msleep(10);
	//value = ioread32(addr + I2C_REG_CON);
	//printk(KERN_ALERT "Valor registro = %d", value);
	return;
}

uint8_t i2c_master_write(void __iomem *addr, uint8_t *buff, uint8_t size)
{
	//int i = 0;
	int value;
	printk(KERN_ALERT "Estoy en master write: %d\n", buff[0]);
	i2c.flag_isr = 0;

	iowrite32(i2c.txBuff[0], addr + I2C_REG_DATA);
	iowrite32(size, addr + I2C_REG_CNT);
	i2c_enable_interrupt(addr, I2C_IRQ_XRDY);
	//i2c_set_data(addr, buff[i]);
		//i2c_set_data(addr, buff[i]);

	i2c_start_transfer(addr, I2C_MASTER_TRANSMITTER);
	//i2c_set_data(addr, buff[i]);
	wait_event_interruptible(mi_queue, i2c.flag_isr);

	printk(KERN_ALERT "Datos enviados\n");

	return 0;
}

uint8_t i2c_master_read(void __iomem *addr, uint8_t *buff, uint8_t size)
{
	//uint8_t value = 0x00;
	printk(KERN_ALERT "Estoy en master read\n");
	//i2c_master_write(addr, &value, 1);

	i2c.flag_isr = 0;

	iowrite32(size, addr + I2C_REG_CNT);

	i2c_enable_interrupt(addr, I2C_IRQ_RRDY);

	i2c_start_transfer(addr, I2C_MASTER_RECEIVER);

	wait_event_interruptible(mi_queue, i2c.flag_isr);


	printk(KERN_ALERT "IRQ 0 = %d\n", buff[0]);
	printk(KERN_ALERT "IRQ 1 =%d\n", buff[1]);
	printk(KERN_ALERT "IRQ 2 =%d\n", buff[2]);

	return 0;

}

void i2c_start_transfer(void __iomem *addr, u32 value) {
	iowrite32(value, addr + I2C_REG_CON);
}

void i2c_enable_interrupt(void __iomem * addr, uint32_t irq) {
	iowrite32(irq, addr + I2C_REG_IRQENABLE_SET);
}

void i2c_clear_irq(void __iomem * addr, u32 irq) {
	iowrite32(irq, addr + I2C_REG_IRQSTATUS);
}

uint8_t i2c_read_data(void __iomem * addr) {
	u32 value;
	value = ioread32(addr + I2C_REG_DATA);
	return (uint8_t)value;
}

irqreturn_t mi_handler(int irq, void *dev_id) {
	uint32_t irq_status = ioread32(i2c.mapAddr + I2C_REG_IRQSTATUS);
	printk(KERN_ALERT "Estoy en el Handler: %d\n", irq_status);
	i2c.flag_isr = 1;
	if(irq_status & I2C_IRQ_XRDY) {
		printk(KERN_ALERT "Estoy en el Handler 2\n");
		//iowrite32(i2c.txBuff[0], i2c.mapAddr + I2C_REG_DATA);
		i2c_clear_irq(i2c.mapAddr, I2C_IRQ_XRDY);
	}
	if( irq_status & I2C_IRQ_RRDY) {
	//i2c.flag_isr = 1;
	i2c.rxBuff[0] = i2c_read_data(i2c.mapAddr);
	i2c.rxBuff[1] = i2c_read_data(i2c.mapAddr);
	i2c.rxBuff[2] = i2c_read_data(i2c.mapAddr);

	i2c_clear_irq(i2c.mapAddr, I2C_CLEAR_RRDY); // los clear de los flag de IRQs deben ir al final
	}
	wake_up_interruptible(&mi_queue);

	return IRQ_HANDLED;
}
