#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>

#define __NR_register_block 404

long register_block()
{
	return syscall(__NR_register_block);
}