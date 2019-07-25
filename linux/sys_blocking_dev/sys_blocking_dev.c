
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/types.h>
#include <linux/linkage.h>
#include <linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/types.h>
#include <linux/stat.h>

#define MAX_PIDS_NUMBER 100

static DECLARE_WAIT_QUEUE_HEAD(wq);
static LIST_HEAD(data_queue);

int blocked_pids[MAX_PIDS_NUMBER];
int arrayAllocation =  0;

void unblock(void) {
	int i;
	arrayAllocation = 0;
	for(i = 0; i < 100; i++) {
		blocked_pids[i] = 0; 
	}
}

int alreadyThere (pid_t pid){
	int i;
	for(i = 0; i < 100; i++) {
		if(blocked_pids[i] == pid) {
			return 1;
		}
	}
	return 0;
}

void block(pid_t pid) {
	if(!alreadyThere(pid)) {
		blocked_pids[arrayAllocation++] = pid;
		pr_info("\nThe process id=%d is blocked!\n", pid);
	}

}

struct device_data {
	struct list_head head;
	char data;
};

static int get_data(char *c)
{
	struct device_data *entry;
	pid_t process_id = sys_getpid();

	if (list_empty(&data_queue)){
		block(process_id);
		return -1;
	}

	entry = list_first_entry(&data_queue, struct device_data, head);
	*c = entry->data;
	list_del(&entry->head);
	kfree(entry);
	return 0;
}

static long put_data(char c)
{
	struct device_data *entry = kmalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry) {
		pr_debug("Device queue data allocation failed\n");
		return -1;
	}
	entry->data = c;
	list_add_tail(&entry->head, &data_queue);
	unblock();
	return 0;
}

static ssize_t blocking_dev_read(struct file *filp, char __user *buffer,
	size_t length, loff_t *ppos)
{
	char c;
	if (!length) {
		return 0;
	}
	if (wait_event_interruptible(wq, !get_data(&c))) 
		return -ERESTARTSYS;
	pr_info("\nThe process id=%d is unblocked!\n", process_id);
	// simple_read_from_buffer will update the file offset and check whether
	// it fits the available data argument (1), use copy_to_user instead.
	return 1 - copy_to_user(buffer, &c, 1);
}

asmlinkage long sys_write_device(int data)
{
	int rc = put_data(data);
	if (rc)
		return rc;
	wake_up_interruptible(&wq);
	return 0;
}

// asmlinkage long sys_register_block()
// {
// 	//int fd = open("/sys/show_blocked", O_WRONLY|O_CREAT);
// 	//write(fd, &blocked_pids, 100 * sizeof(int));
//     return 0;
// }

static const struct file_operations blocking_dev_fops = {
	.owner = THIS_MODULE,
	.read  = blocking_dev_read
};

static struct miscdevice id_misc_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "blocking_dev",
	.fops = &blocking_dev_fops
};

static int __init blocking_dev_init(void)
{
	int retval;

	retval = misc_register(&id_misc_device);
	if (retval)
		pr_err("blocking_dev_dev: misc_register %d\n", retval);
	return retval;
}

static void __exit blocking_dev_exit(void)
{
	misc_deregister(&id_misc_device);
}

module_init(blocking_dev_init);
module_exit(blocking_dev_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Tiago Koji Castro Shibata <tishi@linux.com>");
MODULE_DESCRIPTION("Blocking device sample");