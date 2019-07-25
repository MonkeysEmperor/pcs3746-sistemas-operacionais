#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	int fd = open("/dev/blocking_dev", O_RDONLY);
	if (fd == -1) {
		perror("open");
		return -1;
	}

	char item;
	pid_t process_id = getpid();
	while(1) {
		printf("Process with pid=%d blocked!\n", process_id);
		int size = read(fd, &item, 1);
		printf("Process with pid=%d unblocked!\n", process_id);
		if (!size) {
			printf("EOF\n");
			return 0;
		} else if (size < 0) {
			perror("read");
			return -1;
		} else { 
			printf("Read item: %x\n", item);
		}
	}
	return 0;
}