global cmplstsymb

; cmplstsymb([EBP+8]=str1 address, [EBP+12]=str2 address)
; compares last symbols of two strings
; returns 1 in EAX if symbols are identical else 0
cmplstsymb:
	push	ebp
	mov	ebp, esp
	sub	esp, 4		; for one parameter [EBP-4]

	xor	eax, eax
	xor	ecx, ecx
	mov	eax, [ebp+8]	; read first string
.loop1:
	cmp	[eax], dword 0		; 0 is end of current string
	je	.savesymbol1		; if 0 then save last symbol of string
	inc	eax
	jmp	.loop1

.savesymbol1:
	dec	eax		; to read last symbol before byte 0
	mov	ecx, dword [eax]
	mov	[ebp-4], ecx

	xor	eax, eax
	xor	ecx, ecx
	mov	eax, [ebp+12]	; read second string

.loop2:
	cmp	[eax], dword 0
	je	.savesymbol2
	inc	eax
	jmp	.loop2

.savesymbol2:
	dec	eax
	mov	ecx, [eax]

.compare:
	cmp	[ebp-4], ecx
	je	.setflag

.quit:
	mov	esp, ebp
	pop	ebp
	ret

.setflag:
	xor	eax, eax
	mov	eax, 1
	jmp	.quit
