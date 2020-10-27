#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

#include <string.h>

void main(void)
{
	int fd;
	int size;
	char string_env[] = "HolA mundo";
	char string_rcv[30];

	fd = open("/dev/to_upper", O_RDWR);
	//sleep(2);

	size = write(fd, string_env, strlen(string_env));
	printf("str_env = %s, size = %d\n", string_env, size);

	size = read(fd, string_rcv, 30);
	printf("str_rcv = %s, size = %d\n", string_rcv, size);

	close(fd);

}
