%include "stud_io.inc"

global	_start

section	.text
_start:	xor	eax, eax	; clear eax
	xor	ecx, ecx	; clear ecx
rdstr:	GETCHAR			; save char in EAX
	cmp	eax, -1		; EOF?
	je	quit
	cmp	eax, 10		; is it end of string?
	je	print_stars	; then print stars
	cmp	eax, 32		; is it space in EAX?
	jne	cnt		; if not count word
	jmp	rdstr		; else read next char
cnt:	inc	ecx		; ecx+1
check_space:
	GETCHAR			; read next char
	cmp	eax, -1		; EOF?
	je	quit
	cmp	eax, 10		; is it end of string?
	je	print_stars	; then print stars
	cmp	eax, 32		; is it space?
	jne	check_space	; if not read next char
	jmp	rdstr		; read next word in string
print_stars:
	PUTCHAR 10		; line break
	cmp	ecx, 0		; empty string?
	je	rdstr		; if yes read next string
lp:	PRINT 	"*"
	loop	lp
	xor	eax, eax
	xor	ecx, ecx
	jmp	rdstr		; next string
quit:	FINISH			; end program
