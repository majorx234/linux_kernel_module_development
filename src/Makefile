TARGET = lkm_example1
obj-m := $(TARGET).o

KERNEL_VERSION=$(shell uname -r)
KDIR = /lib/modules/$(KERNEL_VERSION)/build
PWD = $(shell pwd)

all: printk

printk:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
