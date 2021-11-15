VPATH = ../../../include
all : straps.o entry.o head.o
straps.o : straps.c put.h
	$(GCC) -c straps.c $(CFLAG) -I ../../../include
entry.o : entry.S
	$(GCC) -c entry.S $(CFLAG)
head.o : head.S
	$(GCC) -c head.S $(CFLAG)
