#ifndef NAIVE_CONSOLE_H
#define NAIVE_CONSOLE_H

#include <stdint.h>

void ncPrint(const char * string);
void ncPrintChar(char character);
void ncNewline();
void ncPrintDec(uint64_t value);
void ncPrintHex(uint64_t value);
void ncPrintBin(uint64_t value);
void ncPrintBase(uint64_t value, uint32_t base);
void ncClear();

void ncPrintln(const char * string);
void ncPrintCharFormat(char character, char letterColor, char backroundColor);
void ncPrintFormat(const char * string, char letterColor, char backroundColor);
void updateScreen();
void scrollUp();
uint64_t PrintAt(char * character, uint64_t bytes, int x, int y);

#endif