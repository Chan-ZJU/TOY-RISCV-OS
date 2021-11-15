all :
	make -C kernel
	$(LD) -o ../../vmlinux ../../init/main.o ../../init/test.o ../../lib/put.o kernel/head.o kernel/straps.o kernel/entry.o -T kernel/vmlinux.lds
	nm ../../vmlinux > ../../System.map
