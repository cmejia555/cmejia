#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void main(void)
{
	int fd;
	fd = open("/dev/to_upper", O_RDWR);
}
