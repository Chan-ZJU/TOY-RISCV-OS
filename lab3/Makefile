export
CROSS_= riscv64-unknown-elf-
AR=${CROSS_}ar
GCC=${CROSS_}gcc
LD=${CROSS_}ld
OBJCOPY=${CROSS_}objcopy

ISA ?= rv64imafd
ABI ?= lp64

INCLUDE = -I ../include -I ${PWD}/include
CF = -g -O3 -march=${ISA} -mabi=${ABI} -mcmodel=medany -ffunction-sections -fdata-sections -nostartfiles -nostdlib -nostdinc -static -lgcc -Wl,--nmagic -Wl,--gc-sections
MARCO = -DPRIORITY
CFLAG = ${CF} ${INCLUDE} ${MARCO}

run :
	-make clean
	make -C lib
	make -C init
	make -C arch/riscv
	@echo Mode is --- ${MARCO}
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux

debug :
	-make clean
	make -C lib
	make -C init
	make -C arch/riscv
	@echo Mode is --- ${MARCO}
	qemu-system-riscv64 -nographic -machine virt -kernel vmlinux -S -s

.PHONY : clean
clean :
	rm init/main.o init/test.o arch/riscv/kernel/head.o arch/riscv/kernel/straps.o arch/riscv/kernel/entry.o System.map vmlinux lib/put.o lib/rand.o arch/riscv/kernel/sched.o
