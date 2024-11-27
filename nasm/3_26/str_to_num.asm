global	str_to_num

section	.data
multiplier	dw 10


section	.text

; str_to_num([ebp+8]=string address, [ebp+12]=length of string)
; EAX=result number, CL=error flag
str_to_num:
	push	ebp
	mov	ebp, esp
	xor	eax, eax
	xor	ecx, ecx
	push	esi
	push	edi
	push	ebx
	mov	esi, [ebp+8]
	mov	edi, [ebp+12]

.loop:	xor	edx, edx
	mov	dl, [esi]	; read digit from string
	cmp	dl, 48
	jl	.err
	cmp	dl, 57
	jg	.err
	sub	dl, 48		; edx-48=number(not char)
	xor	ebx, ebx
	mov	ebx, edx
	mov	eax, ecx
	mul	dword [multiplier]
	mov	ecx, eax
	add	ecx, ebx
	inc	esi	; next digit in string
	
	dec	edi		; loop counter-1
	cmp	edi, dword 0 ; if lc > 0
	jg	.loop		; then read next digit
	
.exit:	mov	eax, ecx	; result in eax to return
	mov	ecx, 0		; no error
	pop	ebx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8

.err:	mov	eax, -1		; if error return -1 in EAX
	mov	ecx, 1		; error flag is true
	pop	ebx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8
