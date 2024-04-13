#include <linux/ioctl.h>

#define ROOTKIT_ID	0x420
#define ROOTKIT_GET_VAL	_IOR(ROOTKIT_ID, 0, unsigned long *)
#define ROOTKIT_SET_VAL	_IOW(ROOTKIT_ID, 1, unsigned long *)
