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

uint64_t helpCommand(char *args[], uint64_t args_cant);
uint64_t printmemCommand(char *args[], uint64_t args_cant);
uint64_t phyloCommand(char *args[], uint64_t args_cant);
uint64_t psCommand(char *args[], uint64_t args_cant);
uint64_t loopCommand(char *args[], uint64_t args_cant);
uint64_t killCommand(char *args[], uint64_t args_cant);
uint64_t niceCommand(char *args[], uint64_t args_cant);
uint64_t blockCommand(char *args[], uint64_t args_cant);
uint64_t semCommand(char *args[], uint64_t args_cant);
uint64_t catCommand(char *args[], uint64_t args_cant);
uint64_t wcCommand(char *args[], uint64_t args_cant);
uint64_t filterCommand(char *args[], uint64_t args_cant);
uint64_t pipeCommand(char *args[], uint64_t args_cant);


#endif
