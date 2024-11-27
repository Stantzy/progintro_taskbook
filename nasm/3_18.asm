%include "stud_io.inc"

global _start

section .text
_start:	xor	ebx, ebx
	xor	esi, esi
	mov	esi, 10
rd:	xor	eax, eax
	xor 	ecx, ecx
	GETCHAR
	cmp	eax, -1
	je	prnt
	sub	eax, 48
	cmp	eax, 0
	jl	prnt
	cmp	eax, 9
	jg	prnt
	mov	ecx, eax
	mov	eax, ebx
	mul	esi
	mov	ebx, eax
	add	ebx, ecx
	jmp	rd
prnt:	xor	ecx, ecx
	mov	ecx, ebx
lp:	PRINT "*"
	loop	lp
	PUTCHAR	10
	FINISH
