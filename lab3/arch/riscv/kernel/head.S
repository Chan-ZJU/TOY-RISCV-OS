.section .text.init
	.align 3
	.global _start
	.extern start_kernel
	.extern stack_top
	.extern _strp
	.extern bss_start
	.extern bss_end
	.equ MTIME, 0x200bff8
	.equ MTIMECMP, 0x2004000

_start:
	csrci mstatus, 8 #shutdown machine-mode interrupt
	li t0, 0x1800
	csrc mstatus, t0 #clear MPP
	li t0, 0x800
	csrs mstatus, t0 #set MPP to 01, which is Supervisor-mode,
			 #prepare to mret to Supervisor-mode
	li t0, 0x888
	csrc mie, t0	 #clear mie
			 #Machine software,timer,external interrupt
	la t0, fail1
	csrw mtvec, t0	 #set machine mode trap vector
	la t0, super
	csrw mepc, t0	 #when mret, jump to mepc

initialize_bss:
	la t0, bss_start
	la t1, bss_end
loop_bss:
	sb zero, 0(t0)
	addi t0,t0,1
	ble t0,t1,loop_bss #if(t0<=t1) j loop_bss

initialize_mtimecmp:
	li t0, MTIME
	ld t1, 0(t0)	 #t1 = mtime
	li t2, 0xfff000
	add t1, t1, t2
	li t0, MTIMECMP
	sd t1, 0(t0)	 #mtimecmp = mtime + 0xfff000

time_interrupt_delegate:
	li t0, 0x20	 #mideleg[5] is STIP
	csrs mideleg, t0
open_interrupt:
	csrsi mstatus, 8 #open machine-mode interrupt, set mstatus[mie]
	csrsi mstatus, 2 #open supervisor-mode interrupt, set mstatus[sie]
	li t0, 0x80
	csrs mie, t0	 #set mie[MTIE]
	li t0, 0x20
	csrs mie, t0  #set mie[STIE]
	mret 		 #return to superviosr-mode and pc->super(label)
	
super:
	la t0, st_reg
	csrw stvec, t0
	la sp, stack_top
	call t0, start_kernel

fail1:
	csrr t0, mcause
	li t1, 0x8000000000000007
	beq t0, t1, MTI	 #the interrupt is machine timer interrupt

ecall_from_Super:
	li t0, MTIMECMP
	ld t1, 0(t0)
	li t2, 0xfff000
	add t1, t1, t2 #MTIMECMP += 0xfff000
	sd t1, 0(t0)
	li t0, 0x80
	csrs mie, t0	 #set mie[MTIE]
	li t0, 0x20
	csrc mip, t0	 #disable mip[STIP]
	csrr t0, mepc
	addi t0, t0, 4
	csrw mepc, t0
	mret
	
MTI:
	li t0, 0x80
	csrc mie, t0	 #clear mie[MTIE], avoid other timer interrupt
	li t0, 0x20
	csrs mip, t0	 #set mip[STIP], make Superviosr mode timer interrupt
	mret		 #change to Supervisor mode to handle timer interrupt
