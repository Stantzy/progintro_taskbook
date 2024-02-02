%include "stud_io.inc"

global _start

section .text
_start:	xor	eax, eax
	xor	ecx, ecx
rd:	GETCHAR
	cmp	eax, -1
	je	prnt
	cmp	eax, 10
	je	prnt
	inc	ecx
	push	eax
	jmp	rd
prnt:	cmp	ecx, 0
	je	end
	pop	ebx
	PUTCHAR	bl
	loop	prnt
end:	PUTCHAR	10
	FINISH
