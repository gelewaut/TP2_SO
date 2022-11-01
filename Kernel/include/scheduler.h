#ifndef SCHEDULER_H_
#define SCHECULER_H_

#include <stdint.h>
#include <lib.h>
#define PROCESS_SIZE 4*1024

typedef struct PCB {
    uint64_t pid;
    uint64_t ppid;
    int fd[2];
    void * rsp;
    void * rbp;
    uint64_t argc;
    char ** argv;
} PCB;

typedef enum State
{
    READY,
    BLOCKED,
    KILLED
} State;

typedef struct process {
    PCB pcb;
    State state;
    struct process * next;
} process;

typedef struct processList {
    uint64_t ready;
    process * first;
    process * last;
} processList;

typedef struct stackFrame {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;    
} stackFrame;

void initScheduler();
void * schedule();
void addProcess(void (*entryPoint)(int, char**), int argc, char ** argv);
process * newProcess(void (*entryPoint)(int, char**), int argc, char ** argv);
process * findProcess(uint64_t pid);
process * readyProcess();
void unblockProcess (uint64_t pid);
void blockProcess (uint64_t pid);

#endif