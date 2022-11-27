#include <scheduler.h>
#include <memoryManager.h>
#include <naiveConsole.h>
#include <stdint.h>
#include <stddef.h>
#include <my_time.h>
#include <blockedProcessList.h>
#include <lib.h>
#include <semaphore.h>

static uint64_t Next_PID = 0;
static processList processes;
static process * current = NULL;
static process * baseProcess = NULL;

static void wrapper(void (*entryPoint)(int, char**), int argc, char ** argv) {
    (*entryPoint)(argc,argv);
    exit();
}

static void firstProcess() {
    while(1)
        halt(1);
        // ncPrint('.');

}

void exit() {
    changeProcessState(current->pcb.pid, KILLED);
    // halt(1);
    call_timerTick();
}

void initScheduler() {
    
    processes.ready = 0;
    processes.count = 0;
    processes.first = NULL;
    processes.last = NULL;

    int fd[2];
    fd[0] = 0;
    fd[1] = 1;
    baseProcess = newProcess(&firstProcess, 0, NULL, fd, 0);
}

void * schedule(void * oldRsp) {
    if (current != NULL) {
        current->pcb.rsp = oldRsp;
        if (current->state == READY && current->pcb.cycles-- ) {
            return oldRsp;
        } else {
            current->pcb.cycles = CYCLES(current->pcb.priority, current->pcb.foreground);
        }
    }
    if (processes.ready) {
        current = findReadyProcess();
        if (current == NULL) {
            current = baseProcess;
        } else {
            listProcess (current);
        }
    } else {
        current = baseProcess;
    }
    
    return current->pcb.rsp;
}

uint64_t addProcess(void (*entryPoint)(int, char**), int argc, char ** argv, int fd[2], int foreground) {
    process * myProcess = newProcess(entryPoint, argc, argv, fd, foreground);
    if (myProcess == NULL)
        return 0;

    listProcess (myProcess);
    return myProcess->pcb.pid;
}

process * newProcess(void (*entryPoint)(int, char**), int argc, char ** argv, int fd[2], int foreground) {
    if (entryPoint == NULL)
        return NULL;
    
    process * newProcess = (process*) my_malloc(PROCESS_SIZE);
    if (newProcess == NULL)
        return NULL;

    //things inherited from parent
    if (current != NULL) {
        newProcess->pcb.ppid = current->pcb.pid;
    } else {
        newProcess->pcb.ppid = 0;
    }

    copyArgs(newProcess, argc, argv);

    newProcess->pcb.pid = ++Next_PID;
    newProcess->pcb.rbp = newProcess+PROCESS_SIZE-1;
    newProcess->pcb.rsp = newProcess->pcb.rbp;
    newProcess->pcb.fd[0] = fd[0];
    newProcess->pcb.fd[1] = fd[1];
    newProcess->pcb.start = NULL;
    newProcess->pcb.foreground = (foreground>0 ? 1:0);
    newProcess->pcb.cycles = CYCLES(newProcess->pcb.priority , newProcess->pcb.foreground);
    newProcess->pcb.priority = 1;
    newProcess->state = READY;
    newProcess->next = NULL;

    stackFrame * aux = (stackFrame *) newProcess->pcb.rbp - 1;
    aux->rsi = newProcess->pcb.argc;
    aux->rdi = (uint64_t) entryPoint;
    aux->rdx = (uint64_t) newProcess->pcb.argv;
    aux->rip = (uint64_t) wrapper;
    aux->rsp = (uint64_t) aux;
    aux->rbp = (uint64_t) newProcess->pcb.rbp;
    aux->cs = 0x008;
    aux->rflags = 0x202;
    aux->ss = 0x000;

    newProcess->pcb.rsp = (void *)aux->rsp;
    
    return newProcess;
}

