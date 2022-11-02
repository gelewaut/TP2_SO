#include <pipes.h>
#include <lib.h>
#include <stddef.h>
#include <memoryManager.h>

#define IN 0
#define OUT 1

static pipeContext * pipes = NULL;
static int newFD = 2;

int pipe (int fd[2]) {
    pipeContext * newPipe = my_malloc(sizeof(pipeContext));
    if (newPipe == NULL) 
        return -1;
    
    fd[IN] = newFD;
    newPipe->fd[IN] = newFD++;
    fd[OUT] = newFD;
    newPipe->fd[OUT] = newFD++;
    return 0;
}

pipeContext * findPipe (int fd) {
    pipeContext * aux = pipes;
    while (aux != NULL) {
        if (aux->fd[IN] == fd || aux->fd[OUT] == fd)
            return aux;
        aux = aux->next;
    }
    return aux;
}

int readPipe (int fd, char * buff, int size) {
    pipeContext * aux;
    if (fd%2 == OUT || (aux=findPipe(fd)) == NULL)
        return 0;
    
    if (aux->bytesToRead == 0){
        aux->first = blockNewProcess(aux->first, findProcess(getPID()));
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

int writePipe (int fd, char * buff, int size) {
    pipeContext * aux;
    if (fd%2 == IN || (aux=findPipe(fd)) == NULL)
        return 0;

    if (aux->bytesToRead == PIPE_BUFFER_SIZE) {
        aux->first = blockNewProcess(aux->first, findProcess(getPID()));
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