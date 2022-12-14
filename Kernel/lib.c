#include <stdint.h>
#include <lib.h>

uint32_t my_strlen(const char * str) {
	uint32_t len = 0;
	while (*str != '\0') {
		len++;
		str++;
	}
	return len;
}

uint32_t my_strcmp(const char * a, const char * b) {
	while (*a) {
		if (*a != *b) {
			break;
		}
		a++; b++;
	}
	return (uint32_t) (*a - *b);
}

char * my_strcpy(char * dest, const char * source) {
	if (source == NULL) {
		return NULL;
	}
	
	char * ptr = dest;
	while (*source != '\0') {
		*ptr = *source;
		ptr++;
		source++;
	}

	*ptr = '\0';

	return dest;
}

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;
	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}
