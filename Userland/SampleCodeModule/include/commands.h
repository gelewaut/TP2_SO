#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdint.h>
#include <shell.h>
#include <tests.h>

static char *valid_commands[NUMBER_OF_COMMANDS+1] = {
    "exit",     // 0
    "help",     // 1
    "pipe",     // 2
    "mem",      // 3
    "ps",       // 4
    "sem",      // 5

    "kill",     // 6
    "nice",     // 7
    "block",    // 8
    "unblock",   // 9
    "testmm",   // 10
    "testprio", // 11
    "testprocesses",    // 11
    "testsync", // 13

    "cat",      // 14
    "wc",       // 15
    "loop",     // 16
    "phylo",    // 17
    "filter"    // 18
};

uint64_t helpCommand(uint64_t args_cant,char *args[]);
uint64_t printmemCommand(uint64_t args_cant,char *args[]);
uint64_t phyloCommand(uint64_t args_cant,char *args[]);
uint64_t psCommand(uint64_t args_cant,char *args[]);
uint64_t loopCommand(uint64_t args_cant,char *args[]);
uint64_t killCommand(uint64_t args_cant,char *args[]);
uint64_t niceCommand(uint64_t args_cant,char *args[]);
uint64_t blockCommand(uint64_t args_cant,char *args[]);
uint64_t unblockCommand( uint64_t args_cant,char *args[]);
uint64_t semCommand(uint64_t args_cant,char *args[]);
uint64_t catCommand(uint64_t args_cant,char *args[]);
uint64_t wcCommand(uint64_t args_cant,char *args[]);
uint64_t filterCommand(uint64_t args_cant,char *args[]);
uint64_t pipeCommand(uint64_t args_cant,char *args[]);

uint64_t test_mm_command(uint64_t argc, char ** argv);
uint64_t test_sync_command(uint64_t argc, char ** argv);
uint64_t test_pcs_command(uint64_t argc, char ** argv);
uint64_t test_prio_command(uint64_t argc, char ** argv);


#endif
