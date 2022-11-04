#ifndef _SYSCALLS_H_
#define _SYSCALLS_H_

#include <stdint.h>
#include <time.h>
#include <naiveConsole.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <pipes.h>


uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9_rax);
uint64_t sys_read(int fd, char * buf, uint64_t count);
uint64_t sys_write(int fd, const char * buf, uint64_t count);
void * sys_malloc (uint64_t bytes);
void sys_free (void * ap);

void sys_createProcess (void (*entryPoint)(int, char**), int argc, char ** argv, int priority, int foreground);
void sys_killProcess (uint64_t pid);
uint64_t sys_getPID ();
void sys_modifyState (uint64_t pid, State state);
void sys_yield();

#endif