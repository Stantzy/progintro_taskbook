%include "stud_io.inc"

global	_start

section	.data
error_flag	db 0

section .bss
first_num	resd 1
second_num	resd 1
operation	resb 1
result		resb 11

section	.text
_start:	xor	eax, eax
	xor	ebx, ebx
	xor	ecx, ecx
	xor	edx, edx
	xor	esi, esi
	xor	edi, edi
	
	mov	esi, 10		; multiplier
	mov	edi, result	
	mov	[first_num], dword 0
	mov 	[second_num], dword 0
	mov	[operation], byte 0

read_num1:
	xor	eax, eax
	GETCHAR
	cmp	eax, -1		; if EOF
	je	end_prog	; then end program
	
	cmp	eax, 42		; if "*"
	je	save_num1	; then save the readed number in first_num
				; and save the operation between numbers
	cmp	eax, 43		; "+"
	je	save_num1
	cmp	eax, 45		; "-"
	je	save_num1
	cmp	eax, 47		; "/"
	je	save_num1
	
	cmp	eax, 48		; ASCII 48 is "0"
	jl	err		; if less then it is not a number
	cmp	eax, 57		; ASCII 57 is "9"
	jg	err		; if greater then it's not a number
	
	sub	eax, 48		; substract 48 to get the number
	mov	ecx, eax	; save the current number in ECX
	mov	eax, ebx	; put the accumulated number in EAX
	mul	esi		; the accumulated number * 10
	mov	ebx, eax	; put the current number in EBX
	add	ebx, ecx	; and add result of mul to EBX
	xor	ecx, ecx
	jmp	read_num1	; read next char in string

read_num2:
	GETCHAR
	cmp	eax, -1		; if EOF
	je	err		; then end program
	cmp	eax, 10
	je	save_num2
	
	cmp	eax, 48		; ASCII 48 is "0"
	jl	err		; if less then it is not a number
	cmp	eax, 57		; ASCII 57 is "9"
	jg	err		; if greater then it's not a number
	
	sub	eax, 48		; substract 48 to get the number
	mov	ecx, eax	; save the current number in ECX
	mov	eax, ebx	; put the accumulated number in EAX
	mul	esi		; the accumulated number * 10
	mov	ebx, eax	; put the current number in EBX
	add	ebx, ecx	; and add result of mul to EBX
	xor	ecx, ecx
	jmp	read_num2

save_num1:
	mov	[first_num], ebx
	mov	[operation], al
	xor	eax, eax
	xor	ebx, ebx
	xor	ecx, ecx
	jmp	read_num2

save_num2:
	mov	[second_num], ebx
	xor	eax, eax
	xor	ebx, ebx
	xor	ecx, ecx
	xor	edx, edx

compute:	
	mov	eax, [first_num]
	mov	ebx, [second_num]
	mov	cl, byte [operation]
	cmp	cl, 42		; multiplication
	je	_m
	cmp	cl, 43		; addition
	je	_a
	cmp	cl, 45		; subsctraction
	je	_s
	cmp	cl, 47		; division
	je	_d	

err:	cmp	eax, 10		; if EOL
	je	reset_err	; reset flag of error
	cmp	[error_flag], byte 0
	jne	_start
	PRINT	"ERROR: invalid input."
	PUTCHAR	10
	mov	[error_flag], byte 1
	jmp	_start

reset_err:
	mov	[error_flag], byte 0
	jmp	_start

_m:	mul	ebx
	jmp	save_result
_a:	add	eax, ebx
	jmp	save_result
_s:	sub	eax, ebx
	jmp	save_result
_d:	div	ebx
	jmp	save_result

save_result:
	xor	edx, edx
	div	esi		; EAX / 10
	add	dl, 48
	mov	byte [edi], dl	; put the remainder of division in result
	cmp	eax, 0		; end of division
	je	print_result
	inc	edi		; next address in result
	jmp	save_result

print_result:
	xor	eax, eax
	mov	al, [edi]
	PUTCHAR al
	dec	edi
	cmp	edi, result
	jnl	print_result

next_num:
	PUTCHAR	10
	jmp	_start

end_prog:
	PUTCHAR	10
	FINISH
