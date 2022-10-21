#include <hardware.h>
#include <sysCalls.h>

uint64_t sys_read(int fd, char * buf, uint64_t count) {
    if (fd!=0 || count<0)
        return -1;
    
    return dump_buffer(buf, count);
}

uint64_t sys_write(int fd, const char * buf, uint64_t count) {
    int i;
    
    //falla si no hay nada en el buffer, o mal fd, o count negativo
    if (*buf==0 || fd!=0 || count<0)
        return -1;

    for(i=0; i<count; i++) {
        ncPrintChar(buf[i]);
    }
    return i;
}

uint64_t sys_writeAT(char * character, uint64_t bytes, int x, int y) {
    return PrintAt(character, bytes, x, y);
}