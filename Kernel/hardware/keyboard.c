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

static char buffer[BUFFER_SIZE] = {0};
int buffer_index = 0;

static const char keys[] = {
    0,'\e','1','2','3','4','5','6','7','8','9','0','-','=',127, //14
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',  //28
    0,'a','s','d','f','g','h','j','k','l',';','\'', //40
    '`',0,'\\','z','x','c','v','b','n','m',',','.','/', 0, //54
    0,0,' ',//57
};

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

    if (key >= 0 && key<= 57 && buffer_index < BUFFER_SIZE) {
        int token;
        if((token = keys[key])) {
            if (shift_enabled == 1) {
                if (key == 8) {
                    token = '&'; 
                } else if (key == 43) {
                    token = '|';
                }
            }
            buffer[buffer_index++] = token;
        }
    }
}

uint64_t dump_buffer(char * dest, uint64_t size) {
    if(dest == 0 || size <= 0) {
        return -1;
    }
    
    int index = 0;
    for (; index < size && buffer_index > 0; index++) {
        buffer[buffer_index--] = 0;
        dest[index] = buffer[buffer_index];
    }
    return index;
}



char * get_buffer () {
    return buffer;
}

int buffer_count () {
    return buffer_index;
}

void clear_buffer () {
    while (buffer_index>0)
        buffer[buffer_index--] = 0;
}