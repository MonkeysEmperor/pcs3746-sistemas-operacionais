#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>

#define __NR_print_ppid 400

long print_ppid()
{
	return syscall(__NR_print_ppid);
}
