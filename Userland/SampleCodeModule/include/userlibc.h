#ifndef _USERLIBC_H
#define _USERLIBC_H

#include <stdint.h>

uint64_t numToStr(char *buf, unsigned long int num, int base);
uint32_t uintToStr(uint64_t value, char * buffer);
int strToNum(char *str);
int string_compare(const char *s1, const char *s2);
void printf(char *string, ...);
int scanf(char *string, ...);
char getChar();
void putChar(char c);
char getCharContinues();
void printCharAt(char c, int x, int y);
void printDec(uint64_t num);
void printHex(uint64_t num);
void printBase(uint64_t value, uint32_t base);
void print(const char *str);
void printChar(char c);
uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base);
void cleanBuffer();
uint8_t getHours();
uint8_t getMinutes();
uint8_t getSeconds();
uint64_t getTicks();
void clear();
uint16_t string_length(const char *str);
void printDecAT(uint64_t value, int x, int y);
void printHexAT(uint64_t value, int x, int y);

#endif