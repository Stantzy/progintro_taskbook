%include "stud_io.inc"

global _start

section .data
ten dd 10

section .bss
flag	resb 1 ; flag of number
frstnum resd 1 ; first number
scndnum resd 1 ; second number
sum	resb 10 ; array of digits of sum
subt	resb 10 ; array of digits of diff
mult	resb 10 ; array of digits of product

section .text
_start:
main:	xor esi, esi 	; null esi
	xor ebx, ebx 	; null ebx
	xor edx, edx 	; null edx
	mov [frstnum], dword 0
	mov [scndnum], dword 0
	mov [flag], dword 0

rd:	xor eax, eax	; null eax for current digit in stdin
	xor ecx, ecx
	GETCHAR         ; read char from stdin
	cmp eax, -1     ; -1 = EOF
	je  endprog	; if EOF then calc result
	cmp eax, 32	; check space in stdin
	je  chkpos	; read second digit
	cmp eax, 10
	je  dgt2
	cmp eax, 48	; ASCII 48 is "0"
	jl  error
	cmp eax, 57	; ASCII 57 is "9"
	jg  error
	sub eax, 48
	mov ecx, eax
	mov eax, ebx
	mul dword [ten]
	mov ebx, eax
	add ebx, ecx	; add result to edx
	jmp rd

chkpos: add [flag], byte 1 ; flag of first number
	cmp [flag], byte 1 ; 1 is a result of first number
	je  dgt1	   ; save first number
	cmp [flag], byte 2 ; 2 is to read second number
	je  rd		   ; read second number
	cmp [flag], byte 3 ; 3 is a result of second number
	je  dgt2	   ; save second number
	jmp error	

dgt1:	xor edi, edi
	mov edi, frstnum
	add [edi], ebx
	xor edi, edi
	xor ebx, ebx	; null ebx for second number
	jmp rd
	
dgt2:	cmp eax, 32	; if space after space
	je  error	; display an error message
	xor edi, edi
	mov edi, scndnum
	add [edi], ebx
	xor edi, edi
	xor ebx, ebx

; summing numbers
sumnum:	xor eax, eax
	xor ebx, ebx
	xor esi, esi
	xor ecx, ecx
	mov eax, [frstnum]
	mov ebx, [scndnum]
	mov esi, sum
	mov ecx, sum
	add eax, ebx	; 1st number + 2nd number
	jmp calc

; subtracting numbers
subtnum:
	xor eax, eax
	xor ebx, ebx
	xor esi, esi
	xor ecx, ecx
	mov eax, [frstnum]
	mov ebx, [scndnum]
	mov esi, subt
	mov ecx, subt
	sub eax, ebx	; 1st num - 2nd num
	jmp calc

; multiplying numbers
multnum:
	xor eax, eax
	xor ebx, ebx
	xor esi, esi
	xor ecx, ecx
	mov eax, [frstnum]
	mov ebx, [scndnum]
	mov esi, mult
	mov ecx, mult
	mul ebx
	jmp calc

calc:	xor edx, edx
	div dword [ten]
	mov byte [esi], dl
	cmp eax, 0
	je  prnt
	inc esi
	jmp calc

chkopr:	PUTCHAR 10
	inc edi
	cmp edi, 1
	je subtnum
	cmp edi, 2
	je multnum
	jmp endprog

prnt:	xor al, al
	mov al, [esi]
	add al, 48
	PUTCHAR al
	cmp ecx, esi
	je  chkopr
	dec esi
	jmp prnt

error:	PRINT "ERROR."
		PUTCHAR 10

endprog:
	FINISH
