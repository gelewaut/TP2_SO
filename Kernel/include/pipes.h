#ifndef PIPES_H_
#define PIPES_H_

#include <blockedProcessList.h>

#define PIPE_BUFFER_SIZE 50

typedef struct pipeContext {
    int fd[2];
    char buffer [PIPE_BUFFER_SIZE];
    int writeAt;
    int readAt;
    int bytesToRead;
    blockedProcess * first;
    struct pipeContext * next;
} pipeContext;


#endif