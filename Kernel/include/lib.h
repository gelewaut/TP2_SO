#ifndef LIB_H_
#define LIB_H_

// #define NULL ((void *) 0)
#include <stddef.h>
#include <stdint.h>
#include <naiveConsole.h>

uint32_t my_strlen(const char * str);
uint32_t my_strcmp(const char * a, const char * b);
char * my_strcpy(char * dest, const char * source);
void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);

char *cpuVendor(char *result);
uint32_t sys_xchg(uint32_t *lock, uint32_t value);

#endif