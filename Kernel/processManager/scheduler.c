#include <scheduler.h>
#include <memoryManager.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <stddef.h>

static uint64_t Next_PID = 0;
static processList processes;
static process * current = NULL;
static process * baseProcess = NULL;

static void wrapper(void (*entryPoint)(int, char**), int argc, char ** argv) {
    (*entryPoint)(argc,argv);
    // exit();
}

static void firstProcess() {
    while(1)
        // halt(1);
        ncPrintChar('.');
}

void initScheduler() {
    
    processes.ready = 0;
    processes.first = NULL;
    processes.last = NULL;

    // char ** argc = {"First Process"};
    baseProcess = newProcess(&firstProcess, 0, NULL);
    // current = baseProcess;
}

void * schedule(void * oldRsp) {
    // if(current == NULL || processes.ready == 0) {
    // }
    // return oldRsp;
    // /*
    if (current == NULL) {
        current = baseProcess;
        return baseProcess->pcb.rsp;
    }
    current->pcb.rsp = oldRsp;
    return current->pcb.rsp;
    // */
}


void addProcess(void (*entryPoint)(int, char**), int argc, char ** argv) {
    process * myProcess = newProcess(entryPoint, argc, argv);
    if (myProcess == NULL)
        return;
    
    if (processes.first == NULL) {
        processes.first = myProcess;
    } else {
        processes.last->next = myProcess;
    }
    processes.ready++;
}

process * newProcess(void (*entryPoint)(int, char**), int argc, char ** argv) {
    if (entryPoint == NULL)
        return NULL;
    
    process * newProcess = (process*) my_malloc(PROCESS_SIZE);
    if (newProcess == NULL)
        return NULL;

    newProcess->pcb.pid = ++Next_PID;
    newProcess->pcb.rbp = newProcess+PROCESS_SIZE-1;
    newProcess->pcb.rsp = newProcess->pcb.rbp;
    newProcess->state = READY;
    newProcess->next = NULL;

    stackFrame * aux = (stackFrame *) newProcess->pcb.rbp - 1;
    aux->rsi = argc;
    aux->rdi = (uint64_t) entryPoint;
    aux->rdx = (uint64_t) argv;
    aux->rip = (uint64_t) wrapper;
    aux->rsp = (uint64_t) aux;
    aux->rbp = (uint64_t) newProcess->pcb.rbp;
    aux->cs = 0x008;
    aux->rflags = 0x202;
    aux->ss = 0x000;

    newProcess->pcb.rsp = aux->rsp;
    
    return newProcess;
}
