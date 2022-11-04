#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <stdint.h>
#include <lib.h>
#define PROCESS_SIZE 4*1024
#define MAX_PRIORITY 10
//ammount of cicles priority 1 -> 10 cycles
//foreground, cycles + 1
// #define CYCLES(p) (p<MAX_PRIORITY && p>0) ? (MAX_PRIORITY+1-p) : 1
#define CYCLES(p,f) (MAX_PRIORITY+f-p)

typedef struct PCB {
    uint64_t pid;
    uint64_t ppid;
    uint64_t priority;
    uint64_t cycles;
    uint64_t foreground;
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
void * schedule(void * oldRsp);
void addProcess(void (*entryPoint)(int, char**), int argc, char ** argv, int priority, int foreground);
process * newProcess(void (*entryPoint)(int, char**), int argc, char ** argv, int priority, int foreground);
void changeProcessState (uint64_t pid, State state);
void listProcess(process * myProcess);
process * unlistProcess(uint64_t pid);
process * findReadyProcess ();
void freeProcess(process * myProcess);
process * findProcess(uint64_t pid);
uint64_t getPID ();
void changePriority (uint64_t pid, uint64_t priority, uint64_t foreground);
void yield();
process * getCurrentProcess();
process * getProcesses ();

#endif