global lenstr

; lenstr([EBP+8]=str address)
; counts length of string and uses only EAX and ECX
; returns length of string in EAX
lenstr:
	push	ebp
	mov	ebp, esp

	xor	eax, eax
	mov	ecx, [ebp+8]

.count:
	cmp	[ecx], byte 0 	; 0 is end of string
	je	.quit		; if 0 then quit func
	inc	eax		; +1 char in string
	inc	ecx		; read next char in str
	jmp	.count

.quit:
	mov	esp, ebp
	pop	ebp
	ret
