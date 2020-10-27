obj-m := toupper.o
KERNEL_DIR = /home/guido/Escritorio/kernel_beagle

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) modules
	arm-linux-gnueabihf-gcc test.c -o test_prg

copy:
	scp toupper.ko	debian@192.168.0.119:/home/debian/
	scp test_prg	debian@192.168.0.119:/home/debian/


clean:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f test_prg
