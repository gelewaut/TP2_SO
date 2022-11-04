GLOBAL sys_read  //0
GLOBAL sys_write //1
GLOBAL sys_malloc //2
GLOBAL sys_free   //3
    
GLOBAL sys_createProcess //4
GLOBAL sys_killProcess   //5
GLOBAL sys_getPID        //6
GLOBAL sys_modifyState   //7
GLOBAL sys_yield         //8
    
GLOBAL sys_semCreate    //9
GLOBAL sys_semOpen    //10
GLOBAL sys_semClose    //11
GLOBAL sys_semSignal    //12
GLOBAL sys_semWait    //13
    
GLOBAL sys_createPipe    //14
GLOBAL sys_openPipe      //15

section .text
    
;Para pasar argumentos se usan los registros RDI, RSI,
;RDX, R10, R8, R9

; %rbp
; %rsp
; %rbx
; %r12
; %r13
; %r15

%macro pushAsm 0
    push rbp
    push rsp
    push rbx
    push r12
    push r13
    push r15
%endmacro

%macro popAsm 0
    pop r15
    pop r13
    pop r12
    pop rbx
    pop rsp
    pop rbp
%endmacro

%macro handler_sysCall 1
    pushAsm
    mov rax, %1
    INT 80h
    popAsm
    ret
%endmacro

sys_read:
    handler_sysCall 0
sys_write:
    handler_sysCall 1
sys_malloc:
    handler_sysCall 2
sys_free:
    handler_sysCall 3
    
sys_createProcess:
    handler_sysCall 4
sys_killProcess:
    handler_sysCall 5
sys_getPID:
    handler_sysCall 6
sys_modifyState:
    handler_sysCall 7
sys_yield:
    handler_sysCall 8
    
sys_semCreate:
    handler_sysCall 9
sys_semOpen:
    handler_sysCall 10
sys_semClose:
    handler_sysCall 11
sys_semSignal:
    handler_sysCall 12
sys_semWait:
    handler_sysCall 13
    
sys_createPipe:
    handler_sysCall 14
sys_openPipe:
    handler_sysCall 15