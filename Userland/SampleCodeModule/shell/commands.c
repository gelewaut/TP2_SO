#include <stdint.h>
#include <syscalls.h>
#include <phylosophers.h>
#include <commands.h>
#include <shell.h>
#include <userlibc.h>
#include <stddef.h>

uint64_t helpCommand(char *args[], uint64_t args_cant)
{
    if(args_cant == 0){
        printf("\nThese are the available commands");
        for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            putChar('\n');
            printf(valid_commands[i]);
        }
        putChar('\n');
        return EXIT_SUCCESS;
    }

    printf("\nEl comando help no recibe argumentos.\n");
    return EXIT_FAILURE;
}

uint64_t printmemCommand(char *args[], uint64_t args_cant)
{
    if(args_cant == 1 ){
        uint64_t auxArg = strToNum(args[0]);
        if (auxArg != -1 )
        {
            sys_printMem();
            return EXIT_SUCCESS;
        }
    }
    
    printf("\n El comando mem recibe un numero como argumento.\n");
    return EXIT_FAILURE;
}

uint64_t phyloCommand(char *args[], uint64_t args_cant){
    if(args_cant == 1 ){
        uint64_t cant = strToNum(args[0]);
        if (cant != -1 )
        {
            run_phylosophers(cant);
            return EXIT_SUCCESS;
        }
    }
    
    printf("\n El comando phylo recibe la cantidad de filosofos como argumento.\n");
    return EXIT_FAILURE;
}

uint64_t psCommand(char *args[], uint64_t args_cant){
    if(args_cant == 0){
        sys_printProcesses();
        return EXIT_SUCCESS;
    }

    printf("\n El comando ps no recibe argumentos.\n");
    return EXIT_FAILURE; 
}

uint64_t loopCommand(char *args[], uint64_t args_cant){
    if(args_cant == 0){
        // uint64_t pid = sys_getPID();
        return EXIT_SUCCESS;  
    }

    printf("\n El comando loop no recibe argumentos.\n");
    return EXIT_FAILURE; 
}

uint64_t killCommand(char *args[], uint64_t args_cant){
    if(args_cant == 1 ){
        uint64_t pid = strToNum(args[0]);
        if (pid != -1 )
        {
            sys_killProcess(pid);
            return EXIT_SUCCESS;
        }
    }
    
    printf("\n El comando kill recibe el pid del proceso como argumento.");
    return EXIT_FAILURE;
}

uint64_t niceCommand(char *args[], uint64_t args_cant){
    
    if(args_cant == 3 ){
        uint64_t pid, priority,foreground;
        pid = strToNum(args[0]);
        priority = strToNum(args[1]);
        foreground = strToNum(args[2]);
        if (pid != -1 && priority != -1)
        {
            sys_changePriority(pid,priority,foreground);
            return EXIT_SUCCESS;
        }
    }
    
    printf("\n El comando nice recibe el pid del proceso como primer argumento, la nueva priodidad como segundo argumento y un booleano para indicar foreground.");
    return EXIT_FAILURE;
}

uint64_t blockCommand(char *args[], uint64_t args_cant){
    if(args_cant == 1 ){
        uint64_t pid = strToNum(args[0]);
        if (pid != -1 )
        {
            sys_blockProcess(pid);
            return EXIT_SUCCESS;
        }
    }
    
    printf("\n El comando block recibe el pid del proceso como argumento.");
    return EXIT_FAILURE;
}

uint64_t semCommand(char *args[], uint64_t args_cant){
    if(args_cant == 0){
        sys_printSemaphores();
        return EXIT_SUCCESS;     
    }
    
    printf("\n El comando sem no recibe argumentos.\n");
    return EXIT_FAILURE; 
}

uint64_t catCommand(char *args[], uint64_t args_cant){
    if(args_cant == 0){
        char c = 0;
        while((c = getChar()) != '\n'){
            putChar(c);
            c = 0;
        }    

        return EXIT_SUCCESS;    
    }

    printf("\n El comando cat no recibe argumentos.\n");
    return EXIT_FAILURE;  
}

uint64_t wcCommand(char *args[], uint64_t args_cant){
    if(args_cant > 0){
        int count = 1;
        char c = 0;

        printf("Linea: %d \n", count);
        while (1)
        {
            c = getChar();
            putChar(c);
            if (c == '\n')
            {
                count++;
                printf("Linea: %d \n", count);
            }
            c = 0;
        }

        return EXIT_SUCCESS; 
    }

    printf("\n El comando wc no recibe argumentos.\n");
    return EXIT_FAILURE;     
}

uint64_t filterCommand(char *args[], uint64_t args_cant){
    if(args_cant == 0){
        char c = 0;
        while(1){
            c = getChar();
            if(!ISVOWEL(c) ){
                putChar(c);
            }
            c = 0;
        }    
        return EXIT_SUCCESS; 
    }

    printf("\n El comando filter no recibe argumentos.\n");
    return EXIT_FAILURE;     
}

uint64_t pipeCommand(char *args[], uint64_t args_cant){
    if(args_cant == 0){
        sys_printPipes();   
        return EXIT_SUCCESS;  
    }

    printf("\n El comando pipe no recibe argumentos.\n");
    return EXIT_FAILURE;    
}