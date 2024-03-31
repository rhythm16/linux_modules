# Linux Modules

## Prerequisites

* a compiled kernel source tree
* `make modules` on that kernel source

## Common module-related commands

```bash
cat /proc/modules
lsmod
modprobe
insmod
rmmod
```

## Kernel Printk

`cat /proc/sys/kernel/printk`

1. current console log level (smaller levels will apprear on console)
2. default level
3. minimum allowed log level
4. boot time default log level
