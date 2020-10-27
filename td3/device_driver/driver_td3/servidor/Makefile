all:
	#arm-linux-gnueabihf-gcc server.c -o server
	gcc server.c DS3231.c semaphore.c -o server
	gcc client.c -o client

clean:
	rm -f client server
