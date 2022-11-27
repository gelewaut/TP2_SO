#ifndef _USERLIBC_H
#define _USERLIBC_H

#include <stdint.h>

#define ISSPACE(c) c == ' '
#define ISDIGIT(c) (c >= '0' && c <= '9')
#define ISUPPER(c) (c >= 'A' && c <= 'Z')
#define ISALPHA(c) ((c >= 'a' && c <= 'z') || ISUPPER(c))
#define ISVOWEL(c) (c== 'a'|| c=='e' || c== 'i' || c=='o' || c=='u' || c== 'A'|| c=='E' || c== 'I' || c=='O' || c=='U')
#ifndef ULONG_MAX
#define ULONG_MAX ((unsigned long)(~0L)) /* 0xFFFFFFFF */
#endif
#ifndef LONG_MAX
#define LONG_MAX ((long)(ULONG_MAX >> 1)) /* 0x7FFFFFFF */
#endif
#ifndef LONG_MIN
#define LONG_MIN ((long)(~LONG_MAX)) /* 0x80000000 */
#endif


uint64_t numToStr(uint64_t value, char * buffer, uint32_t base);
uint32_t uintToStr(uint64_t value, char * buffer);
int strToNum(char *str);
int string_compare(const char *s1, const char *s2);
int printf(char *string, ...);
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
void sleep(unsigned int seconds);
int getSecondsElapsed();
char *strcpy(char *destination, const char *source);
int strlen(char * s);
long strtol(const char *nptr, char **endptr, register int base);
void printPID (int pid);

#endif