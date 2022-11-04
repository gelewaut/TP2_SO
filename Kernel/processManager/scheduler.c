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

    baseProcess = newProcess(&firstProcess, 0, NULL, MAX_PRIORITY, 0);
}

void * schedule(void * oldRsp) {
    if (current != NULL) {
        current->pcb.rsp = oldRsp;
        if (current->pcb.cycles--) {
            return oldRsp;
        } else {
            current->pcb.cycles = CYCLES(current->pcb.priority, current->pcb.foreground);
        }
    }
    if (processes.ready) {
        current = findReadyProcess();
        listProcess (current);
    } else {
        current = baseProcess;
    }
    
    return current->pcb.rsp;
}

void addProcess(void (*entryPoint)(int, char**), int argc, char ** argv, int priority, int foreground) {
    process * myProcess = newProcess(entryPoint, argc, argv, priority, foreground);
    if (myProcess == NULL)
        return;

    listProcess (myProcess);
}

process * newProcess(void (*entryPoint)(int, char**), int argc, char ** argv, int priority, int foreground) {
    if (entryPoint == NULL)
        return NULL;
    
    process * newProcess = (process*) my_malloc(PROCESS_SIZE);
    if (newProcess == NULL)
        return NULL;

    //things inherited from parent
    if (current != NULL) {
        newProcess->pcb.ppid = current->pcb.pid;
        //pipes??
    } else {
        newProcess->pcb.ppid = 0;
    }

    newProcess->pcb.pid = ++Next_PID;
    newProcess->pcb.rbp = newProcess+PROCESS_SIZE-1;
    newProcess->pcb.rsp = newProcess->pcb.rbp;
    newProcess->state = READY;
    newProcess->next = NULL;
    newProcess->pcb.priority = ( (priority > 0 && priority <= 10) ? priority : 1);
    newProcess->pcb.foreground = (foreground>0 ? 1:0);
    newProcess->pcb.cycles = CYCLES(newProcess->pcb.priority , newProcess->pcb.foreground);

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

    newProcess->pcb.rsp = (void *)aux->rsp;
    
    return newProcess;
}

void changeProcessState (uint64_t pid, State state) {
    process * aux = unlistProcess(pid);
    if (aux == NULL)
        return;

    if (state == KILLED) {
        if (aux->pcb.pid == current->pcb.pid)
            current = NULL;
        freeProcess(aux);
    } else {
        if (state == BLOCKED) {
            if (aux->pcb.pid == current->pcb.pid)
                current = NULL;
        }
        aux->pcb.cycles = CYCLES(aux->pcb.priority, aux->pcb.foreground);
        aux->state = state;
        listProcess(aux);
    }
    //CALL TIMER TICK
    halt(1);
}

void listProcess(process * myProcess) {
    if (processes.first == NULL) {
        processes.first = myProcess;
        processes.last = myProcess;
    } else {
        processes.last->next = myProcess;
        processes.last = myProcess;
    }

    if (myProcess->state == READY)
        processes.ready++;
}

process * unlistProcess(uint64_t pid) {
    process * previous = processes.first;
    
    if (previous == NULL) 
        return NULL;
    if (previous->pcb.pid == pid) {
        processes.first = previous->next;
        if (previous->state == READY)
            processes.ready--;

        //check if it is the last one
        // if (previous->next == NULL)
        //     processes.last == previous->next;
        return previous;
    }

    process * aux = previous->next;
    while (aux != NULL) {
        if (aux->pcb.pid == pid) {
            previous->next = aux->next;
            if (aux->state == READY)
                processes.ready--;

            //check if it the last one
            if (aux->next == NULL)
                processes.last = previous;
            return aux;
        }
        previous = aux;
        aux = aux->next;
    }
    return aux;
}

process * findReadyProcess () {
    process * previous = processes.first;
    
    if (previous == NULL) 
        return NULL;
    if (previous->state == READY) {
        processes.first = previous->next;
        processes.ready--;
        return previous;
    }

    process * aux = previous->next;
    while (aux != NULL) {
        if (aux->state == READY) {
            previous->next = aux->next;
            processes.ready--;

            //check if it is the last one
            if (aux->next == NULL)
                processes.last = previous;
            return aux;
        }
        previous = aux;
        aux = aux->next;
    }
    return aux;
}

void freeProcess(process * myProcess) {
    //Free everything inside myProcess
    my_free(myProcess);
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

uint64_t getPID () {
    if(current != NULL) {
        return current->pcb.pid;
    }
    return 0;
}

void changePriority (uint64_t pid, uint64_t priority, uint64_t foreground) {
    process * aux = findProcess(pid);
    if (aux != NULL) {
        aux->pcb.priority =  ( (priority > 0 && priority <= 10) ? priority : 1);
        aux->pcb.foreground = (foreground>0 ? 1:0);
    }
}

void yield() {
    if (current != NULL) {
        current->pcb.cycles = 0;
    }
    //CALL TIMER TICK
    halt(1);
}

process * getCurrentProcess() {
    return current;
}

process * getProcesses () {
    return processes.first;
}



/*
TO ELIMINATE
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

void killProcess (uint64_t pid) {
    process * aux = findProcess(pid);
    if (aux != NULL) {
        if (aux->state == READY) 
            processes.ready--;
        aux->state = KILLED;
    }
    if (pid == current->pcb.pid) {
        current->pcb.cycles = 0;
    }
    //CALL TIMER TICK
    halt(1);
}

process * readyProcess() {
    if (!processes.ready) {
        return NULL;
    }

    process * previous = processes.first;
    while (previous->state == KILLED) {
        processes.first = previous->next;
        my_free(previous);
        previous = processes.first; 
    }
    
    //see if the first is ready
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
        } else if (aux->state == KILLED) {
            process * toKILL = aux;
            aux = aux->next;
            my_free(toKILL);
        } else {
            previous = aux;
            aux = aux->next;
        }
    }
    return aux;
}
*/

