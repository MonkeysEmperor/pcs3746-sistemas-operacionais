#include <linux/linkage.h>
#include <linux/fcntl.h>
#include <linux/types.h>
#include <linux/stat.h>

asmlinkage long sys_register_block(int pid, int flags)
{
    //pid_t pid = getpid();
	//char* s_pid = kmalloc(16, GFP_KERNEL);
	//int fd = open("/sys/show_blocked", O_RDWR|O_CREAT|O_APPEND);
	//char* buff = kmalloc(1024 * sizeof(char), GFP_KERNEL);
	//read(fd, buff, 1024);
    return pid;
}