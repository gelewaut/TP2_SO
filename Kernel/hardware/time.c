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

void halt10() {
	uint32_t seconds = 10;
	uint64_t sec = 0, ant = 0;
	unsigned long start = ticks;
	ncPrint("Restarting shell in:");
	while (ticks - start < seconds * 18){
		sec = (ticks - start)/18 % 60;
		if (sec > ant) {
			ncPrintDec(seconds - sec);
			ncPrintChar(' ');
			ant = sec;
		}
	}
}