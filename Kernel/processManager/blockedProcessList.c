#include <blockedProcessList.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <lib.h>
#include <stddef.h>

blockedProcess * blockNewProcess(blockedProcess * first, process * toBlock) {
    blockedProcess * newBlockedProcess = my_malloc(sizeof(blockedProcess));
    if (newBlockedProcess == NULL)
        return first;

    toBlock->state = BLOCKED;
    newBlockedProcess->head = toBlock;
    newBlockedProcess->next = first;
    return newBlockedProcess;
}

blockedProcess * unblockAllProcess (blockedProcess * first) {
    blockedProcess * aux;
    while(first != NULL) {
        aux = first;
        first = first->next;
        aux->head->state = READY;
        my_free(aux);
    }
    return first;
}