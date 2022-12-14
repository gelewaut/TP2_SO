GLOBAL cpuVendor
GLOBAL sys_xchg
GLOBAL call_timerTick

section .text
	
cpuVendor:
	push rbp
	mov rbp, rsp

	push rbx

	mov rax, 0
	cpuid


	mov [rdi], ebx
	mov [rdi + 4], edx
	mov [rdi + 8], ecx

	mov byte [rdi+13], 0

	mov rax, rdi

	pop rbx

	mov rsp, rbp
	pop rbp
	ret

sys_xchg:
	push rbp
	mov rbp, rsp

	mov rax, rsi
	xchg [rdi], eax

	mov rsp, rbp
	pop rbp
	ret

call_timerTick:
	int 20h
	ret