%include "stud_io.inc"

global	_start

extern	str_to_num
extern	num_to_str
extern	print_number
extern	read

section	.bss
global	first_num_str
global	second_num_str
global	result_str


global	first_num
global	second_num
global	result_num

first_num_str	resb 11
second_num_str	resb 11
result_str	resb 11

first_num	resd 1
second_num	resd 1
result_num	resd 1


section	.text
_start:

; read and save first number
	push	dword first_num_str
	push	dword 11
	call	read

	cmp	eax, 0
	jne	error		; not 0 in EAX means error from func

	push	ecx
	push	dword first_num_str
	call	str_to_num

	cmp	ecx, 1		; if func returns error (ECX is error flag)
	je	error		; then end program with error
	mov	[first_num], eax		; else save number

; read and save second number
	push	dword second_num_str
	push	11
	call	read

	cmp	eax, 0
	jne	error		; not 0 in EAX means error from func
	
	push	ecx
	push	dword second_num_str
	call	str_to_num

	cmp	ecx, 1		; if func returns error
	je	error		; then end program with error
	mov	[second_num], eax	; else save number

; addition
	mov	eax, [first_num]
	mov	ebx, [second_num]
	add	eax, ebx
	mov	[result_num], eax	; save result in result var

	push	dword result_num
	push	dword result_str
	call	num_to_str

	push	dword result_str
	call	print_number

; substraction
	mov	eax, [first_num]
	mov	ebx, [second_num]
	sub	eax, ebx
	mov	[result_num], eax

	push	dword result_num
	push	dword result_str
	call	num_to_str

	push	result_str
	call	print_number

; multiplication
	mov	eax, [first_num]
	mov	ebx, [second_num]
	mul	ebx
	mov	[result_num], eax

	push	result_num
	push	result_str
	call	num_to_str

	push	result_str
	call	print_number

exit:	FINISH

error:	PRINT	"ERROR"
	PUTCHAR	10
	FINISH
