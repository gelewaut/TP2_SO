#include <stdint.h>
#include <syscalls.h>
#include <phylosophers.h>
#include <commands.h>
#include <shell.h>
#include <userlibc.h>
#include <stddef.h>
#include <tests.h>

//ok
uint64_t helpCommand( uint64_t args_cant,char *args[])
{
    if(args_cant == 0){
        printf("\nThese are the available commands");
        for (int i = 0; i < NUMBER_OF_COMMANDS; i++)
        {
            putChar('\n');
            printf(valid_commands[i]);
        }
        putChar('\n');
    }else{
        printf("\nEl comando help no recibe argumentos.\n");
    }
    return EXIT_SUCCESS;
}

//ok
uint64_t printmemCommand( uint64_t args_cant,char *args[])
{
    if(args_cant == 0 ){
        sys_printMem();
    }else{
        printf("\n El comando mem no recibe argumentos.\n");
    }
    return EXIT_SUCCESS;
}

//muy malllllllllll
uint64_t phyloCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 0 ){
        run_phylosophers();
    }else{
        printf("\n El comando phylo no recibe argumentos.\n");
    }
    return EXIT_SUCCESS;
}

//ok
uint64_t psCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 0){
        sys_printProcesses();
    }else{
        printf("\n El comando ps no recibe argumentos.\n");
    }
    return EXIT_SUCCESS; 
}

//ok
uint64_t loopCommand( uint64_t args_cant,char *args[]){
    char c;
    if(args_cant == 0){
        int t = 0;
        long pid = sys_getPID();
        c = getChar();
        while(c != -1 && c != '\e'){
            t = sys_getTicks();
            if(t % 18 == 0 ) {
                printf("PID: %d\n", pid);
            }
            c = getCharContinues();
        }  
    }else{
        printf("\n El comando loop no recibe argumentos.\n");
    }
    return EXIT_SUCCESS; 
}

//ok
uint64_t killCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 1 ){
        uint64_t pid = strToNum(args[0]);
        if (pid != -1 )
        {
            sys_killProcess(pid);
        }else{
            printf("\n El pid del proceso debe ser un numero valido, para ver procesos activos correr el comando ps.\n");
        }
    }else{
        printf("\n El comando kill recibe el pid del proceso como argumento.\n");
    }
    return EXIT_SUCCESS; 
}

//ok
uint64_t niceCommand( uint64_t args_cant,char *args[]){
    
    if(args_cant == 3 ){
        uint64_t pid, priority,foreground;
        pid = strToNum(args[0]);
        priority = strToNum(args[1]);
        foreground = strToNum(args[2]);
        if (pid != -1 && priority != -1)
        {
            sys_changePriority(pid,priority,foreground);
        }else{
            printf("\n El pid y la nueva prioridad son valores numericos. \n");
        }
    }else{
        printf("\n El comando nice recibe el pid del proceso como primer argumento, la nueva priodidad como segundo argumento y un booleano para indicar foreground.");
    }
    return EXIT_SUCCESS;
}

//ok
uint64_t blockCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 1 ){
        uint64_t pid = strToNum(args[0]);
        if (pid != -1 )
        {
            sys_blockProcess(pid);
        }else{
            printf("\n El pid del proceso debe ser un numero valido, para ver procesos activos correr el comando ps.");
        }
    } else {
        printf("\n El comando block recibe solamente el pid del proceso como argumento.");
    }
    return EXIT_SUCCESS;
}

//ok
uint64_t unblockCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 1 ){
        uint64_t pid = strToNum(args[0]);
        if (pid != -1 )
        {
            sys_unblockProcess(pid);
        }else{
            printf("\n El pid del proceso debe ser un numero valido, para ver procesos activos correr el comando ps.");
        }
    } else {
        printf("\n El comando block recibe solamente el pid del proceso como argumento.");
    }
    return EXIT_SUCCESS;
}

//ok
uint64_t semCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 0){
        sys_printSemaphores();
    }else{
        printf("\n El comando sem no recibe argumentos.\n");
    }
    return EXIT_SUCCESS; 
}

//maso, no funciona sin pipe
uint64_t catCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 0){
        char c = 0, bufferIdx = 0;
        while ((c = getChar()) != ESC && c != -1){
            if(c == BACKSPACE) {
                if (bufferIdx > 0){
                    putChar(c);
                    bufferIdx--;
                }
            } else if (c > 0) {
                putChar(c);
                bufferIdx++;
            }
        }        
    }else{
        printf("\n El comando cat no recibe argumentos.\n");
    }
    return EXIT_SUCCESS;  
}

//maso, no funciona sin pipe
uint64_t wcCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 0){
        int count = 1, bufferIdx = 0, totalCount = 0;
        char c = 0;
        // printf("\nPresione ESC para volver a la shell\n");
        while ((c = getChar()) != ESC && c != -1)
        {
            if (c == BACKSPACE){
                if (bufferIdx > 0){
                    // putChar(c);
                    bufferIdx--;
                }
            }
            else if (c > 0){
                // putChar(c);
                bufferIdx++;
                if (c == '\n'){
                    count++;
                    totalCount += bufferIdx;
                    bufferIdx = 0;
                }
            }
        }
        char buf[10], wcbuff[10];
        numToStr(count, buf, 10);
        numToStr(totalCount, wcbuff, 10);
        printf("\nHay %s palabras y %s lineas \n", wcbuff ,buf); 
    }else{
        printf("\n El comando wc no recibe argumentos.\n");
    }
    return EXIT_SUCCESS;     
}


uint64_t filterCommand( uint64_t args_cant,char *args[]){

    if(args_cant == 0){
        char c = 0, bufferIdx = 0;
        while ((c = getChar()) != ESC && c != -1){
            if(c == BACKSPACE) {
                if (bufferIdx > 0){
                    putChar(c);
                    bufferIdx--;
                }
            } else if (c > 0 && !ISVOWEL(c) ) {
                putChar(c);
                bufferIdx++;
            }
        }        
    }else{
        printf("\n El comando filter no recibe argumentos.\n");    }
    return EXIT_SUCCESS;
}

//ok
uint64_t pipeCommand( uint64_t args_cant,char *args[]){
    if(args_cant == 0){
        sys_printPipes();   
        return EXIT_SUCCESS;  
    }else{
        printf("\n El comando pipe no recibe argumentos.\n");
    }

    return EXIT_SUCCESS;    
}


//ok
uint64_t test_mm_command(uint64_t argc, char ** argv){
    test_mm(argc, argv);
    return 1;
}

//ok
uint64_t test_prio_command(uint64_t argc, char ** argv){
    test_prio();
    return 1; 
}

//ok
uint64_t test_pcs_command(uint64_t argc, char ** argv){
    return test_processes(argc, argv); 
}


uint64_t test_sync_command(uint64_t argc, char ** argv){
    printf("\nIn test_sync_command...\n");
    return test_sync(argc, argv);
}