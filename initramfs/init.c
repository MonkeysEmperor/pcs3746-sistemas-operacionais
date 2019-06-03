#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include "print_ppid.h"

#define len(_arr) ((int)((&_arr)[1] - _arr))

static const char * const programs[] = { "/father" };

void panic(const char *msg)
{
	fprintf(stderr, "%s: %s (errno = %d)\n", msg, strerror(errno), errno);
	exit(-1);
}

void mount_fs()
{
	printf("Mounting filesystems\n");
	// If /sys is not created, make it read-only (mode = 444)
	if (mkdir("/sys", 0x124) && errno != EEXIST)
		panic("mkdir");
	if (mount("none", "/sys", "sysfs", 0, ""))
		panic("mount");
}

int main()
{
	printf("Custom initramfs - Hello World syscall:\n");
	mount_fs();

	printf("Forking to run %d programs\n", len(programs));
	pid_t pid;
	for (int i = 0; i < len(programs); i++) {
		const char *path = programs[i];
		pid = fork();
		if (pid == -1) {
			panic("fork");
		} else if (pid) {
			printf("Starting %s (pid = %d)\n", path, pid);
		} else {
			execl(path, path, (char *)NULL);
			panic("execl");
		}
	}

	for (int i = 0; 1; i++) {
		print_ppid();
		if (i == 100) kill(pid, 9);
	}

	printf("init finished\n");
	for (;;)
		sleep(1000);
	return 0;
}
