#include <hardware.h>
#include <sysCalls.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <pipes.h>
#include "../include/semaphore.h"

uint64_t sys_read(int fd, char * buf, uint64_t count) {
    if (count<=0 || fd != 0)
        return -1;
    process * aux = getCurrentProcess();
    if (aux == NULL) {
        return -1;
    }
    
    if (aux->pcb.fd[0] == 0) {
        clear_buffer();
        while(buffer_count()<count) {
            fill_buffer();
        }
        char * bufferAux = get_buffer();
        int i;
        for (i=0; i<count; i++) {
            buf[i] = bufferAux[i];
        }
        clear_buffer();
        return i;
        // return dump_buffer(buf, count);
    }
    return readPipe(aux->pcb.fd[0], buf, count);
}

uint64_t sys_write(int fd, const char * buf, uint64_t count) {
    int i;
    
    //falla si no hay nada en el buffer, o count negativo
    if (*buf==0 || count<0 || fd != 1)
        return -1;
    process * aux = getCurrentProcess();
    if (aux == NULL) {
        return -1;
    }
    
    if (aux->pcb.fd[1] == 1) {
        for(i=0; i<count; i++) {
            ncPrintChar(buf[i]);
        }
        return i;
    }
    return writePipe(aux->pcb.fd[1], buf, count);
}

void * sys_malloc (uint64_t bytes) {
    return my_malloc(bytes);
}

void sys_free (void * ap) {
    my_free(ap);
}

uint64_t sys_createProcess(void (*entryPoint)(int, char**), int argc, char ** argv, int fd[2], int foreground) {
    return addProcess(entryPoint, argc, argv, fd, foreground);
}

void sys_killProcess (uint64_t pid) {
    changeProcessState(pid, KILLED);
}

uint64_t sys_getPID () {
    return getPID();
}

void sys_blockProcess (uint64_t pid) {
    process * aux = findProcess(pid);
    if (aux == NULL)
        return;
    if (aux->state == READY) {
       return changeProcessState(pid, BLOCKED);
    } else if (aux->state == BLOCKED){
        return changeProcessState(pid, READY);
    }
}

void sys_changePriority(uint64_t pid, uint64_t priority, uint64_t foreground) {
    changePriority(pid, priority, foreground);
}

void sys_yield() {
    yield();
}

uint64_t sys_semCreate (const char * _name, uint64_t _value) {
    return (uint64_t) sem_create(_name, _value);
}

uint64_t sys_semOpen (const char * _name) {
    return (uint64_t) sem_open(_name);
}

uint64_t sys_semClose (Semaphore * sem) {
    return (uint64_t) sem_close(sem);
}

uint64_t sys_semSignal (Semaphore * sem) {
    return (uint64_t) sem_signal(sem);
}

uint64_t sys_semWait (Semaphore * sem) {
    return (uint64_t) sem_wait(sem);
}

uint64_t sys_createPipe(int id, int r_or_w) {
    return createPipe(id, r_or_w);
}

uint64_t sys_openPipe(int id, int r_or_w) {
    return openPipe(id, r_or_w);
}

void sys_printMem() {
    print_memSet();
}

void sys_printProcesses() {
    schedulerInfo();
}

void sys_printSemaphores() {
    sem_info();
}

void sys_printPipes() {
    pipeInfo();
}

//MISSING PRINT MEM
//MISSING PRINT PROCESSES
//MISSING PRINT PIPES
//MISSING PRINT SEMAPHORES


/*
uint64_t sys_writeAT(char * character, uint64_t bytes, int x, int y) {
    return PrintAt(character, bytes, x, y);
}
*/