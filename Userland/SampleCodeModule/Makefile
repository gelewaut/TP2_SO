include ../Makefile.inc

MODULE=0000-sampleCodeModule.bin
SOURCES=$(wildcard [^_]*.c) $(wildcard ./shell/*.c) 
SOURCES_ASM=$(wildcard *.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
ALL_OBJECTS= $(OBJECTS_ASM) $(OBJECTS)

all: $(MODULE)

$(MODULE): $(SOURCES)  $(OBJECTS) $(OBJECTS_ASM) _loader.o
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld _loader.o -o ../$(MODULE) $(OBJECTS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T sampleCodeModule.ld _loader.o --oformat=elf64-x86-64 -o ../0000-sampleCodeModule.elf $(OBJECTS) $(OBJECTS_ASM)


_loader.o:_loader.c
	$(GCC) $(GCCFLAGS) -c _loader.c

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

clean:
	rm -rf *.o shell/*.o

.PHONY: all clean print
