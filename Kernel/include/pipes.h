#ifndef PIPES_H_
#define PIPES_H_

#include <blockedProcessList.h>

#define IN 0
#define OUT 1
#define PIPE_BUFFER_SIZE 50

typedef struct pipeContext {
    int id;
    int fd[2];
    char buffer [PIPE_BUFFER_SIZE];
    int writeAt;
    int readAt;
    int bytesToRead;
    blockedProcess * first;
    struct pipeContext * next;
} pipeContext;

int createPipe (int id, int r_or_w);
int openPipe (int id, int r_or_w);
pipeContext * findPipe (int id, int fd);
int readPipe (int fd, char * buff, int size);
int writePipe (int fd, const char * buff, int size);
void pipeInfo();

#endif