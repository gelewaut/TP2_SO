#include <shell.h>
#include <syscalls.h>
#include <userlibc.h>
#include <stdint.h>
#include <commands.h>
#include <tests.h>

static char shell_buffer[MAX_BUFFER + 1] = {0};
static int bufferIdx = 0;
static char * argv[MAX_ARGS] = {0};
static int argc = 0;
static int PIPE_ID = 1;

typedef uint64_t (*Command) (uint64_t, char **);

static Command command_functions[NUMBER_OF_COMMANDS] = { 
    //Built in
    (Command)&helpCommand,          //  0
    (Command)&pipeCommand,          //  1    
    (Command)&printmemCommand,      //  2
    (Command)&psCommand,            //  3
    (Command)&semCommand,           //  4

    
    //No built in
    (Command)&killCommand,          //  5
    (Command)&niceCommand,          //  6   
    (Command)&blockCommand,         //  7
    (Command)&unblockCommand,       //  8
    (Command)&test_mm_command,      //  9
    (Command)&test_prio_command,    //  10
    (Command)&test_pcs_command,     //  11
    (Command)&test_sync_command,    //  12
    (Command)&catCommand,           //  13
    (Command)&wcCommand,            //  14
    (Command)&loopCommand,          //  15
    (Command)&phyloCommand,         //  16
    (Command)&filterCommand         //  17  
};

void printArgs(int argc, char ** argv) {
    
    for (int i=0; i<argc; i++) {
        // for (int j = 0; j<400000000; j++);
        printf(argv[i]);
    }
    return;
}
void readArgs(int argc, char ** argv) {

    char c;
    putChar('\n');
    while (1) {
        c = getChar();
        if (c == -1) {
            printf("\nPipes Work\n");
            return;
        }
        putChar(c);
    }
}

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

void shell_loop() {
    while(1) { 
        putChar('\n');
        printf(PROMPT);
        my_readline();
        my_parseline();
        int pipe_idx = check_pipe();
        if (pipe_idx > 1 && pipe_idx < argc) {
            init_pipe(pipe_idx);
        } else {
            int ok = execute_command(argc, argv, 0, 1, 0);
            if (ok < 0) {
                printf("\nCould not execute command\n");
            }
            // int fd[2] = {0,1};
            // int pid = sys_createProcess(&printArgs, argc, argv, fd, 1);
            // sys_wait(pid);
        }
        // for(int i=0; i<argc; i++) {
        //     putChar('\n');
        //     printf(argv[i]);
        // }
        cleanup();
    }
}

void my_readline() {
    
    int8_t c;
    while ((c = getCharContinues()) != ENTER)
    {
        if (c == BACKSPACE)
        {
            if (bufferIdx > 0)
            {
                shell_buffer[--bufferIdx] = 0;
                putChar(c);
            }
        }
        else if (c > 0)
        {
            if (bufferIdx < MAX_BUFFER) {
                shell_buffer[bufferIdx++] = c;
                putChar(c);
            }
        }
    }
}

void my_parseline() {
    uint8_t buffer_idx = 0;
    uint8_t token = shell_buffer[buffer_idx];

    if (token != '\0' && token != ' ') {
        argv[argc++] = &(shell_buffer[buffer_idx]);
    }


    while(shell_buffer[buffer_idx] != '\0' && argc < MAX_ARGS) {

        token = shell_buffer[buffer_idx];
        if (token == ' ') {
            shell_buffer[buffer_idx] = 0;
            if (shell_buffer[buffer_idx+1] != 0 && shell_buffer[buffer_idx+1] != ' ') {
                argv[argc++] = &(shell_buffer[buffer_idx+1]);
            }
        }

        buffer_idx++;
    }
}

int check_pipe() {
    for (int i=0; i<argc; i++) {
        if (string_compare(argv[i], "|") == 0) {
            return i+1;
        }
    }
    return 0;
}

void init_pipe(int pipe_idx) {
    
    char * argv_left[MAX_ARG_LENGHT] = {0};
    char * argv_right[MAX_ARG_LENGHT] = {0};
    uint8_t argc_left = 0;
    uint8_t argc_right = 0;

    for (int i = 0; i < pipe_idx-1; i++) {
        argc_left++;
        argv_left[i] = argv[i];
    }
    for (int i = pipe_idx, j=0; i < argc; i++, j++) {
        argc_right++;
        argv_right[j] = argv[i];
    }

    //abro pipes
    int read = sys_createPipe(PIPE_ID, 0);
    int write = sys_openPipe(PIPE_ID++, 1);
    if (!read || !write) {
        printf("Pipe Couldnt be opened\n");
        return;
    }

    int pid_left = execute_command(argc_left, argv_left, 0, write, 1);
    if (pid_left <= 0) {
        printf("\n1st Command not valid\n");
        return;
    }
    int pid_right = execute_command(argc_right, argv_right, read, 1, 1);
    if (pid_right <= 0) {
        printf("\n2nd Command not valid\n");
        sys_killProcess(pid_left);
        return;
    }
    sys_wait(pid_left);
    sys_wait(pid_right);
}

int check_command(const char * str) {
    for (int i=0; i<NUMBER_OF_COMMANDS; i++) {
        if (string_compare(str, valid_commands[i]) == 0) {
            return i;
        }
    }
    return -1;
}

int execute_command(int argc_, char ** argv_, int fd_in, int fd_out, int pipe) {
    if (argc <= 0) {
        return 0;       
    }
    int foreground = 1;
    if (string_compare(argv_[argc_-1], "&") == 0) {
        foreground = 0;
        argc_--;
    }
    int cmd = check_command(argv_[0]);
    int fd[2] = {fd_in, fd_out};

    if (cmd > 0 ) {
        Command command = command_functions[cmd-1];
        if (cmd <= 4 && !pipe) {
            return command(argc_-1, &(argv[1]));
        } else {
            int childPid = sys_createProcess((void *)command, argc_, argv_, fd, foreground);
            if (foreground && !pipe) {
                sys_wait(childPid);
            }
            return childPid;
        }
    }
    return -1;
}

void cleanup() {
    for (int i = 0; i <= bufferIdx; i++) {
        shell_buffer[i] = 0;
    }
    bufferIdx = 0;
    for (int i= 0; i < MAX_ARGS; i++) {
        argv[i] = 0;
    }
    argc = 0;
}