#include <pipes.h>
#include <lib.h>
#include <stddef.h>
#include <memoryManager.h>
#include <scheduler.h>

static pipeContext * pipes = NULL;
static int newFD = 2;

// 0 read y 1 write
// id > 0
int createPipe (int id, int r_or_w) {
    if (id <= 0)
        return -1;
    if(findPipe(id, 0) != NULL)
        return -1;
    
    pipeContext * newPipe = my_malloc(sizeof(pipeContext));
    if (newPipe == NULL) 
        return -1;
    
    newPipe->id = id;
    newPipe->fd[IN] = newFD++;
    newPipe->fd[OUT] = newFD++;
    newPipe->next = pipes;
    pipes = newPipe;

    if (r_or_w)
        return newPipe->fd[OUT];
    return newPipe->fd[IN];
}

int openPipe (int id, int r_or_w) {
    pipeContext * aux = findPipe(id, 0);
    if (aux != NULL) {
        if (r_or_w)
            return aux->fd[OUT];
        return aux->fd[IN];
    }
    return -1;
}

pipeContext * findPipe (int id, int fd) {
    pipeContext * aux = pipes;
    while (aux != NULL) {
        if (aux->id==id || aux->fd[IN] == fd || aux->fd[OUT] == fd)
            return aux;
        aux = aux->next;
    }
    return aux;
}

int readPipe (int fd, char * buff, int size) {
    pipeContext * aux;
    if (fd%2 == OUT || (aux = findPipe(0 , fd)) == NULL)
        return 0;
    
    while (aux->bytesToRead == 0){
        aux->first = blockNewProcess(aux->first, getCurrentProcess());
    }

    int i;
    for (i=0; i<size && aux->bytesToRead; i++) {
        buff[i] = aux->buffer[aux->readAt];
        aux->buffer[aux->readAt++] = 0;
        aux->bytesToRead--;
        if (aux->readAt >= PIPE_BUFFER_SIZE)
            aux->readAt = 0;
    }

    if (aux->first != NULL) {
        aux->first = unblockAllProcess(aux->first);
    }

    return i;
}

int writePipe (int fd, const char * buff, int size) {
    pipeContext * aux;
    if (fd%2 == IN || (aux=findPipe(0 , fd)) == NULL)
        return 0;

    while (aux->bytesToRead == PIPE_BUFFER_SIZE) {
        aux->first = blockNewProcess(aux->first, getCurrentProcess());
    }

    int i;
    for (i=0; i<size && aux->bytesToRead < PIPE_BUFFER_SIZE; i++) {
        aux->buffer[aux->writeAt++] = buff[i];
        aux->bytesToRead++;
        if (aux->writeAt >= PIPE_BUFFER_SIZE)
            aux->writeAt = 0;
    }

    if (aux->first != NULL) {
        aux->first = unblockAllProcess(aux->first);
    }
    
    return i;
}

void pipeInfo () {
    ncPrint("\nPipe ID, FDIN, FDOUT, STATE, BLOCKED PROCESSES ID\n");
    pipeContext * aux = pipes;
    blockedProcess * pcs;
    if (aux == NULL) {
        ncPrint("No Active PIPES\n");
        return;
    }
    while (aux != NULL) {
        ncPrintDec(aux->id);
        ncPrint(", ");
        ncPrintDec(aux->fd[IN]);
        ncPrint(", ");
        ncPrintDec(aux->fd[OUT]);
        ncPrint(", ");
        if(aux->bytesToRead == 0) {
            ncPrint("Empty, ");
        } else if (aux->bytesToRead == PIPE_BUFFER_SIZE) {
            ncPrint("Full, ");
        } else {
            ncPrint("Can Write and Read, ");
        }
        pcs = aux->first;
        if (pcs == NULL) {
            ncPrint("No Blocked Processes\n");
        } else {
            while (pcs != NULL) {
                ncPrintDec(pcs->head->pcb.pid);
                ncPrintChar(' ');
                pcs = pcs->next;
            }
            ncPrintChar('\n');
        }
        aux = aux->next;
    }
}