void copyArgs(process * newProcess, int argc, char ** argv) {
    if (argc > 0) {
        my_strcpy(newProcess->pcb.name, argv[0]);
        // newProcess->pcb.name = my_malloc(my_strlen(argv[0]));
        // if (newProcess->pcb.name != NULL) {
            // newProcess->pcb.name = my_strcpy (newProcess->pcb.name, argv[0]);
    } else {
        newProcess->pcb.name[0] = '\0';
        newProcess->pcb.argc = 0;
        newProcess->pcb.argv = NULL;
        return;
    }
    int i, flag=1;

    char ** args = my_malloc(sizeof(char *) * argc-1);
    if (args != NULL) {
        for (i=0; i<argc-1 && flag; i++) {
            args[i] = my_malloc(my_strlen(argv[i+1]));
            if (args[i] != NULL) {
                args[i] = my_strcpy(args[i], argv[i+1]);
            } else {
                flag = 0;
            }
        }
        //couldnt copy the argv
        if (flag == 0) {
            for (int j=0; j<i; j++) {
                my_free(args[j]);
            }
            my_free(args);
            newProcess->pcb.argc = 0;
            newProcess->pcb.argv = NULL;
        } else {
            newProcess->pcb.argc = i;
            newProcess->pcb.argv = args;
        }
    } else {
        newProcess->pcb.argc = 0;
        newProcess->pcb.argv = NULL;
    }
}

int changeProcessState (uint64_t pid, State state) {
    process * aux = unlistProcess(pid);
    uint64_t pid1, pid2;
    if (aux == NULL)
        return -1;

    if (state == KILLED) {
        pid1 = aux->pcb.pid;
        pid2 = current->pcb.pid;
        if (aux->pcb.pid == current->pcb.pid)
            current = NULL;
        if (aux->pcb.start != NULL) {
            aux->pcb.start = unblockWaitingList(aux->pcb.start);
        }
        char c = -1;
        writePipe(aux->pcb.fd[1], &c, 1);
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
    
    call_timerTick();

    return 0;
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
    processes.count++;
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
        if (previous->next == NULL)
            processes.last = NULL;
        processes.count--;
        previous->next = NULL;
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
            
            aux->next = NULL;
            processes.count--;
            return aux;
        }
        previous = aux;
        aux = aux->next;
    }
    return NULL;
}

process * findReadyProcess () {
    process * previous = processes.first;
    
    if (previous == NULL) 
        return NULL;
    if (previous->state == READY) {
        processes.first = previous->next;
        processes.ready--;
        processes.count--;
        previous->next = NULL;
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
            aux->next = NULL;
            processes.count--;
            return aux;
        }
        previous = aux;
        aux = aux->next;
    }
    return NULL;
}

void freeProcess(process * myProcess) {
    //Free everything inside myProcess
    // my_free(myProcess->pcb.name);
    for (int i=0; i<myProcess->pcb.argc; i++) {
        my_free(myProcess->pcb.argv[i]);
    }
    my_free(myProcess->pcb.argv);
    my_free(myProcess);
}

process * findProcess(uint64_t pid) {
    process * aux = processes.first;
    while (aux != NULL) {
        if (aux->pcb.pid == pid)
            return aux;
        aux = aux->next;
    }
    return NULL;
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
    // halt(1);
    call_timerTick();
}

void wait(uint64_t pid) {
    process * aux = findProcess(pid);
    if (aux == NULL || aux->state == KILLED)
        return;
    aux->pcb.start = addProcessWait(aux->pcb.start, getPID());
    // halt(1);
    call_timerTick();
}

process * getCurrentProcess() {
    return current;
}


//process id, name, stack, base pointer, state, priority, foreground
void schedulerInfo() {
    process * aux = processes.first;
    ncPrint("\nProcess Name, ID, SP, BP, FDIN, FDOUT, Priority, State\n");
    if (aux == NULL) {
        ncPrint("Couldnt get Processes");
        return;
    }
    while (aux != NULL) {
        ncPrint(aux->pcb.name);
        ncPrint(", ");
        ncPrintDec(aux->pcb.pid);
        ncPrint(", ");
        ncPrintHex((uint64_t)aux->pcb.rsp);
        ncPrint(", ");
        ncPrintHex((uint64_t)aux->pcb.rbp);
        ncPrint(", ");
        ncPrintDec(aux->pcb.fd[0]);
        ncPrint(", ");
        ncPrintDec(aux->pcb.fd[1]);
        ncPrint(", ");
        ncPrintDec(aux->pcb.priority);
        ncPrint(", ");
        if (aux->state == READY) {
            ncPrintChar('R');
        } else if (aux->state == BLOCKED) {
            ncPrintChar('B');
        } else {
            ncPrintChar('K');
        }
        ncPrintChar('\n');
        aux = aux->next;
    }
    return;
}