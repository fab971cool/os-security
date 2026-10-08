obj-m += version.o

KDIR ?= /usr/src/linux-6.18.55
PWD := $(shell pwd)


default:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=x86_64 modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
