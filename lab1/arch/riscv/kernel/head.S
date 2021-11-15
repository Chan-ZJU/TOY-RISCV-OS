	.section .text
	.align 3
	.global _start

_start: 
# according to the lds file
# shut down M-mode interrupt
	csrrci t0, mstatus, 8 # set mie bit of mstatus to 0
# set M-mode exception address with mtvec register
# load t1 of the address of Mexcep
	la t1, Mexcep 
	csrrw t0, mtvec, t1
# change from M-mode to S-mode by mret instruction
# MPP has 2 bits, first clear the high bit, then set the low bit
	li t0, 0x1000
	csrrc t1, mstatus, t0
	li t0, 0x800 # used to set mstatus.MPP to 1 (S-mode)
	csrrs t1, mstatus, t0 # now mstatus.MPP is 1
	la t2, SMode
	csrrw t0, mepc, t2 # mepc points to label SMode
	mret # change to S-Mode 

# in S-Mode
SMode:
	la t3, Sexcep
	csrrw t0, stvec, t3
	la sp, stack_top # stack_top is in the lds file
	la t0, start_kernel
	jr t0

# two exception base address
Mexcep: # Machine mode exception
Sexcep: # Supervisor mode exception
