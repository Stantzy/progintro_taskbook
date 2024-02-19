%include "stud_io.inc"

global _start

; %1: start value  %2: step  %3: number of numbers
%macro	fill_array 3
	%assign	n %1			; initiate first number
	%rep %3				; loop
		dd	n		; save n value
		%assign	n n+%2		; next n = n + step
	%endrep				; end of loop
%endmacro

section	.data
array	fill_array 1, 1, 10		; test values
array_size equ ($-array)/4

section	.text
; this program will not display numbers more than 9
; this is just a test version to check if the macro
; works correctly 
_start:	cmp	ecx, array_size
	je	end
	mov	al, [array+ecx*4]
	add	al, 48
	PUTCHAR	al
	PUTCHAR	10
	inc	ecx
	jmp	_start
end:	FINISH
