#include <shell.h>
#include <syscalls.h>
#include <userlibc.h>
#include <stdint.h>
#include <commands.h>
#include <tests.h>

static char shell_buffer[MAX_BUFFER + 1] = {0};
static int bufferIdx = 0;
static char args[MAX_ARG_LENGHT][MAX_ARG_LENGHT+1];
static uint8_t args_counter = 0;
static int PIPE_ID = 1;

static uint8_t pipe_check();
static int8_t pipe_init(uint8_t pipe_idx);
int check_command(const char * str);

static uint8_t pipe_check() {
    uint8_t i;
    for (i = 0; i < args_counter; i++) {
        if (string_compare(args[i], "|") == 0) {
            return i;
        }
    }
    return 0;
}

int check_command(const char * str) {
    for (int i = 0; i < NUMBER_OF_COMMANDS; i++) {
        if (string_compare(str, valid_commands[i]) == 0)
        {
            return i;
        }
    }
    return -1;
}

static int8_t pipe_init(uint8_t pipe_idx) {
    if (pipe_idx == 0 || pipe_idx == args_counter - 1) {
        return 1;
    }
    
    char * args_left[MAX_ARG_LENGHT] = {0};
    char * args_right[MAX_ARG_LENGHT] = {0};
    uint8_t argc_left = 0;
    uint8_t argc_right = 0;

    for (int i = 0; i < pipe_idx; i++) {
        argc_left++;
        args_left[i] = args[i];
    }
    for (int i = pipe_idx + 1; i < args_counter; i++) {
        argc_right++;
        args_right[i] = args[i];
    }

    int left_cmd, right_cmd;
    left_cmd = check_command(args_left[0]);
    right_cmd = check_command(args_right[0]);

    if (left_cmd < 0 || right_cmd < 0) {
        printf("One of the commands doesnt exist.");
        return 1;
    }
    
    //abro pipes
    int read = sys_createPipe(PIPE_ID, 0);
    int write = sys_openPipe(PIPE_ID++, 1);
    if (!read || !write) {
        printf("Pipe Couldnt be opened\n");
        return 1;
    }
    
    int ret_left, ret_right;
    ret_right = -1;
    int fd_left[2] = {0,write};
    int fd_right[2] = {read, 1};
    
    ret_left = commandDispatcher(left_cmd-1, argc_left, args_left, fd_left);
    if (ret_left)
        ret_right = commandDispatcher(right_cmd-1, argc_right, args_right, fd_right);
    
    return ret_right;
}

typedef uint64_t (*Command) (uint64_t, char **);

static Command command_functions[NUMBER_OF_COMMANDS] = { 
    //Built in
    (Command)&helpCommand,          //  0
    (Command)&pipeCommand,          //  1    
    (Command)&printmemCommand,      //  2
    (Command)&psCommand,            //  3
    (Command)&killCommand,          //  4
    (Command)&niceCommand,          //  5   
    (Command)&blockCommand,         //  6
    (Command)&semCommand,           //  7
    (Command)&test_mm,              //  8
    (Command)&test_prio,            //  9
    (Command)&test_processes,       //  10
    (Command)&test_sync,            //  11


    //No built in
    (Command)&catCommand,           //  12
    (Command)&wcCommand,            //  13
    (Command)&loopCommand,          //  14
    (Command)&phyloCommand,         //  15
    (Command)&filterCommand         //  16  
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

    if (shell_buffer[0] == 0) {
        return;
    }

    args[0][aux_idx++] = shell_buffer[buffer_idx++];

    while (shell_buffer[buffer_idx])
    {
        token = shell_buffer[buffer_idx];
        if (token != ' ') {
            if (shell_buffer[buffer_idx-1] == ' ') {
                args_counter++;
                aux_idx=0;
            }
            args[args_counter][aux_idx++] = token;
        }
        buffer_idx++;
    }
    args_counter++;
}

uint8_t shell_execute()
{
    int result = 1;
    if (args_counter <= 0) 
        return 1;

    int pipe_idx = pipe_check();

    if (pipe_idx > 0) {
        result = pipe_init(pipe_idx);
    } else {
        int8_t cmd = check_command(args[0]);
        if (cmd > 0)
        {
            int fd[2] = {0,1};
            result = commandDispatcher(cmd - 1, args_counter, args, fd);
        }
        else if (cmd == 0)
        {
            return 0;
        }
        else
        {
            printf("\nUnknown command type help for a list of commands");
        }
    }
    
    cleanup();
    return result;
}


uint64_t commandDispatcher(uint64_t cmd, int argc, char * argv[], int fd[2]) {
    Command command = command_functions[cmd];
    int foreground = 1;
    if (command != 0) {
        if (cmd <= 11) {
            return command(argc-1, argv+1);
        } else {
            if (string_compare(args[argc-1], "&") == 0){
                foreground = 0;
                argc--;
            }
            int ok = sys_createProcess((void *)command, argc, argv, fd, foreground);
            return ok;

        }
    }
    return 0;
}

void cleanup()
{
    clear_buffer();
    clear_args();
}

void clear_buffer()
{
    for (int i = 0; i <= bufferIdx; i++)
        shell_buffer[i] = 0;
    bufferIdx = 0;
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