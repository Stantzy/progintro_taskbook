%include "stud_io.inc"
global _start

section .bss
arrdgt resb 10			 ; max numerical rank in 32 bit is 10

section .text
_start:	xor 	ecx, ecx
	xor 	esi, esi
	mov 	esi, arrdgt

rd:	xor 	eax, eax
	GETCHAR
	cmp 	eax, -1
	je 	calculate
	inc 	ecx
	jmp 	rd

calculate:
	mov	eax, ecx
	mov	ebx, 10
lp:	xor 	edx, edx
	div 	ebx
	mov 	byte [esi], dl
	cmp 	eax, 0
	je 	prnt
	inc	esi
	jmp 	lp

prnt:	xor 	al, al
	mov 	al, [esi]
	add 	al, byte 48
	PUTCHAR	al
	dec 	esi
	cmp 	esi, arrdgt
	jnl 	prnt
	PUTCHAR	10
	FINISH
