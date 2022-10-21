GLOBAL saveRegs
GLOBAL nextReg

EXTERN saveIp
EXTERN saveRsp
EXTERN saveReg

saveRegs:
	; push rbp
	; mov rbp, rsp

	mov rdi, [rsp+120]
    call saveIp

    lea rdi, [rsp+120]
    call saveRsp

    mov rbx, 0
    mov rcx, rsp
    add rcx, 8
	call nextReg

	; mov rsp, rbp
	; pop rbp

	ret

nextReg:
    push rcx
    mov rdi, rbx
    ; call printRegName
    ; pop rcx
    ; push rcx
    mov rsi, [rcx]
    call saveReg
    pop rcx
    add rcx, 8
    inc rbx
    cmp rbx, 15
    jne nextReg

	ret