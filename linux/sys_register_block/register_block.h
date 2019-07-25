#include <linux/unistd.h>

_syscall2(long, "sys_register_block", int, pid, int, flags)