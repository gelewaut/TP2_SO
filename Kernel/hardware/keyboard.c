#include <hardware.h>
#include <inforeg.h>

//capslock 58
//shift izq 42
//shift der 54
//ctrl 29
//alt 56
//lk 75
//rk 77
//upk 72
//dwnk 80

// #define BUFFER_SIZE 57
#define LSHIFT_PRESSED 0x2A
#define LSHIFT_RELEASED 0xAA
#define RSHIFT_PRESSED 0x36 
#define RSHIFT_RELEASED 0xB6

static uint8_t shift_enabled = 0;

static int rdIdx = 0; //Posicion de escritura
static int wrIdx = 0; //Posicion de lectura
static int activeSize = 0; //Elementos legibles en el buffer

static char buffer[BUFFER_SIZE] = {0};
int buffer_index = 0;

static const char keys[] = {
    0,'\e','1','2','3','4','5','6','7','8','9','0','-','=',127, //14
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',  //28
    0,'a','s','d','f','g','h','j','k','l',';','\'', //40
    '`',0,'\\','z','x','c','v','b','n','m',',','.','/', 0, //54
    0,0,' ',//57
};

int dumpChar();
void loadKey(char c);

void keyboard_handler() {
    fill_buffer();
}

void fill_buffer () {
    uint64_t key = getKey();
    if (key == 56) {
        saveRegs();
    }

    if (key == LSHIFT_PRESSED || key == RSHIFT_PRESSED) {
        shift_enabled = 1;
    }
    if (key == LSHIFT_RELEASED || key == RSHIFT_RELEASED) {
        shift_enabled = 0;
    }

    if (key >= 0 && key<= 57) {
        if(buffer_index >= BUFFER_SIZE) {
            buffer_index = 0;
        }
        int token;
        if((token = keys[key])) {
            if (shift_enabled == 1) {
                if (key == 8) {
                    token = '&'; 
                } else if (key == 43) {
                    token = '|';
                }
            }
            loadKey(token);
        }
    }
}

void loadKey(char c){
    if (c != 0)
    {
        buffer[wrIdx] = c;
        wrIdx = (wrIdx + 1) % BUFFER_SIZE; //Ciclo circularmente por el buffer

        // Si no llene el buffer, aumento su tamaño, si lo llene, agrego igual pero pierdo su ultimo valor "lectura artificial"
        if (activeSize < BUFFER_SIZE)
            activeSize++;
        else
            rdIdx = (rdIdx + 1) % BUFFER_SIZE;
    }
}

int dumpChar()
{
    if (activeSize <= 0) 
        return -1;

    int c = buffer[rdIdx];
    rdIdx = (rdIdx + 1) % BUFFER_SIZE;
    activeSize--;
    return c;
}

int dumpBuffer(char* destination, int size)
{
    if (size<=0 || activeSize<=0)
        return -1;

    int idx=0;
    while (idx<size-1 && activeSize){
        destination[idx] = dumpChar();
        idx++;
    }
    destination[idx]=0;
    return idx;
}