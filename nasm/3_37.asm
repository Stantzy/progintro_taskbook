global	_start


section	.data
nlmsg			db	10, 0
nlmsg_len		equ $-nlmsg
num1			dd	0
num2			dd	0

section	.bss
sum			resb	12
mult			resb	12

section	.text
_start:	cmp	dword [esp], 3	; check for two cli args
	jne	error

	push	dword [esp+8]	; cli arg1
	call	read_number
	add	esp, 4
	test	cl, cl		; not 0 in cl means an error 
	jnz	error
	mov	dword [num1], eax

	push	dword [esp+12]	; cli arg2
	call	read_number
	add	esp, 4
	test	cl, cl
	jnz	error
	mov	dword [num2], eax

	mov	ebx, dword [num1]
	add	ebx, eax
	push	dword sum
	push	ebx
	call	save_number
	add	esp, 8		; the length of the str is now in EAX

	push	eax
	push	sum		; the result of addition
	call	print_number
	add	esp, 8

	call	line_break

	mov	eax, dword [num1]
	mov	ebx, dword [num2]
	mul	ebx
	push	dword mult
	push	eax
	call	save_number	; the length of the str is now in EAX
	add	esp, 8

	push	eax
	push	mult		; the result of multiplication
	call	print_number
	add	esp, 8

	call	line_break
	
quit:	mov	eax, 1
	mov	ebx, 0
	int 	80h

error:	mov	eax, 1		; sys _exit
	mov	ebx, 1		; error code
	int	80h

; read_number(arg1 == str address)
; returns number in EAX
read_number:
	push	ebp
	mov	ebp, esp
	push	esi
	
	mov	esi, dword [ebp+8]
	xor	eax, eax
	xor	ecx, ecx
	
.lp:	mov	cl, byte [esi]
	cmp	cl, '0'		
	jb	.quit		; below '0' is not a digit
	cmp	cl, '7'		; after '0' is not an octal digit
	ja	.quit
	sub	cl, 48		; convert digit char to number
	shl	eax, 3		; octal digit shift
	add	eax, ecx
	inc	esi
	jmp	.lp

.quit:	pop	esi
	mov	esp, ebp
	pop	ebp
	ret

; save_number(arg1 == number's address, arg2 == dest address)
; writes a number as an array of chars to memory address
save_number:
	push	ebp
	mov	ebp, esp
	push	edi
	sub	esp, 4
	
	mov	byte [ebp-4], 8	; divider
	mov	eax, [ebp+8]	
	xor	ecx, ecx

.loop:	xor	edx, edx
	div	dword [ebp-4]	; eax / 8
	push	edx		; save the remainder
	inc	ecx		; +1 digit
	test	eax, eax	; no remainder 
	jnz	.loop

	xor	eax, eax
	mov	edi, dword [ebp+12]
.save:	pop	edx
	add	dl, '0'
	mov	[edi+eax], dl
	inc	eax
	loop	.save
	
	add	esp, 4
	pop	edi	
	mov	esp, ebp
	pop	ebp
	ret

; print_number(arg1 == num's address, arg2 == length)
;
print_number:
	push	ebp
	mov	ebp, esp

	push	ebx
	mov	edx, dword [ebp+12]	; length
	mov	ecx, dword [ebp+8]	; str address
	mov	ebx, 1		; stdout
	mov	eax, 4		; sys write
	int	80h	
	pop	ebx
	
	mov	esp, ebp
	pop	ebp
	ret
; line_break
; prints the newline character
line_break:
	push	nlmsg_len
	push	nlmsg
	call	print_number
	add	esp, 8
	ret
