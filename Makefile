obj-m += hello_world.o

PWD := $(CURDIR)

all:
	make -C $(KDIR) M=$(PWD) modules

help:
	@echo -e "Usage:"
	@echo -e "make KDIR=path/to/linux/source ARCH=cpu_arch\n"
	@echo -e "Add"
	@echo -e "CROSS_COMPILE=cross-compiler-prefix-"
	@echo -e "for gcc cross compiler\n"
	@echo -e "Or"
	@echo -e "LLVM=1 for clang cross compiling\n"

clean:
	make -C $(KDIR) M=$(PWD) clean
