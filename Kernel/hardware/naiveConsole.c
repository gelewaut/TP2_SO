#include <naiveConsole.h>

#define WHITE_ON_BLACK 0x0F
// #define LIGHTGRAY 7
// #define BLACK 0

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;
// static uint8_t * videoEnd = (uint8_t *) (video + width * height * 2);

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintln(const char * string) {
	ncPrint(string);
	ncNewline();
}

void ncPrintChar(char character)
{
	ncPrintCharFormat(character, LIGHTGRAY, BLACK);
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
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

void ncPrintCharFormat(char character, char letterColor, char backroundColor)
{
	if (character == '\n'){ 
		ncNewline();
	} else if (character == '\t') {
		for (int i=0; i<4; i++) 
			ncPrintChar(' ');
	} else if (character == 127) {//127 ascii del delete
		if (currentVideo > video) {
			currentVideo-=2;
			*currentVideo = ' ';
		}
	} else {
    	*currentVideo = character;
    	currentVideo += 1;

    	*currentVideo = (16 * backroundColor) + letterColor;
    	currentVideo += 1;
		updateScreen();
	}
}

void ncPrintFormat(const char * string, char letterColor, char backroundColor)
{
    int i;

    for (i = 0; string[i] != 0; i++)
        ncPrintCharFormat(string[i], letterColor, backroundColor);
}

void updateScreen() {
	if(currentVideo >= (video + width * height * 2)) { //videoEnd
		scrollUp();
	}
}

void scrollUp() {
	int i = 0;
	while (i < 24 * 80 * 2) {
		video[i] = video[i+80 * 2];
		i++;
	}
	while (i < 25 * 80 * 2) {
		video[i] = ' ';
		video[i+1] = 0x0F;
		i += 2;
	}
	currentVideo = video + 24 * 80 * 2;
}

uint64_t PrintAt(char * character, uint64_t bytes, int x, int y) {
	int position;
	int i;

	if (x > width || y > height ||x <0 || y < 0)
	{
		// invalid_OPCODE();
		return -1;
	}
	

	for (i=0; i<bytes && y<=height; i++) {
		if (x>=width) {
			x=0;
			y++;
		}
		position = (x*2) + (y*width*2);
		video[position] = character[i];
		video[position+1] = 0x07;
		x++;
	}
	return i;
}