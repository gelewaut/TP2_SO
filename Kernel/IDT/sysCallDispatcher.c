#include <sysCalls.h>
#include <naiveConsole.h>

typedef uint64_t (*SysCall) (uint64_t, uint64_t, uint64_t, uint64_t, uint64_t);

// static SysCall sysCalls[10] = { (SysCall)&sys_read, (SysCall)&sys_write, (SysCall)&getRTC};
static SysCall sysCalls[25] = { 
    (SysCall)&sys_read,  //0
    (SysCall)&sys_write, //1
    (SysCall)&sys_malloc, //2
    (SysCall)&sys_free,   //3
    
    (SysCall)&sys_createProcess, //4
    (SysCall)&sys_killProcess,   //5
    (SysCall)&sys_getPID,        //6
    (SysCall)&sys_blockProcess,   //7
    (SysCall)&sys_changePriority,   //8
    (SysCall)&sys_yield,         //9
    
    (SysCall)&sys_semCreate,    //10
    (SysCall)&sys_semOpen,    //11
    (SysCall)&sys_semClose,    //12
    (SysCall)&sys_semSignal,    //13
    (SysCall)&sys_semWait,    //14
    
    (SysCall)&sys_createPipe,    //15
    (SysCall)&sys_openPipe,      //16

    (SysCall)&sys_printMem,      //17
    (SysCall)&sys_printProcesses,  //18
    (SysCall)&sys_printSemaphores, //19
    (SysCall)&sys_printPipes,     //20
    
    (SysCall)&ncClear,            //21
    (SysCall)&sys_getTicks,        // 22  

    (SysCall)&sys_wait        // 22  
    };

uint64_t sysCallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx, uint64_t r8, uint64_t r9_rax) {
    SysCall sysCall = sysCalls[r9_rax];
    if (sysCall != 0) {
        return sysCall(rdi, rsi, rdx, rcx, r8);
    }
    return 0;
}