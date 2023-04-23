	;; @file state.asm
	;; @brief My own implementations of setjmp and longjmp.
	;; @author Erick Carrillo.
	;; @copyright Copyright (C) 2023, Erick Alejandro Carrillo López, All right reserved.b
	;; @license This project is released under the MIT License

	section	.data

	section .bss
	
	section .text
	global setjmp


	;; To get the rip register
get_rip:
	pop rax
	ret

	;; int setjmp(JmpBuf *buf)
	;; buf -> is the address for the buffer
	;; Initialize a state jump point for a future jumping.
setjmp:
	;; this function is not a normal c function 
	mov rax, qword [rsp]	; catch the address of the buffer

	;; Capture the process state
	mov qword [rax], rbx		; buf[0] = rbx
	mov qword [rax + 8], rsi	; buf[1] = rsi
	mov qword [rax + 16], rdi	; buf[2] = rdi
	mov qword [rax + 24], rbp	; buf[3] = rbp

	;; catch the previos value of rsp
	lea rcx, qword [rsp + 8] ; address of the previos stack pointer

	mov qword [rax + 32], rcx 	; buf[4] = rcx

	;; Catch the rip instruction pointer
	push rax		; Copy the value of rax
	call get_rip
	mov rcx, rax		; Catcht he rip register
	pop rax
	
	mov qword [rax + 40], rcx 	; buf[5] = rip the instruction pointer
	
	xor rax, rax		; rax = 0
	ret
	
