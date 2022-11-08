#include <stdint.h>
#include <userlibc.h>
#include <syscalls.h>
#include <test_util.h>
#include <stddef.h>

#define MINOR_WAIT 1000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT      10000000 // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

#define TOTAL_PROCESSES 3
#define LOWEST 1 //TODO: Change as required
#define MEDIUM 2 //TODO: Change as required
#define HIGHEST 3 //TODO: Change as required

int64_t priority[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(uint64_t args_cant, char *args[]){

    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {"endless_loop_print"};
    uint64_t i;

    int fd[2];
    fd[0] = 0;
    fd[1] = 1;
    for(i = 0; i < TOTAL_PROCESSES; i++){
      pids[i] = sys_createProcess(&endless_loop_print, 2, argv, fd, 1);
    }

    bussy_wait( TOTAL_PROCESSES * WAIT);

    printf("\nCAMBIANDO PRIORIDADES\n");

    for(i = 0; i < TOTAL_PROCESSES; i++){
      sys_changePriority(pids[i], priority[i],1);
    }

    bussy_wait(TOTAL_PROCESSES * WAIT);

    printf("\nBLOQUEANDO...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++)
      sys_blockProcess(pids[i]);

    printf("CAMBIANDO PRIORIDADES MIENTRAS ESTA BLOQUEADO...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++)
      sys_changePriority(pids[i], MEDIUM,1);

    printf("DESBLOQUEANDO...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++)
      sys_blockProcess(pids[i]);

    bussy_wait( TOTAL_PROCESSES * WAIT);
    printf("\nMATANDO LOS PROCESOS...\n");

    for(i = 0; i < TOTAL_PROCESSES; i++)
      sys_killProcess(pids[i]);
    return;
}