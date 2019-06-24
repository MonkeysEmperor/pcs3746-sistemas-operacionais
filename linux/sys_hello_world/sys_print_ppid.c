#include <linux/linkage.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/syscalls.h>


asmlinkage long sys_print_ppid(void)
{
	//getpid() - will return process id of calling function
	pid_t process_id = sys_getpid();
	//getppid() - will return process id of parent function
	pid_t p_process_id = sys_getppid();

	//printing the process ids
	pr_info("\nThe process id: %d\n",process_id);
	return pr_info("The process id of parent function: %d\n",p_process_id);
}
