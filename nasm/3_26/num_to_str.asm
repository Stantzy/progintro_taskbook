global	num_to_str

section	.data
multiplier	dw 10

section .text
; num_to_str([ebp+12]=number address, [ebp+8]=destination address of string)
num_to_str:
	push	ebp
	mov	ebp, esp
	push	edi
	
	xor	eax, eax
	xor	edx, edx	
	mov	edi, [ebp+8]
	mov	edx, [ebp+12]	; copy address to edx
	mov	eax, [edx]	; copy value from address to eax
	xor	edx, edx

.loop:	div	dword [multiplier]
	add	dl, 48		; to get char of digit
	mov	[edi], dl
	xor	edx, edx
	cmp	eax, 0
	je	.exit
	inc	edi
	jmp	.loop

.exit:	inc	edi
	mov	[edi], byte 0	; flag of the end of a string

	pop	edi
	mov	esp, ebp
	pop	ebp
	ret	8
