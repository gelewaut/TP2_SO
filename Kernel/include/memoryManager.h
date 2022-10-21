#ifndef __MEMORY_MANAGER__
#define __MEMORY_MANAGER__

#include <sysCalls.h>
#include <lib.h>

void mem_init(char *mem_base, unsigned long mem_size);
void *my_malloc(unsigned long nbytes);
void my_free(void *ap);
void print_memSet();

#endif