#ifndef LIB_H
#define LIB_H

#define NULL ((void *) 0)
#include <stdint.h>
#include <naiveConsole.h>

uint32_t strlen(const char * str);
uint32_t strcmp(const char * a, const char * b);
void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
uint32_t sys_xchg(uint32_t *lock, uint32_t value);

#endif