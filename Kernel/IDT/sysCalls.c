#include <hardware.h>
#include <sysCalls.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <pipes.h>

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

void * sys_malloc (uint64_t bytes) {
    return my_malloc(bytes);
}

void sys_free (void * ap) {
    my_free(ap);
}

void sys_createProcess (void (*entryPoint)(int, char**), int argc, char ** argv, int priority, int foreground) {
    addProcess(entryPoint, argc, argv, priority, foreground);
}

void sys_killProcess (uint64_t pid) {
    changeProcessState(pid, KILLED);
}

uint64_t sys_getPID () {
    return getPID();
}

void sys_modifyState (uint64_t pid, State state) {
    changeProcessState(pid, state);
}

void sys_yield() {
    yield();
}


//MISSING PRINT PROCESSES



/*
uint64_t sys_writeAT(char * character, uint64_t bytes, int x, int y) {
    return PrintAt(character, bytes, x, y);
}
*/