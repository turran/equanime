
# configuration
KERNEL_DIR := /home/jl/work/neuros/osd2/git/linux-davinci-2.6
CROSS_COMPILE := arm-linux-
GCC := $(CROSS_COMPILE)gcc
INCLUDE := -I.

obj-m += dm6446_media.o

ifeq ($(KERNEL_DIR),)
KERNEL_DIR := ""
endif

all:
	make -C $(KERNEL_DIR) M=$(PWD)

modules_install:
	make -C $(KERNEL_DIR) M=$(PWD) modules_install


clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
