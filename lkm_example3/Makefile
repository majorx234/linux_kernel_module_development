TARGET = lkm_example3
obj-m := $(TARGET).o
KERNEL_VERSION=$(shell uname -r)
KDIR = /lib/modules/$(KERNEL_VERSION)/build
PWD = $(shell pwd)

lkm_example3:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

all: lkm_example3
