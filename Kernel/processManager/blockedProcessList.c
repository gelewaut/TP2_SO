#include <blockedProcessList.h>
#include <memoryManager.h>
#include <scheduler.h>
#include <lib.h>
#include <stddef.h>

waiting_list * addProcessWait(waiting_list * first, uint64_t pid) {
    process * auxProcess = findProcess(pid);
    if(auxProcess == NULL) {
        return first;
    }
    waiting_list * aux = my_malloc(sizeof(waiting_list));
    if (aux == NULL) {
        return first;
    }
    auxProcess->state = BLOCKED;
    aux->pid = pid;
    aux->next = first;
    return aux;
}

waiting_list * unblockWaitingList(waiting_list * first) {
    waiting_list * aux;
    process * auxProcess;
    while (first != NULL) {
        aux = first;
        first = first->next;
        if ((auxProcess = findProcess(aux->pid)) != NULL) {
            auxProcess->state = READY;
        }
        my_free(aux);
    }
    return first;
}

blockedProcess * blockNewProcess(blockedProcess * first, process * toBlock) {
    blockedProcess * newBlockedProcess = my_malloc(sizeof(blockedProcess));
    if (newBlockedProcess == NULL)
        return first;
    
    toBlock->state = BLOCKED;
    newBlockedProcess->head = toBlock;
    
    if (first == NULL) {
        newBlockedProcess->next = NULL;
    } else {
        newBlockedProcess->next = first;
    }
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