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
	xor	edi, edi
	xor	ecx, ecx
	mov	edi, first_num_str
	mov	ecx, 11
	call	read

	cmp	eax, 0
	jne	error		; not 0 in EAX means error from func

	xor	esi, esi
	xor	ecx, ecx
	mov	esi, first_num_str
	mov	ecx, ebx
	call	str_to_num

	cmp	ecx, 1		; if func returns error (ECX is error flag)
	je	error		; then end program with error
	mov	[first_num], eax	; else save number

; read and save second number
	xor	edi, edi
	xor	ecx, ecx
	mov	edi, second_num_str
	mov	ecx, 11
	call	read

	cmp	eax, 0
	jne	error		; not 0 in EAX means error from func

	xor	esi, esi
	xor	ecx, ecx
	mov	esi, second_num_str
	mov	ecx, ebx
	call	str_to_num

	cmp	ecx, 1		; if func returns error
	je	error		; then end program with error
	mov	[second_num], eax	; else save number

; addition
	mov	eax, [first_num]
	mov	ebx, [second_num]
	add	eax, ebx
	mov	[result_num], eax	; save result in result var

	xor	esi, esi
	xor	edi, edi
	mov	esi, result_num
	mov	edi, result_str
	call	num_to_str

	xor	eax, eax
	mov	eax, result_str
	call	print_number

; substraction
	mov	eax, [first_num]
	mov	ebx, [second_num]
	sub	eax, ebx
	mov	[result_num], eax

	xor	esi, esi
	xor	edi, edi
	mov	esi, result_num
	mov	edi, result_str
	call	num_to_str

	xor	eax, eax
	mov	eax, result_str
	call	print_number

; multiplication
	mov	eax, [first_num]
	mov	ebx, [second_num]
	mul	ebx
	mov	[result_num], eax

	xor	esi, esi
	xor	edi, edi
	mov	esi, result_num
	mov	edi, result_str
	call	num_to_str

	xor	eax, eax
	mov	eax, result_str
	call	print_number

exit:	FINISH

error:	PRINT	"ERROR"
	PUTCHAR	10
	FINISH

; str_to_num(ESI=string address, ECX=length of string)
; EAX=result number, CL=error flag
str_to_num:
	xor	eax, eax
	xor	ebx, ebx
.loop:	xor	edx, edx
	mov	dl, [esi]	; read digit from string
	cmp	dl, 48
	jl	.err
	cmp	dl, 57
	jg	.err
	sub	dl, 48		; edx-48=number(not char)
	xor	edi, edi
	mov	edi, edx
	mov	eax, ebx
	mul	dword [multiplier]
	mov	ebx, eax
	add	ebx, edi
	inc	esi
	loop	.loop
	mov	eax, ebx	; result in eax to return
	mov	cl, 0		; no error
	ret

.err:	mov	eax, -1		; if error return -1 in EAX
	mov	ecx, 1		; error flag
	ret 

; num_to_str(ESI=number address, EDI=destination address of string)
num_to_str:
	xor	eax, eax
	xor	edx, edx
	mov	eax, [esi]	; copy number to eax
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
	ret

; print_number(EAX=source address) 
print_number:
	xor	ebx, ebx
	xor	ecx, ecx
	mov	ecx, eax	; save starting address in ECX
.loop:	cmp	[eax], byte 0	; 0 is the end of an array of digits
	je	.reverse_print
	inc	eax
	jmp	.loop
.reverse_print:
	mov	bl, [eax]
	PUTCHAR	bl
	dec	eax
	cmp	eax, ecx
	jge	.reverse_print
	PUTCHAR	10
	ret

; read(EDI=destination address, ECX=allocated memory size)
; EAX=code of result, EBX=number of digits
read:
	xor	ebx, ebx
.loop:	GETCHAR			; read symbol
	cmp	al, -1		; if EOF
	je	.exit		; end func
	cmp	al, 10		; if end of line
	je	.exit		; end func
	cmp	al, 48
	jl	.input_error
	cmp	al, 57
	jg	.input_error
	inc	ebx		; +1 readed symbol
	mov	[edi], al	; copy readed digit into memory
	inc	edi		; next address in memory
	loop	.loop		; read next symbol
	jmp	.size_error
.exit:	
	mov	eax, 0		; no error
	ret
.size_error:
	mov	eax, 1		; number of digits > allocated memory size
	ret
.input_error:
	ret			; in EAX returns code of error symbol
