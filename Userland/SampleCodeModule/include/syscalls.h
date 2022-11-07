#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>

typedef uint64_t * Semaphore;

uint64_t sys_read(int fd, char * buf, uint64_t count);
uint64_t sys_write(int fd, const char * buf, uint64_t count);
void * sys_malloc (uint64_t bytes);
void sys_free (void * ap);
void sys_printMem();

uint64_t sys_createProcess (void (*entryPoint)(int, char**), int argc, char ** argv, int fd[2], int foreground);
int sys_killProcess (uint64_t pid);
uint64_t sys_getPID ();
void sys_modifyState (uint64_t pid);
void sys_yield();
void sys_changePriority(uint64_t pid, uint64_t priority, uint64_t foreground);
void sys_printProcesses();
int sys_blockProcess(uint64_t pid);
void sys_wait(uint64_t pid);

Semaphore* sys_semCreate (const char * _name, uint64_t _value);
Semaphore* sys_semOpen (const char * _name);
uint64_t sys_semClose (Semaphore * sem);
uint64_t sys_semSignal (Semaphore * sem);
uint64_t sys_semWait (Semaphore * sem);
void sys_printSemaphores();

uint64_t sys_createPipe(int id, int r_or_w);
uint64_t sys_openPipe(int id, int r_or_w);
void sys_printPipes();

void sys_clearScreen();

uint64_t sys_getTicks();

#endif