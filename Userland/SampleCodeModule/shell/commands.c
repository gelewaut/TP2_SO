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
    if(args_cant == 0 ){
        sys_printMem();
        return EXIT_SUCCESS;
    }
    
    printf("\n El comando mem no recibe argumentos.\n");
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
    // if(args_cant == 0){
    //     int t = 0;
    //     long pid = my_getpid();

    //     while(1){
    //         t = ticks();
    //         if(t % 18 == 0 ) {
    //             printf("PID: %d\n", pid);
    //         }  
    //     }
            
    //     return EXIT_SUCCESS;  
    // }

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
    if(args_cant == 0){
        int count = 1, bufferIdx = 0;
        char c = 0;
        printf("\nPresione ESC para volver a la shell\n");
        while ((c = getChar()) != 27)
        {
            if (c == BACKSPACE){
                if (bufferIdx > 0){
                    putChar(c);
                    bufferIdx--;
                }
            }
            else {
                putChar(c);
                bufferIdx++;
                if (c == '\n'){
                    count++;
                    bufferIdx = 0;
                }
            }
        }
        char buf[10];
        numToStr(count, buf, 10);
        printf("\nHay %s lineas \n", buf);

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

//comandos para los tests

// void testMM(int argc, char ** argv){
//     if(argc != 2){
//         printf("Cantidad de parametros invalida. Metodo de uso: test_mm <MAX_MEMORY>\n");
//         return;
//     }
//     test_mm(1, &argv[1]);
// }

// void testPrio(int argc, char ** argv){
//     if(argc != 1){
//         printf("Cantidad de parametros invalida. Metodo de uso: test_prio\n");
//         return;
//     }
//     test_prio();
// }

// void testPCS(int argc, char ** argv){
//     if(argc != 2){
//         printf("Cantidad de parametros invalida. Metodo de uso: test_processes <MAX_PROCESSES>\n");
//         return;
//     }
//     test_processes(1, &argv[1]); 
// }
// void testSync(int argc, char ** argv){
//     if(argc != 3){
//         printf("Cantidad de parametros invalida. Metodo de uso: test_sync <n> <use_sem>\n");
//         return;
//     }
//     char * argAux [2]={argv[1], argv[2]};
//     test_sync(2, argAux);
// }