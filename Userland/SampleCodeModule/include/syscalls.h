#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

uint64_t sys_read(int fd, char * buf, uint64_t count);

uint64_t sys_write(int fd, const char * buf, uint64_t count);

uint64_t sys_writeAt(char * buf, uint64_t count, int x, int y);

uint64_t sys_readNoStop(uint64_t fd, uint64_t buffer, uint64_t length);
/*
    Reg#	  Description

    00        RTC seconds
    01        RTC seconds alarm
    02        RTC minutes
    03        RTC minutes alarm
    04        RTC hours
    05        RTC hours alarm
    06        RTC day of week
    07        RTC day of month
    08        RTC month
    09        RTC year
*/
uint64_t sys_clock(uint64_t code);

uint64_t sys_timerTick(void);
void sys_infoReg(void);
void sys_printMem(uint64_t * direction);
void sys_clearScreen(void);

#endif