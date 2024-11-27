%include "stud_io.inc"
 
global	_start

section	.text
_start:
	xor	eax, eax
	xor	ecx, ecx
	xor	edx, edx
	mov	esi, esp	; save the initial value of ESP
	mov	edi, esp	; same

rd_str:
	GETCHAR
	push	eax
	cmp	eax, -1		; EOF
	je	chk_str		; print strings
	inc	ecx		; counting chars
	jmp	rd_str

next_str:
	sub	edi, 4

chk_str:
	cmp	ecx, 0		; if input is empty
	je	end		; then just end the program
lp_chk:	mov	edx, [edi]	; read a char from stack
	cmp	edx, 10		; if EOL in stack
	je	prnt		; print the current string
	cmp	edx, -1		; if EOF
	je	prnt		; print str
	sub	edi, 4		; -4 bytes = next char in stack
	jmp	lp_chk

prnt:
	xor	ebx, ebx
	mov	ebx, edi	; save current position in ebx
lp_prnt:
	cmp	[ebx], dword -1 ; EOF in stack
	je	end
	PUTCHAR	[ebx]
	add	ebx, 4		; prev char in stack
	cmp	ebx, esi	; if the start of stack in ebx
	je	next_str	; check next string
	cmp	dword [ebx], 10 ; or EOL
	je	next_str
	cmp	ebx, esp	; if the end of stack
	je	end		; end program
	jmp	lp_prnt

end:	PUTCHAR	10
	FINISH
