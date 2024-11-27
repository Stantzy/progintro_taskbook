global	_start

section	.bss
str_address	resd	4	

section	.data
longest_lenght	db	0
args		db	0

section	.text
_start:
	call	count_args
	mov	[args], dl		; number of args to "args" var
	
	cmp	[args], byte 0		; if no command line args
	je	quit			; then end program

lp:	xor	eax, eax		; loop for args
	

	dec	[args]			; -1 readed str
	cmp	[args], byte 0
	je	print_str

quit:	mov	eax, 1			; syscall _exit
	mov	ebx, 0
	int	80h




;----------------------------------------------------------
; count_args()
; returns the number of command line args in EDX
count_args:
	xor	edx, edx
	mov	edx, [esp]		; command line args to ECX
	dec	edx			; subtract prog name
	ret

; str_lenght(arg1 [EBP+8] = str address)
; counts the number of chars in a string
; returns lenght of str in EAX
str_lenght:
	push	ebp
	mov	ebp, esp
	
	
	
	mov	esp, ebp
	pop	ebp

; save_str(ESI = str address)
; saves the address of string in str_address
save_str:


; is_longest(EAX = lenght of the string, ESI = address)
; checks if the current str is the longest
; returns 1 in EAX if true, else 0
is_longest:


; read_arg([ESP+8] = command line arg / str address)
; 
read_arg:



; print_str(arg1 [EBP+8] = str address)
; prints string
print_str:
	push	ebp
	mov	ebp, esp
	


	mov	esp, ebp
	pop	ebp
