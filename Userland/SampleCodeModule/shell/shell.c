#include <shell.h>
#include <sysCalls.h>
#include <userlibc.h>
#include <stdint.h>
#include <commands.h>
#include <tests.h>

static char shell_buffer[MAX_BUFFER + 1] = {0};
static int bufferIdx = 0;
static char command_buffer[MAX_COMMAND_LENGHT + 1] = {0};
static char * args[MAX_ARG_LENGHT + 1] = {0};
static uint8_t args_counter = 0;

static uint8_t pipe_check();
static void pipe_init(uint8_t pipe_idx);
static uint8_t pipe_run(uint8_t pipe_idx);
uint8_t check_command(const char * str);

static uint8_t pipe_check() {
    uint8_t i;
    for (i = 0; i < args_counter; i++) {
        if (string_compare(args[i], "|") == 0) {
            return i;
        }
    }
    return 0;
}

uint8_t check_command(const char * str) {
    for (int i = 0; i < NUMBER_OF_COMMANDS; i++) {
        if (string_compare(command_buffer, valid_commands[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

static void pipe_init(uint8_t pipe_idx) {
    if (pipe_idx == 0 || pipe_idx == args_counter - 1) {
        return;
    }

    char * args_left[MAX_ARG_LENGHT] = {0};
    char * args_right[MAX_ARG_LENGHT] = {0};
    uint8_t * argc_left = 0;
    uint8_t * argc_right = 0;

    for (int i = 0; i < pipe_idx; i++) {
        argc_left++;
        args_left[i] = args[i];
    }
    for (int i = pipe_idx + 1; i < args_counter; i++) {
        argc_right++;
        args_right[i] = args[i];
    }

    uint8_t left_cmd, right_cmd;
    left_cmd = isCommand(argc_left[0]);
    right_cmd = isCommand(args_right[0]);

    if (left_cmd < 0 || right_cmd < 0) {
        printf("One of the commands doesnt exist.");
        return;
    }

    // aca deberiamos crear el pipe
    // falta tambien checkear si es background o foreground

    Command command_left = command_functions[left_cmd]; 
    Command command_left = command_functions[right_cmd];

    // aca crear los procesos y llamarlos con sus respectivos argumentos y fd del pipie
    
    
}

typedef uint64_t (*Command) (char **, uint8_t);

static Command command_functions[NUMBER_OF_COMMANDS] = { 
    (Command)&helpCommand,  //0
    (Command)&phyloCommand, //1
    (Command)&printmemCommand,   //2
    (Command)&psCommand,        //3
    (Command)&loopCommand,   //4
    (Command)&killCommand,         //5
    (Command)&niceCommand,         //6   
    (Command)&blockCommand,         //7
    (Command)&semCommand,         //8
    (Command)&catCommand,         //9
    (Command)&wcCommand,         //10
    (Command)&pipeCommand,       //11
    (Command)&filterCommand    //12  
};

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
    uint8_t buffer_idx = 0;
    uint8_t aux_idx = 0;

    uint8_t token;

    while (shell_buffer[buffer_idx])
    {
        token = shell_buffer[buffer_idx++];
        if (token == ' ' && shell_buffer[buffer_idx] != ' ')
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
            args[args_counter][aux_idx++] = token;
        }
    }
}

uint8_t shell_execute()
{
    int8_t cmd = isCommand(), result = 1;
    if (cmd > 0)
    {
        result = commandDispatcher(cmd - 1);
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


uint64_t commandDispatcher(uint64_t cmd) {
    Command command = command_functions[cmd];
    if (command != 0) {
        return command(args, args_counter);
    }
    return 0;
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
    for (int i = 0; i < args_counter; i++)
    {
        for (int j = 0; j < MAX_ARG_LENGHT; j++)
        {
            args[i][j] = 0;
        }
    }
    args_counter = 0;
}