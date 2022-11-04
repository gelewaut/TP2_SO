#include <stdint.h>
#include <syscalls.h>
#include <phylosophers.h>
#include <commands.h>
#include <shell.h>
#include <userlibc.h>


uint8_t helpCommand()
{
    printf("\nThese are the available commands");
    for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        putChar('\n');
        printf(valid_commands[i]);
    }
    putChar('\n');
    return EXIT_SUCCESS;
}

uint8_t printmemCommand(uint64_t args[])
{
    int auxArg = strToNum(args);
    if (auxArg == -1)
    {
        printf("\n A printmem SE LE DEBE PASAR UN NUMERO COMO PARAMETRO\n");
        return EXIT_SUCCESS;
    }
//     sys_printMem(auxArg);
    return EXIT_SUCCESS;
}

uint8_t divByZeroCommand()
{
    // divByZero();
    return EXIT_SUCCESS;
}

uint8_t phyloCommand(){
    run_phylosophers(5);
    return EXIT_SUCCESS;
}

uint8_t opCodeCommand()
{
    // opCodeTest();
    return EXIT_SUCCESS;
}

uint8_t psCommand(){
    return 0;
}

uint8_t loopCommand(){
    return 0;
}

uint8_t killCommand(){
    return 0;
}

uint8_t niceCommand(){
    return 0;
}

uint8_t blockCommand(){
    return 0;
}

uint8_t semCommand(){
    return 0;
}

uint8_t catCommand(){
    return 0;
}

uint8_t wcCommand(){
    return 0;
}

uint8_t pipeCommand(){
    return 0;
}