global	_start

extern  cmplstsymb
extern  lenstr

section	.data
flag	dw	1		; start value of flag is 1

section	.text
_start:	cmp	[esp], dword 3	; checking for 2 command line args
				; (1 is prog's name)
	jne	error
	
	xor	eax, eax
	mov	eax, dword [esp+8]	; address of the first string
	push	eax
	call	lenstr
	add	esp, 4

	mov	ebx, eax	; save length of the first string

	xor	eax, eax
	mov	eax, dword [esp+12]	; address of the second string
	push	eax
	call	lenstr
	add	esp, 4
	
	cmp	ebx, eax	; comparing the lengths of strings
	jne	error		; if they are not equals then end with error
	
	xor	eax, eax
	xor	ebx, ebx
	mov	eax, dword [esp+8]	; address of the first string
	mov	ebx, dword [esp+12]	; address of the second string
	push	ebx
	push	eax
	call	cmplstsymb	
	add	esp, 8

	cmp	eax, 1		; 1 = last symbols are equals
	jne	error		; if not then end with error
	
quit:	xor	eax, eax
	xor	ebx, ebx
	mov	[flag], dword 0	; success code
	mov	eax, 1		; syscall _exit
	mov	ebx, [flag]
	int	80h

error:	mov	eax, 1		; syscall _exit
	mov	ebx, [flag]
	int	80h