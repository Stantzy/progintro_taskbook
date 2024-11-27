%include "stud_io.inc"
global _start

%macro jmpmrk 1-*
  %assign n 1
  %rep %0

    cmp eax, n
    je %1

    %assign n n+1
    %rotate 1

  %endrep
%endmacro

section	.text
_start:	mov	eax, 3
	jmpmrk	test1, test2, test3
	jmp	exit

test1:	PUTCHAR 49
	jmp exit
test2:	PUTCHAR	50
	jmp exit
test3:	PUTCHAR	51
	jmp exit

exit:	PUTCHAR 10
	FINISH