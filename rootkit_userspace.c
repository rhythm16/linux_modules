#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "rootkit.h"

int main() {
	unsigned long user_val = 0;
	int rootkit_fd = open("/dev/rootkit", O_RDWR);
	ioctl(rootkit_fd, ROOTKIT_GET_VAL, &user_val);
	printf("GET_VAL, user_val is now: %lx\n", user_val);

	user_val = 0xBBBB;
	ioctl(rootkit_fd, ROOTKIT_SET_VAL, &user_val);
	printf("SET_VAL, user_val is now: %lx\n", user_val);
}
