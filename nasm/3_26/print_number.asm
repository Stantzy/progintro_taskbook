%include "stud_io.inc"

global	print_number

section	.text

; print_number([ebp+8]=source address) 
print_number:
	push	ebp
	mov	ebp, esp

	xor	edx, edx
	xor	ecx, ecx
	mov	eax, [ebp+8]
	mov	ecx, eax	; save starting address in ECX

.loop:	cmp	[eax], byte 0	; 0 is the end of an array of digits
	je	.reverse_print
	inc	eax
	jmp	.loop

.reverse_print:
	mov	dl, [eax]
	PUTCHAR	dl
	dec	eax
	cmp	eax, ecx
	jge	.reverse_print
	PUTCHAR	10
	
	mov	esp, ebp
	pop	ebp
	ret	4
