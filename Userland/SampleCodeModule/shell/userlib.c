#include <userlibc.h>
#include <stdarg.h>
#include <stdint.h>
#include <syscalls.h>
#include <stdlib.h>

#define BUFFER_SIZE 32
#define STDIN 0
#define STDOUT 1
#define STDERR 2
#define CLOCK_HOURS 7
#define CLOCK_MINUTES 4
#define CLOCK_SECONDS 2
static char buffer[BUFFER_SIZE] = {0};


uint64_t numToStr(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

int strToNum(char *str)
 {

    int n = strlen(str);
    int mult = 1;
    int number = 0;
    n = (int) n < 0 ? -n : n;

    while(n--){
        if((str[n] < '0' || str[n] > '9') && str[n] != '-'){
            if(number)
                break;
            else
                continue;
        }

        if(str[n] == '-'){
            if(number){
                number = -number;
                break;
            }
        }
        else {
            number += (str[n] - '0') * mult;
            mult *= 10;
        }
    }

    return number;
}
int string_compare(const char *s1, const char *s2)
{
	int i;
	for (i = 0; s1[i] == s2[i]; i++)
	{
		if (s1[i] == '\0')
			return 0;
	}
	return s1[i] - s2[i];
}

long strtol(const char *nptr, char **endptr, register int base)
{
	register const char *s = nptr;
	register unsigned long acc;
	register int c;
	register unsigned long cutoff;
	register int neg = 0, any, cutlim;
	/*
         * Skip white space and pick up leading +/- sign if any.
         * If base is 0, allow 0x for hex and 0 for octal, else
         * assume decimal; if base is already 16, allow 0x.
         */
	do
	{
		c = *s++;
	} while (ISSPACE(c));
	if (c == '-')
	{
		neg = 1;
		c = *s++;
	}
	else if (c == '+')
		c = *s++;
	if ((base == 0 || base == 16) &&
		c == '0' && (*s == 'x' || *s == 'X'))
	{
		c = s[1];
		s += 2;
		base = 16;
	}
	if (base == 0)
		base = c == '0' ? 8 : 10;
	cutoff = neg ? -(unsigned long)LONG_MIN : LONG_MAX;
	cutlim = cutoff % (unsigned long)base;
	cutoff /= (unsigned long)base;
	for (acc = 0, any = 0;; c = *s++)
	{
		if (ISDIGIT(c))
			c -= '0';
		else if (ISALPHA(c))
			c -= ISUPPER(c) ? 'A' - 10 : 'a' - 10;
		else
			break;
		if (c >= base)
			break;
		if (any < 0 || acc > cutoff || (acc == cutoff && c > cutlim))
			any = -1;
		else
		{
			any = 1;
			acc *= base;
			acc += c;
		}
	}
	if (any < 0)
	{
		acc = neg ? LONG_MIN : LONG_MAX;
	}
	else if (neg)
		acc = -acc;
	if (endptr != 0)
		*endptr = (char *)(any ? s - 1 : nptr);
	return (acc);
}
int scanf(char *str, ...)
{
	va_list vl;
	int i = 0, j = 0, ret = 0;
	char buff[100] = {0}, c; //tmp[20] no se usa
	char *out_loc;
	while (c)
	{
		if (sys_read(STDIN, &c, 1))
		{
			buff[i] = c;
			i++;
		}
	}
	va_start(vl, str);
	i = 0;
	while (str && str[i])
	{
		if (str[i] == '%')
		{
			i++;
			switch (str[i])
			{
			case 'c':
			{
				*(char *)va_arg(vl, char *) = buff[j];
				j++;
				ret++;
				break;
			}
			case 'd':
			{
				*(int *)va_arg(vl, int *) = strtol(&buff[j], &out_loc, 10);
				j += out_loc - &buff[j];
				ret++;
				break;
			}
			case 'x':
			{
				*(int *)va_arg(vl, int *) = strtol(&buff[j], &out_loc, 16);
				j += out_loc - &buff[j];
				ret++;
				break;
			}
			}
		}
		else
		{
			buff[j] = str[i];
			j++;
		}
		i++;
	}
	va_end(vl);
	return ret;
}
int printf(char *str, ...)
{
    va_list args;
    int i = 0, j = 0;  // i lectura en str  - j pos en buffer
    char buff[100] = {0}, tmp[20];
    char *str_arg;
    va_start(args, str);
    while (str && str[i])
    {
        if (str[i] == '%')
        {
            i++;
            switch (str[i])
            {
                case 'c':
                {
                    buff[j] = (char)va_arg(args, int);
                    j++;
                    break;
                }
                case 'd':
                {
                    numToStr(va_arg(args, int),tmp,10);
                    strcpy(&buff[j], tmp);
                    j += strlen(tmp);
                    break;
                }
                case 's': //modificacion para aceptar strings
                {
                    str_arg = (char *)va_arg(args, char *);
                    strcpy(&buff[j], str_arg);
                    j += strlen(str_arg);
                    break;
                }
            }
        }
        else
        {
            buff[j] = str[i];
            j++;
        }
        i++;
    }
    sys_write(1,buff,j);
    va_end(args);
    return j;
}
char getChar()
{
	char c;
	while( sys_read(STDIN, &c, 1) < 1);
	return c;
}
void putChar(char c)
{
	sys_write(STDOUT, &c, 1);
}
char getCharContinues()
{
    char buff[2] = {0};
    int ret = sys_read(0, buff, 2);
    if (ret <= 0)
        return -1;
    return buff[0];
}

void cleanBuffer()
{
	for (int i = 0; i < BUFFER_SIZE; i++)
	{
		buffer[i] = 0;
	}
}

void clear()
{
    sys_clearScreen();
}
uint16_t string_lenght(const char *str)
{
	uint16_t i = 0;
	while (str[i])
		i++;
	return i;
}
void printDec(uint64_t value)
{
	printBase(value, 10);
}


void printHex(uint64_t value)
{
	printBase(value, 16);
}
void printBin(uint64_t value)
{
	printBase(value, 2);
}
void printBase(uint64_t value, uint32_t base)
{
	uintToBase(value, buffer, base);
	print(buffer);
}

int strlen(char * s)
{
    int i;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}

char *strcpy(char *destination, const char *source)
{

    char *ptr = destination;

    while (*source != '\0')
    {
        *destination = *source;
        destination++;
        source++;
    }

    *destination = '\0';
    return ptr;
 }

void print(const char *str)
{
	for (int i = 0; str[i]; i++)
	{
		printChar(str[i]);
	}
}
void printChar(char c)
{
	sys_write(STDIN, &c, 1);
}

uint32_t uintToBase(uint64_t value, char *buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;
	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	} while (value /= base);
	// Terminate string in buffer.
	*p = 0;
	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}
	return digits;
}


int getSecondsElapsed()
{
      int t = sys_getTicks();
      return t / 18;
}

void sleep(unsigned int seconds)
{
      unsigned int limitTime = seconds + getSecondsElapsed();
      while (getSecondsElapsed() < limitTime);
}