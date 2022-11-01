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

    baseProcess = newProcess(&firstProcess, 0, NULL);
}

void blockProcess (uint64_t pid) {
    process * aux = findProcess(pid);
    if (aux != NULL) {
        aux->state = BLOCKED;
        processes.ready--;
        //CALL TIMER TICK
        halt(1);
    }
}

void unblockProcess (uint64_t pid) {
    process * aux = findProcess(pid);
    if (aux != NULL) {
        aux->state = READY;
        processes.ready++;
        //CALL TIMER TICK
        halt(1);
    }
}

process * findProcess(uint64_t pid) {
    process * aux = processes.first;
    while (aux != NULL) {
        if (aux->pcb.pid == pid)
            return aux;
        aux = aux->next;
    }
    return aux;
}

process * readyProcess() {
    if (!processes.ready) {
        return NULL;
    }
    //see if the first is ready
    process * previous = processes.first;
    if (previous->state == READY) {
        processes.first = processes.first->next;
        processes.last->next = previous;
        previous->next = NULL;
        processes.last = previous;
        return previous;
    }

    process * aux = previous->next;
    int found = 0;

    while ( aux!=NULL && !found ) {
        if (aux->state == READY) {
            found = 1;
            //watch if ready is the last one
            if (aux->pcb.pid != processes.last->pcb.pid) {
                previous->next = aux->next;
                aux->next = NULL;
                processes.last->next = aux;
                processes.last = aux;
            }
        } else {
            previous = aux;
            aux = aux->next;
        }
    }
    return aux;
}

void * schedule(void * oldRsp) {
    if (current != NULL) {
        current->pcb.rsp = oldRsp;
    } 
    if (processes.ready) {
        current = readyProcess();
        //solo un elemento en la lista
        /*
        if (processes.first->next == NULL) { 
            current = processes.first;
        } else {
            current = processes.first;
            processes.first = processes.first->next;
            processes.last->next = current;
            current->next = NULL;
            processes.last = current;
        }
        */
    } else {
        current = baseProcess;
    }
    
    return current->pcb.rsp;
}


void addProcess(void (*entryPoint)(int, char**), int argc, char ** argv) {
    process * myProcess = newProcess(entryPoint, argc, argv);
    if (myProcess == NULL)
        return;
    
    if (processes.first == NULL) {
        processes.first = myProcess;
        processes.last = myProcess;
    } else {
        processes.last->next = myProcess;
        processes.last = myProcess;
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
