obj-m := hola_driver.o
KERNEL_DIR = /home/cesar/Escritorio/RepoTD3/kernel_beagle/

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) modules
	arm-linux-gnueabihf-gcc test.c -o test_driver

copy:
	scp hola_driver.ko debian@192.168.7.2:/home/debian/Desktop/Drivers/
	scp test_driver debian@192.168.7.2:/home/debian/Desktop/Drivers/

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f test_driver


