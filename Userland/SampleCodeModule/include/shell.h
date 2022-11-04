#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdint.h>
void init_shell();
void shell_welcome();
void shell_loop();
void shell_read_line();
void shell_parse_line();
uint8_t shell_execute();
uint8_t isCommand();
uint8_t runCommand(int8_t cmd);
uint8_t helpCommand();
uint8_t timeCommand();
uint64_t getAdjustedHours();
uint8_t fortuneCommand();
uint8_t inforegCommand();
uint8_t printmemCommand();
uint8_t divByZeroCommand();
uint8_t opCodeCommand();
void cleanup();
void clear_buffer();
void clear_command_buffer();
void clear_args();

#endif