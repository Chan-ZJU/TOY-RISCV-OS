all :
	make -C kernel
	$(LD) -o ../../vmlinux ../../init/main.o ../../init/test.o kernel/head.o -T kernel/vmlinux.lds
	nm ../../vmlinux > ../../System.map
	$(OBJCOPY) -O binary ../../vmlinux boot/Image --strip-all
