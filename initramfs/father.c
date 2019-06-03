#include "print_ppid.h"
#include <unistd.h>

#define len(_arr) ((int)((&_arr)[1] - _arr))

static const char * const programs[] = { "/son" };


int main()
{
	for (int i = 0; i < len(programs); i++) {
		const char *path = programs[i];
		pid_t pid = fork();
		if (pid == -1) {
		} else if (pid) {
			printf("Starting %s (pid = %d)\n", path, pid);
		} else {
			execl(path, path, (char *)NULL);
		}
	}
   while (1) {
      print_ppid();
   }
   return 0;
}