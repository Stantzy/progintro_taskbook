global	_start

section	.text
_start:	mov	ecx, 4		; three arguments + prog name
				; according the task conditions	
	cmp	ecx, [esp]	; in ESP number of arguments
	jnz	error		; if not then end program with code 1

success:
	mov	eax, 1		; syscall _exit
	mov	ebx, 0		; code "success"
	int	80h		

error:	mov	eax, 1		; syscall _exit
	mov	ebx, 1		; code "fail"
	int	80h
