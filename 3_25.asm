%include "stud_io.inc"

global	_start

section	.data
multiplier	dw 10

section	.bss
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

; str_to_num([ebp+8]=string address, [ebp+12]=length of string)
; EAX=result number, CL=error flag
str_to_num:
	push	ebp
	mov	ebp, esp
	xor	eax, eax
	xor	ecx, ecx
	push	esi
	push	edi
	push	ebx
	mov	esi, [ebp+8]
	mov	edi, [ebp+12]

.loop:	xor	edx, edx
	mov	dl, [esi]	; read digit from string
	cmp	dl, 48
	jl	.err
	cmp	dl, 57
	jg	.err
	sub	dl, 48		; edx-48=number(not char)
	xor	ebx, ebx
	mov	ebx, edx
	mov	eax, ecx
	mul	dword [multiplier]
	mov	ecx, eax
	add	ecx, ebx
	inc	esi	; next digit in string
	
	dec	edi		; loop counter-1
	cmp	edi, dword 0 ; if lc > 0
	jg	.loop		; then read next digit
	
.exit:	mov	eax, ecx	; result in eax to return
	mov	ecx, 0		; no error
	pop	ebx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8

.err:	mov	eax, -1		; if error return -1 in EAX
	mov	ecx, 1		; error flag is true
	pop	ebx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8

; num_to_str([ebp+12]=number address, [ebp+8]=destination address of string)
num_to_str:
	push	ebp
	mov	ebp, esp
	push	edi
	
	xor	eax, eax
	xor	edx, edx	
	mov	edi, [ebp+8]
	mov	edx, [ebp+12]	; copy address to edx
	mov	eax, [edx]	; copy value from address to eax
	xor	edx, edx

.loop:	div	dword [multiplier]
	add	dl, 48		; to get char of digit
	mov	[edi], dl
	xor	edx, edx
	cmp	eax, 0
	je	.exit
	inc	edi
	jmp	.loop

.exit:	inc	edi
	mov	[edi], byte 0	; flag of the end of a string

	pop	edi
	mov	esp, ebp
	pop	ebp
	ret	8

; print_number([ebp+8]=source address) 
print_number:
	push	ebp
	mov	ebp, esp

	xor	edx, edx
	xor	ecx, ecx
	mov	eax, [ebp+8]
	mov	ecx, eax	; save starting address in ECX

.loop:	cmp	[eax], byte 0	; 0 is the end of an array of digits
	je	.reverse_print
	inc	eax
	jmp	.loop

.reverse_print:
	mov	dl, [eax]
	PUTCHAR	dl
	dec	eax
	cmp	eax, ecx
	jge	.reverse_print
	PUTCHAR	10
	
	mov	esp, ebp
	pop	ebp
	ret	4

; read([ebp+12]=destination address, [ebp+8]=allocated memory size)
; EAX=code of result, ECX=number of digits
read:
	push	ebp
	mov	ebp, esp
	push	esi
	push	edi
	mov	esi, [ebp+8]
	mov	edi, [ebp+12]
	xor	ecx, ecx

.loop:	GETCHAR			; read symbol
	cmp	al, -1		; if EOF
	je	.exit		; end func
	cmp	al, 10		; if end of line
	je	.exit		; end func
	cmp	al, 48
	jl	.input_error
	cmp	al, 57
	jg	.input_error
	inc	ecx		; +1 readed symbol
	mov	[edi], al	; copy readed digit into memory
	inc	edi		; next address in memory
	dec	esi		; loop counter - 1
	cmp	esi, dword 0
	jg	.loop		; read next symbol
	jmp	.size_error

.exit:	
	mov	eax, 0		; no error
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8

.size_error:
	mov	eax, 1		; number of digits > allocated memory size
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8

.input_error:
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp
	ret	8		; in EAX returns code of error symbol
