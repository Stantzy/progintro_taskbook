%include "stud_io.inc"

global	read

section	.text

; read([ebp+12]=destination address, [ebp+8]=allocated memory size)
; EAX=code of result, ECX=number of digits
read:
	push	ebp
	mov	ebp, esp
	push	esi
	push	edi
	mov	esi, [ebp+8]
	mov	edi, [ebp+12]
	xor	ecx, ecx

.loop:	GETCHAR			; read symbol
	cmp	al, -1		; if EOF
	je	.exit		; end func
	cmp	al, 10		; if end of line
	je	.exit		; end func
	cmp	al, 48
	jl	.input_error
	cmp	al, 57
	jg	.input_error
	inc	ecx		; +1 readed symbol
	mov	[edi], al	; copy readed digit into memory
	inc	edi		; next address in memory
	dec	esi		; loop counter - 1
	cmp	esi, dword 0
	jg	.loop		; read next symbol
	jmp	.size_error

.exit:	
	mov	eax, 0		; no error
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8

.size_error:
	mov	eax, 1		; number of digits > allocated memory size
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8

.input_error:
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8		; in EAX returns code of error symbol

