#ifndef BLOCKEDPROCESSLIST_H_
#define BLOCKEDPROCESSLIST_H_

#include <scheduler.h>

typedef struct blockedProcess {
    struct process * head;
    struct blockedProcess * next;
} blockedProcess;

blockedProcess * blockNewProcess(blockedProcess * first, process * toBlock);
blockedProcess * unblockAllProcess(blockedProcess * first);

#endif