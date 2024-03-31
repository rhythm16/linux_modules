#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>

MODULE_AUTHOR("rhythm");
MODULE_DESCRIPTION("hello world module");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int __init helloworld_init(void)
{
    printk(KERN_INFO "Hello, world!\n");
    return 0;
}

static void __exit helloworld_exit(void)
{
    printk(KERN_INFO "Goodbye, world!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

