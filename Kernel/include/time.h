#ifndef TIME_H_
#define TIME_H_

#include <stdint.h>
#include <idtLoader.h>

void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
void halt(uint32_t seconds);


#endif
