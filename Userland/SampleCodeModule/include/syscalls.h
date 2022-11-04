#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

typedef enum State
{
    READY,
    BLOCKED,
    KILLED
} State;

typedef uint64_t * Semaphore;

uint64_t sys_read(int fd, char * buf, uint64_t count);
uint64_t sys_write(int fd, const char * buf, uint64_t count);
void * sys_malloc (uint64_t bytes);
void sys_free (void * ap);

void sys_createProcess (void (*entryPoint)(int, char**), int argc, char ** argv, int priority, int foreground);
void sys_killProcess (uint64_t pid);
uint64_t sys_getPID ();
void sys_modifyState (uint64_t pid, State state);
void sys_yield();

uint64_t sys_semCreate (const char * _name, uint64_t _value);
uint64_t sys_semOpen (const char * _name);
uint64_t sys_semClose (Semaphore * sem);
uint64_t sys_semSignal (Semaphore * sem);
uint64_t sys_semWait (Semaphore * sem);

uint64_t sys_createPipe(int id, int r_or_w);
uint64_t sys_openPipe(int id, int r_or_w);

#endif