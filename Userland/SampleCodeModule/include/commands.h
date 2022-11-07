#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdint.h>
#include <shell.h>

static char *valid_commands[NUMBER_OF_COMMANDS+1] = {
    "exit",     // 0
    "help",     // 1
    "phylo",    // 2
    "mem",      // 3
    "ps",       // 4
    "loop",     // 5
    "kill",     // 6
    "nice",     // 7
    "block",    // 8
    "sem",      // 9
    "cat",      // 10
    "wc",       // 11
    "pipe",     // 12
    "filter"    // 13
};

uint64_t helpCommand(uint64_t args_cant,char *args[]);
uint64_t printmemCommand(uint64_t args_cant,char *args[]);
uint64_t phyloCommand(uint64_t args_cant,char *args[]);
uint64_t psCommand(uint64_t args_cant,char *args[]);
uint64_t loopCommand(uint64_t args_cant,char *args[]);
uint64_t killCommand(uint64_t args_cant,char *args[]);
uint64_t niceCommand(uint64_t args_cant,char *args[]);
uint64_t blockCommand(uint64_t args_cant,char *args[]);
uint64_t semCommand(uint64_t args_cant,char *args[]);
uint64_t catCommand(uint64_t args_cant,char *args[]);
uint64_t wcCommand(uint64_t args_cant,char *args[]);
uint64_t filterCommand(uint64_t args_cant,char *args[]);
uint64_t pipeCommand(uint64_t args_cant,char *args[]);


#endif
