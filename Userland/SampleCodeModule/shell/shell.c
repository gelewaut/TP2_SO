#include <shell.h>
#include <userlibc.h>
#include <stdint.h>
#include <syscalls.h>
#include <tests.h>
#include <phylosophers.h>

#define PROMPT "$>"
#define MAX_BUFFER 128
#define MAX_COMMAND_LENGHT 9
#define MAX_ARGS 1
#define MAX_ARG_LENGHT 16
#define NUMBER_OF_COMMANDS 8

#define BACKSPACE 127
#define ENTER '\n'

#define EXIT_SUCCESS 1
#define EXIT_FAILURE 0

static char shell_buffer[MAX_BUFFER + 1] = {0};
static int bufferIdx = 0;
static char command_buffer[MAX_COMMAND_LENGHT + 1] = {0};
static char args[MAX_ARG_LENGHT + 1] = {0};

static char *valid_commands[NUMBER_OF_COMMANDS] = {
    "exit",
    "help",     // 1
    "time",     // 2
    "phylo",    // 3
    "mem",      // 4
    "divzero",  // 5
    "opcode",   // 6
    "ps",       // 7
    "loop",     // 8
    "kill",     // 9
    "nice",     // 10
    "block",    // 11
    "sem",      // 12
    "cat",      // 13
    "wc",       // 14
    "pipe"     // 15
};

// static uint8_t args_for_command[NUMBER_OF_COMMANDS] = {
//     0,
//     0,
//     0,
//     0,
//     0};

void init_shell()
{
    // setup
    clear();
    cleanup();
    shell_welcome();

    //loop
    shell_loop();

    //shutdown
}

void shell_welcome()
{
    printf("WELCOME TO THE SHELL");
}

void shell_loop()
{
    uint8_t mustContinue = 1;

    do
    {
        putChar('\n');
        printf(PROMPT);
        shell_read_line();
        shell_parse_line();
        mustContinue = shell_execute();
        cleanup();

    } while (mustContinue);
}

void shell_read_line()
{
    uint8_t c;

    while ((c = getChar()) != ENTER)
    {
        if (c == BACKSPACE)
        {
            if (bufferIdx > 0)
            {
                shell_buffer[--bufferIdx] = 0;
                putChar(c);
            }
        }
        else
        {
            shell_buffer[bufferIdx++] = c;
            putChar(c);
        }
    }
}

void shell_parse_line()
{
    uint8_t args_counter = 0;
    uint8_t buffer_idx = 0;
    uint8_t aux_idx = 0;

    uint8_t token;

    while (shell_buffer[buffer_idx])
    {
        token = shell_buffer[buffer_idx++];
        if (token == ' ')
        {
            args_counter++;
            aux_idx = 0;
            continue;
        }
        if (args_counter == 0)
        {
            command_buffer[aux_idx++] = token;
        }
        else
        {
            args[aux_idx++] = token;
        }
    }
}

uint8_t shell_execute()
{
    int8_t cmd = isCommand(), result = 1;
    if (cmd > 0)
    {
        result = runCommand(cmd);
    }
    else if (cmd == 0)
    {
        return 0;
    }
    else
    {
        printf("\nUnknown command type help for a list of commands");
    }
    cleanup();
    return result;
}

uint8_t isCommand()
{
    for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
    {
        if (string_compare(command_buffer, valid_commands[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

uint8_t runCommand(int8_t cmd)
{
    switch (cmd)
    {
    // En cada caso puedo definir si le paso args si es necesario
    // y cuantos les paso
    case 1:
    {
        return helpCommand();
        break;
    }
    case 2:
    {
        return timeCommand();
        break;
    }
    case 3:
    {
        return phyloCommand();
        break;
    }
    case 4:
    {
        return printmemCommand(args);
        break;
    }
    case 5:
    {
        return divByZeroCommand();
        break;
    }
    case 6:
    {
        return opCodeCommand();
        break;
    }
    }
    return -1;
}

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

uint8_t timeCommand()
{
    putChar('\n');

    uint64_t hours, minutes, seconds;
    hours = getAdjustedHours();
    minutes = sys_clock(2);
    seconds = sys_clock(0);

    // arreglando esto
    if (hours < 10){
        putChar('0');
    }
    printDec(hours);
    putChar(':');
    if (minutes < 10){
        putChar('0');
    }
    printHex(minutes);
    putChar(':');
    if (seconds < 10){
        putChar('0');
    }
    printHex(seconds);
    putChar(' ');
    printHex(sys_clock(7));
    putChar('/');
    printHex(sys_clock(8));
    putChar('/');
    printHex(sys_clock(9));

    return EXIT_SUCCESS;
}

uint64_t getAdjustedHours()
{
    uint64_t gmt_hours = sys_clock(4);

    if (gmt_hours < 3)
    {
        return gmt_hours + 21;
    }
    return gmt_hours - 3;
}

uint8_t phyloCommand(){
    uint8_t c;
    uint8_t phyloCant = 0;
    while ((c = getChar()) != ENTER)
    {
        printf("PRESIONE 'a' PARA AGREGAR UN FILOSOFO Y 'r' PARA REMOVER UNO, PRESIONE ENTER PARA COMENZAR\n");
        if(c == 'a'){
            phyloCant++;
        }else if (phyloCant != 0 && c == 'r')
        {
            phyloCant--;
        }
        printf("Cantidad de filosofos: %d\n", phyloCant);        
    }
    
    if(phyloCant > 0){
        run_phylosophers(phyloCant);
    }

}

uint8_t printmemCommand(char arg[])
{
    int auxArg = strToNum(arg);
    if (auxArg == -1)
    {
        print("\n A printmem SE LE DEBE PASAR UN NUMERO COMO PARAMETRO\n");
        return EXIT_SUCCESS;
    }
    sys_printMem(auxArg);
    return EXIT_SUCCESS;
}

uint8_t divByZeroCommand()
{
    divByZero();
    return EXIT_SUCCESS;
}

uint8_t opCodeCommand()
{
    opCodeTest();
    return EXIT_SUCCESS;
}

void cleanup()
{
    clear_buffer();
    clear_command_buffer();
    clear_args();
}

void clear_buffer()
{
    for (int i = 0; i <= bufferIdx; i++)
        shell_buffer[i] = 0;
    bufferIdx = 0;
}

void clear_command_buffer()
{
    for (int i = 0; i < MAX_COMMAND_LENGHT; i++)
        command_buffer[i] = 0;
}

void clear_args()
{
    for (int j = 0; j < MAX_ARG_LENGHT; j++)
    {
        args[j] = 0;
    }
}