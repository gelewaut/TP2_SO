#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <stdint.h>
#define BUFFER_SIZE 57

uint64_t getKey();
uint64_t getRTC(uint64_t code);
void keyboard_handler();
uint64_t dump_buffer(char * dest, uint64_t size);

void fill_buffer ();
int dumpBuffer(char* destination, int size);

char * get_buffer ();
int buffer_count ();
void clear_buffer ();

#endif
