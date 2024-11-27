%include "stud_io.inc"
 
global    _start
 
section	.text
_start:	xor	eax, eax	; null eax
rdchr:	GETCHAR                 ; read current char and save in EAX
	cmp	eax, -1		; if EOF in EAX
	je   	quit		; then end program if EOF
	cmp    	eax, 10	        ; if EOL (end of line)
	je	nxtstr          ; then read next string
	cmp    	eax, 32         ; space in EAX
	jne    	prnt           	; if not then print string
	PUTCHAR	al              ; write space from AL
	jmp    	rdchr           ; read next char
prnt:	PUTCHAR	40  	        ; open word with "("
wrd:	PUTCHAR	al           	; write symbol from AL
	GETCHAR                	; read next symbol
	cmp	eax, -1
	je	quit
	cmp	eax, 10
	je	brckt
	cmp    	eax, 32        	; is it space?
	jne    	wrd           	; if not then read next symbol in word
clswrd: PUTCHAR	41    	        ; close word with ")"
	PUTCHAR	al              ; write space from AL
	jmp    	rdchr           ; read next word in string
brckt:	PUTCHAR	41		; write ")" if EOL after symbol
nxtstr: PUTCHAR	10              ; new line
	xor    	eax, eax
	jmp    	rdchr
quit:	PUTCHAR 10              ; new line
	FINISH			; end program
