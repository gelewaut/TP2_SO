#ifndef _INFOREG_H_
#define _INFOREG_H_

#include <stdint.h>

void saveIp(uint64_t _rip);
void saveRsp(uint64_t _rsp);
void saveReg(int i, uint64_t reg);
void printRegs();
void saveRegs();
void nextReg();

#endif