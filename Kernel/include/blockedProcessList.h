#ifndef BLOCKEDPROCESSLIST_H_
#define BLOCKEDPROCESSLIST_H_

#include <scheduler.h>

typedef struct blockedProcess {
    struct process * head;
    struct blockedProcess * next;
} blockedProcess;

typedef struct waiting_list {
  uint64_t pid;
  struct waiting_list *next;
} waiting_list;

blockedProcess * blockNewProcess(blockedProcess * first, process * toBlock);
blockedProcess * unblockAllProcess(blockedProcess * first);
waiting_list * addProcessWait(waiting_list * first, uint64_t pid);
waiting_list * unblockWaitingList(waiting_list * first);

#endif