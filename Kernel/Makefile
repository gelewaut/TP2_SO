include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c) $(wildcard exeptions/*.c) $(wildcard hardware/*.c) $(wildcard IDT/*.c) $(wildcard memoryManager/*.c) $(wildcard processManager/*.c) $(wildcard semaphore/*.c)
SOURCES_ASM=$(wildcard asm/*.asm) $(wildcard hardware/*.asm) $(wildcard IDT/*.asm) $(wildcard exeptions/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

MMU= -D MMU
LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=


all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include $(MMU) -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf asm/*.o *.o *.bin exeptions/*.o hardware/*.o IDT/*.o memoryManager/*.o processManager/*.o semaphore/*.o *.elf

.PHONY: all clean
