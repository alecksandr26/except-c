	;; @file stackjmp.asm
	;; @brief My own implementations of stackjmp and jmpback.
	;; @author Erick Carrillo.
	;; @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All right reserved.b
	;; @license This project is released under the MIT License

	section	.data

	section .bss
	
	section .text
	global stackjmp
	global jmpback

	;; int stackjmp(JmpBuf *buf)
	;; rdi -> buf -> is the address for the buffer
	;; Initialize a state jump point for a future jumping.
stackjmp:
	;; this function is not a normal c function 
	mov rax, rdi	; catch the address of the buffer

	;; Capture the process state
	mov qword [rax], rbx		; buf[0] = rbx
	mov qword [rax + 8], rsi	; buf[1] = rsi
	mov qword [rax + 16], rdi	; buf[2] = rdi
	mov qword [rax + 24], rbp	; buf[3] = rbp

	;; catch the previos value of rsp
	lea rcx, [rsp + 8] ; address of the previos stack pointer

	mov qword [rax + 32], rcx 	; buf[4] = rcx

	;; Get the rip instruction pointer
	mov rcx, qword [rsp]
	mov qword [rax + 40], rcx 	; buf[5] = rip the instruction pointer
	
	xor rax, rax		; rax = 0
	ret


	;; void jmpback(JmpBuf *buf, int val)
	;; rdi -> buf -> it is the address for the initialized buffer
	;; rsi -> val -> It is the value to return
jmpback:
	mov rdx, rdi
	mov rax, rsi

	cmp rax, 0		; is int val == 0
	jne jmpback_continue
	inc rax			; Increment 1 
	
jmpback_continue:
	mov rbx, qword [rdx]		; rbx = buf[0] 
	mov rsi, qword [rdx + 8]	; rsi = buf[1] 
	mov rdi, qword [rdx + 16]	; rdi = buf[2]
	mov rbp, qword [rdx + 24]	; rbp = buf[3]
	mov rsp, qword [rdx + 32]	; rsp = buf[4]
	jmp qword [rdx + 40]		; Jump to the set jump
	
