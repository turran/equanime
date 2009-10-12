
# configuration
CFLAGS += -Wall
KERNEL_DIR := /home/jl/work/neuros/osd2/git/linux-davinci-2.6
IP := 192.168.173.5
USER := root
DIR := /tmp
CROSS_COMPILE := arm-linux-
GCC := $(CROSS_COMPILE)gcc
INCLUDE := -I.

obj-m += mp2530f_host.o

ifeq ($(KERNEL_DIR),)
KERNEL_DIR := "/home/jl/work/placas/netdcu8/src-1.2/linux26/linux26/"
endif

all:
	make -C $(KERNEL_DIR) M=$(PWD)
	#make -C $(KERNEL_DIR) M=$(PWD) ARCH=arm modules CROSS_COMPILE=$(CROSS_COMPILE)

install:
	make -C $(KERNEL_DIR) M=$(PWD) ARCH=arm modules_install CROSS_COMPILE=$(CROSS_COMPILE)
	#make -C $(KERNEL_DIR) M=$(PWD) ARCH=arm modules_install CROSS_COMPILE=$(CROSS_COMPILE)


clean:
	make -C $(KERNEL_DIR) M=$(PWD) clean
	#make -C $(KERNEL_DIR) M=$(PWD) ARCH=arm clean

deploy:
	scp dm644x_dma.ko dpmmod.ko testdsp testdpm $(USER)@$(IP):$(DIR)
