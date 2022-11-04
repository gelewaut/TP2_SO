GLOBAL sys_read
GLOBAL sys_write
GLOBAL sys_malloc
GLOBAL sys_free
    
GLOBAL sys_createProcess
GLOBAL sys_killProcess
GLOBAL sys_getPID
GLOBAL sys_modifyState
GLOBAL sys_yield
    
GLOBAL sys_semCreate
GLOBAL sys_semOpen
GLOBAL sys_semClose
GLOBAL sys_semSignal
GLOBAL sys_semWait
    
GLOBAL sys_createPipe
GLOBAL sys_openPipe

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