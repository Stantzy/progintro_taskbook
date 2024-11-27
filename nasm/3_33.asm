global	_start

section	.data
flag	dw	1		; start value of flag is 1

section	.text
_start:	cmp	[esp], dword 3	; checking for 2 command line args
				; (1 is prog's name)
	jne	quit
	
	xor	eax, eax
	mov	eax, dword [esp+8]	; address of the first string
	push	eax
	call	strlen
	add	esp, 4

	mov	ebx, eax	; save length of the first string

	xor	eax, eax
	mov	eax, dword [esp+12]	; address of the second string
	push	eax
	call	strlen
	add	esp, 4
	
	cmp	ebx, eax	; comparing the lengths of strings
	jne	quit		; if they are not equals then end with error
	
	xor	eax, eax
	xor	ebx, ebx
	mov	eax, dword [esp+8]	; address of the first string
	mov	ebx, dword [esp+12]	; address of the second string
	push	ebx
	push	eax
	call	cmplstsym
	add	esp, 8
	
quit:	xor	eax, eax
	xor	ebx, ebx
	mov	eax, 1		; syscall _exit
	mov	ebx, [flag]
	int	80h


; cmplstsymb([EBP+8]=str1 address, [EBP+12]=str2 address)
; compares last symbols of two strings
; set 0 in flag var if symbols are identical else 1
cmplstsym:
	push	ebp
	mov	ebp, esp

	xor	eax, eax
	mov	eax, [ebp+8]	; read first string
.loop1:
	cmp	[eax], byte 0		; 0 is end of current string
	je	.savesymbol1		; if 0 then save last symbol of string
	inc	eax
	jmp	short .loop1

.savesymbol1:
	xor	edx, edx
	dec	eax		; to read last symbol before byte 0
	mov	dl, byte [eax]

	xor	eax, eax
	mov	eax, dword [ebp+12]	; read second string

.loop2:
	cmp	[eax], byte 0
	je	.savesymbol2
	inc	eax
	jmp	short .loop2

.savesymbol2:
	xor	ecx, ecx
	dec	eax
	mov	dh, byte [eax]

.compare:
	cmp	dh, dl
	je	.setflag

.quit:
	mov	esp, ebp
	pop	ebp
	ret

.setflag:
	mov	[flag], dword 0
	jmp	.quit


; strlen([EBP+8]=str address)
; counts length of string and uses only EAX and ECX
; returns length of string in EAX
strlen:
	push	ebp
	mov	ebp, esp

	xor	eax, eax
	mov	ecx, [ebp+8]		; str

.count:
	cmp	[ecx+eax], byte 0 	; 0 is end of string
	je	.quit			; if 0 then quit func
	inc	eax			; +1 char in string
	jmp	short .count

.quit:
	mov	esp, ebp
	pop	ebp
	ret
