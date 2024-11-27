global _start

section	.text

nlstr		db	10, 0

_start:	cmp	dword [esp], 1
	jz	quit			; no argv

	mov	ebx, dword [esp]
	dec	ebx
	mov	esi, esp
	add	esi, 8

lp:	push	dword [esi]
	call	checkA
	add	esp, 4
	cmp	eax, 1
	jz	callprint

	push	dword [esi]
	call	checkB
	add	esp, 4
	cmp	eax, 1
	jz	callprint

	push	dword [esi]
	call	checkC
	add	esp, 4
	cmp	eax, 1
	jz	callprint

	push	dword [esi]
	call	checkD
	add	esp, 4
	cmp	eax, 1
	jz	callprint

	add	esi, 4
	dec	ebx
	jnz	lp

quit:	mov	eax, 1
	mov	ebx, 1
	int	80h

callprint:
	push	ebx
	push	dword [esi]
	call	printstr
	add	esp, 4
	pop	ebx
	push	ebx
	push	dword nlstr
	call	printstr
	add	esp, 4
	pop	ebx
	add	esi, 4
	dec	ebx
	cmp	ebx, 0
	ja	lp
	jmp	quit


; strlen([EBP+8] = str address)
; counts the length of the string
; returns the length in EAX
strlen:
	push	ebp
	mov	ebp, esp

	xor	eax, eax
	mov	ecx, [ebp+8]
.lp:	cmp	byte [ecx+eax], 0	; the end of the str
	je	.quit
	inc	eax
	jmp	.lp

.quit:	mov	esp, ebp
	pop	ebp
	ret

; printstr([EBP+8] = str address)
printstr:
	push	ebp
	mov	ebp, esp

	push	dword [ebp+8]
	call	strlen
	add	esp, 4			; the length is now in eax

	mov	edx, eax		; length
	mov	ecx, dword [ebp+8]	; str address
	mov	ebx, 1			; stdout
	mov	eax, 4			; sys write
	int	80h

	mov	esp, ebp
	pop	ebp
	ret

; checkA([EBP+8] = str address)
; checks if there is a capital Latin letter
; returns 1 if true in EAX else 0
checkA:
	push	ebp
	mov	ebp, esp

	xor	eax, eax
	xor	edx, edx
	mov	ecx, [ebp+8]
.lp:	cmp	byte [ecx+edx], 0
	je	.quit
	cmp	byte [ecx+edx], 65	; "A"
	jae .setflag		; >65 is uppercase
.cont:	inc	edx
	jmp	.lp			; read next char

.setflag:
	cmp	byte [ecx+edx], 90	; "Z"
	ja	.cont
	mov	eax, 1

.quit:	mov	esp, ebp
	pop	ebp
	ret

; checkB([EBP+8] = str address)
; [EBP-4] = "@" flag, [EBP-8] = "." flag
; checks if there is only one "@" char and at least one "." char
; returns 1 if true in EAX else 0
checkB:
	push	ebp
	mov	ebp, esp
	sub	esp, 8			; [ebp-4] and [ebp-8]
	mov	dword [ebp-4], 0
	mov	dword [ebp-8], 0

	xor	edx, edx
	mov	ecx, [ebp+8]
.lp:	cmp	byte [ecx+edx], 0
	jz	.check
	cmp	byte [ecx+edx], 64	; "@"
	jz	.setflag_1
	cmp	byte [ecx+edx], 46	; "."
	jz	.setflag_2
	inc	edx
	jmp	.lp

.setflag_1:
	inc	dword [ebp-4]
	inc	edx
	jmp	.lp
.setflag_2:
	inc	dword [ebp-8]
	inc	edx
	jmp	.lp	

.quit:	mov	esp, ebp
	pop	ebp
	ret	8

.check:	cmp	dword [ebp-4], 1
	ja	.quit
	cmp	dword [ebp-4], 1
	jb	.quit
	cmp	dword [ebp-8], 1
	jb	.quit
	mov	eax, 1
	jmp	.quit

; checkC([EBP+8] = str address)
; checks whether the string starts with a capital letter
;  and ends with a lowercase one
; returns 1 if true in EAX else 0
checkC:
	push	ebp
	mov	ebp, esp

	xor	edx, edx
	mov	ecx, [ebp+8]
.upper:	cmp	byte [ecx], 65		; "A"
	jae	.setflag_1
	jmp	.quit

.lower:	push	ecx			; save ECX
	push	dword [ebp+8]
	call	strlen
	add	esp, 4			; now the length of the str in EAX
	pop	ecx
	cmp	byte [ecx+eax], 97	; "a"
	jae	.setflag_2
	jmp	.quit

.setflag_1:
	cmp	byte [ecx], 90		; "Z"
	ja	.quit
	mov	dword [ebp-4], 1
	jmp	.lower
.setflag_2:
	cmp	byte [ecx+eax], 122	; "z"
	ja	.quit

	mov	eax, 1
	jmp	.qt
.quit:	xor	eax, eax
.qt:	mov	esp, ebp
	pop	ebp
	ret

; checkD([EBP+8] = str address)
; [EBP-4] = flag
; checks if the string consists of the same char
; returns 1 if true in EAX else 0
checkD:
	push	ebp
	mov	ebp, esp
	sub	esp, 4
	mov	dword [ebp-4], 0

	xor	edx, edx
	mov	ecx, [ebp+8]
	cmp	byte [ecx+edx], 0
	jz	.quit
	xor	al, al
	mov	al, byte [ecx+edx]
.lp:	cmp	byte [ecx+edx], 0
	jz	.setflag
	cmp	byte [ecx+edx], al
	jnz	.quit
	inc	edx
	jmp	.lp

.setflag:
	mov	dword [ebp-4], 1
.quit:	mov	eax, dword [ebp-4]
	mov	esp, ebp
	pop	ebp
	ret

