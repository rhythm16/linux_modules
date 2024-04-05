#define pr_fmt(fmt) "%s: %s(): " fmt, KBUILD_MODNAME, __func__

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_AUTHOR("rhythm");
MODULE_DESCRIPTION("hello world module");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

// module params can be accessed at /sys/module/modname/parameters/*
static int print_count = 1;
module_param(print_count, int, 0660);
MODULE_PARM_DESC(print_count, "How many times to print hello to kernel logs");

static int __init helloworld_init(void)
{
	for (int i = 0; i < print_count; i++) {
		printk(KERN_INFO "Hello, world!\n");
		pr_info("this is some info\n");
	}
	return 0;
}

static void __exit helloworld_exit(void)
{
	for (int i = 0; i < print_count; i++) {
		printk(KERN_INFO "Goodbye, world!\n");
	}
}

module_init(helloworld_init);
module_exit(helloworld_exit);

