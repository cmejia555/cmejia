#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

void main(void)
{
	int fd;
	int size;
	char string_env[] = "Hola mundo";
	char string_rcv[30];

	fd = open("/dev/to_upper", O_RDWR);

	write(fd, string_env, 10);
	size = read(fd, string_rcv, 30);

	printf("str_rcv = %s, size = %d\n", string_rcv, size);

}
