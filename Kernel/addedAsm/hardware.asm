GLOBAL getRTC
GLOBAL getKey
section .text

getKey:
	push rbp
	mov rbp, rsp	
	mov rax,0
loop:    
	in al,0x64       
    mov cl,al
    and al,0x01       
    cmp al,0
    je loop
    in al,0x60
       
    mov rsp, rbp
    pop rbp
    ret


getRTC:
	push rbp
	mov rbp, rsp

	mov rax, 0
	mov rax, rdi
	out 70h, al
	mov rax, 0
	in al, 71h

	mov rsp, rbp
	pop rbp
	ret