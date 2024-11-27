%include "stud_io.inc"

global	_start

%macro	jmpmrk	1-*
	cmp	eax, 1
	jb near %%quit
	cmp	eax, %0
	ja near %%quit

	jmp	 %%jtable

%%table:
	dd	%1
	%rep	%0-1
	  %rotate 1
	  dd	%1
	%endrep
	
%%jtable:
	jmp	[%%table+eax*4-4]

%%quit:	nop
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
