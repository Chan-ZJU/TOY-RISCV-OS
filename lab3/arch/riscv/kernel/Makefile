VPATH = ${PWD}/include ../include
all : sched.o straps.o entry.o head.o
sched.o: sched.c sched.h put.h rand.h
	$(GCC) -c sched.c $(CFLAG)
straps.o : straps.c put.h
	$(GCC) -c straps.c $(CFLAG)
entry.o : entry.S
	$(GCC) -c entry.S $(CFLAG)
head.o : head.S
	$(GCC) -c head.S $(CFLAG)
