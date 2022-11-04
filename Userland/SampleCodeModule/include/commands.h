#ifndef _COMMANDS_H_
#define _COMMANDS_H_

#include <stdint.h>
#include <shell.h>

static char *valid_commands[NUMBER_OF_COMMANDS+1] = {
    "exit",
    "help",     // 1
    "phylo",    // 2
    "mem",      // 3
    "divzero",  // 4
    "opcode",   // 5
    "ps",       // 6
    "loop",     // 7
    "kill",     // 8
    "nice",     // 9
    "block",    // 10
    "sem",      // 11
    "cat",      // 12
    "wc",       // 13
    "pipe"      // 14
};

uint8_t helpCommand();
uint8_t printmemCommand(uint64_t args[]);
uint8_t divByZeroCommand();
uint8_t phyloCommand();
uint8_t opCodeCommand();
uint8_t psCommand();
uint8_t loopCommand();
uint8_t killCommand();
uint8_t niceCommand();
uint8_t blockCommand();
uint8_t semCommand();
uint8_t catCommand();
uint8_t wcCommand();
uint8_t pipeCommand();


#endif
