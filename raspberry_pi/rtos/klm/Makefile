obj-m += klm.o
MOD = /nobackup/upstream/linux_pi/
#/tmp/bin/lib/modules/6.6.69-v7+/build

all:
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- V=1 -C $(MOD) M=$(PWD) modules 

clean:
	make -C $(MOD) M=$(PWD) clean


