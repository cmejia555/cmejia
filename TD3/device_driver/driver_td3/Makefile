obj-m := hola_driver.o
KERNEL_DIR = /home/cesar/Escritorio/RepoTD3/kernel_beagle/

PATH_SERVER = ./servidor
FILES = server.c semaphore.c DS3231.c

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) modules
	arm-linux-gnueabihf-gcc test.c -o test_driver
	#arm-linux-gnueabihf-gcc $(PATH_SERVER)/server.c $(PATH_SERVER)/semaphore.c $(PATH_SERVER)/DS3231.c- o $(PATH_SERVER)/server
	arm-linux-gnueabihf-gcc $(addprefix $(PATH_SERVER)/, $(FILES)) -o $(PATH_SERVER)/server
	gcc ./servidor/client.c -o $(PATH_SERVER)/client

copy:
	sshpass -p temppwd scp hola_driver.ko debian@192.168.7.2:/home/debian/Desktop/Drivers/
	sshpass -p temppwd scp test_driver debian@192.168.7.2:/home/debian/Desktop/Drivers/
	sshpass -p temppwd scp ./servidor/server debian@192.168.7.2:/home/debian/Desktop/Drivers/

clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f test_driver $(PATH_SERVER)/server $(PATH_SERVER)/client
