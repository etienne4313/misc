KDIR  := /nobackup/sim/initrd/lib/modules/5.4.142/build

obj-m += rtos_x86.o
test-y := rtos_x86.o 

default:
	$(MAKE) V=1 -C $(KDIR) M=$$PWD modules LOCALVERSION=""
	$(MAKE) V=1 -C $(KDIR) M=$$PWD modules_install INSTALL_MOD_PATH=/nobackup/sim/initrd/

clean:
	rm -rf *.ko *.o *.mod.c .*.cmd Module.symvers modules.order .tmp_versions 

