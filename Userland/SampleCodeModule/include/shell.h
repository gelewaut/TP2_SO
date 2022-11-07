#ifndef _SHELL_H_
#define _SHELL_H_

#include <stdint.h>
#include <userlibc.h>

#define PROMPT "$>"
#define MAX_BUFFER 128
#define MAX_COMMAND_LENGHT 7
#define MAX_ARGS 1
#define MAX_ARG_LENGHT 16
#define NUMBER_OF_COMMANDS 14

#define BACKSPACE 127
#define ENTER '\n'
#define ESC 27

#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

void init_shell();
void shell_welcome();
void shell_loop();
void shell_read_line();
void shell_parse_line();
uint8_t shell_execute();


uint8_t isCommand();
uint64_t commandDispatcher(uint64_t cmd, int argc, char *argv[], int fd[2]);

void cleanup();
void clear_buffer();
void clear_command_buffer();
void clear_args();

#endif