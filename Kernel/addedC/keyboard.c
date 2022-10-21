#include <hardware.h>

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
static char buffer[BUFFER_SIZE] = {0};
int buffer_index = 0;

static const char keys[] = {
    0,'\e','1','2','3','4','5','6','7','8','9','0','-','=',127, //14
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',  //28
    0,'a','s','d','f','g','h','j','k','l',';','\'', //40
    '`',0,'\\','z','x','c','v','b','n','m',',','.','/', 0, //54
    0,0,' ' //57
};

void keyboard_handler() {
    fill_buffer();
}

uint64_t fill_buffer () {
    uint64_t key = getKey();
    if (key >= 0 && key<= 57 && buffer_index<BUFFER_SIZE) {
        if(keys[key])
            buffer[buffer_index++] = keys[key];
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