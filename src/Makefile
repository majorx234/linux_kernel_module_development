TARGET = lkm_example
obj-m := $(TARGET)1.o $(TARGET)2.o
KERNEL_VERSION=$(shell uname -r)
KDIR = /lib/modules/$(KERNEL_VERSION)/build
PWD = $(shell pwd)

lkm_example1:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

lkm_example2:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

all: lkm_example1, lkm_example2

test:
	sudo dmesg -C
	sudo insmod lkm_example1.ko
	sudo rmmod lkm_example1.ko
	sudo insmod lkm_example2.ko
	sudo rmmod lkm_example2.ko
	sudo dmesg
