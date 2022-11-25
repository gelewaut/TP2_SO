#include <stdint.h>
#include <userlibc.h>
#include <syscalls.h>
#include <test_util.h>
#include <stddef.h>

#define TOTAL_PROCESSES 3
#define LOWEST 5 //TODO: Change as required
#define MEDIUM 3 //TODO: Change as required
#define HIGHEST 1 //TODO: Change as required

int64_t priority[TOTAL_PROCESSES] = {LOWEST, MEDIUM, HIGHEST};

void test_prio(){

    int64_t pids[TOTAL_PROCESSES];
    char *argv[] = {"endless_loop_print"};
    uint64_t i;

    int fd[2];
    fd[0] = 0;
    fd[1] = 1;
    for(i = 0; i < TOTAL_PROCESSES; i++){
      pids[i] = sys_createProcess(&endless_loop_print, 1, argv, fd, 1);
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