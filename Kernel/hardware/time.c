#include <time.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void halt(uint32_t seconds) {
	unsigned long start = ticks;
	while (ticks - start < seconds * 18) ;
}