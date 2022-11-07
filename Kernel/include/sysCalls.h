#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <time.h>
#include <naiveConsole.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <pipes.h>
#include "./semaphore.h"

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9_rax);
uint64_t sys_read(int fd, char * buf, uint64_t count);
uint64_t sys_write(int fd, const char * buf, uint64_t count);
void * sys_malloc (uint64_t bytes);
void sys_free (void * ap);

uint64_t sys_createProcess (void (*entryPoint)(int, char**), int argc, char ** argv, int fd[2], int foreground);
void sys_killProcess (uint64_t pid);
uint64_t sys_getPID ();
void sys_blockProcess (uint64_t pid);
void sys_changePriority(uint64_t pid, uint64_t priority, uint64_t foreground);
void sys_yield();

uint64_t sys_semCreate (const char * _name, uint64_t _value);
uint64_t sys_semOpen (const char * _name);
uint64_t sys_semClose (Semaphore * sem);
uint64_t sys_semSignal (Semaphore * sem);
uint64_t sys_semWait (Semaphore * sem);

uint64_t sys_createPipe(int id, int r_or_w);
uint64_t sys_openPipe(int id, int r_or_w);

void sys_printMem();
void sys_printProcesses();
void sys_printSemaphores();
void sys_printPipes();

uint64_t sys_getTicks();

#endif