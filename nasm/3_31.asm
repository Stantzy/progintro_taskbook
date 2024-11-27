%macro savestr 1
  %assign n 1
  %strlen sl %1

  %rep sl
    %substr char %1 n

    %ifdef WORD
      dw char
    %elifdef DWORD
      dd char
    %elifdef QWORD
      dq char
    %else
      %error Please define WORD/DWORD/QWORD
      %exitrep
    %endif

    %assign n n+1
  %endrep

%endmacro

section	.data
string savestr "TEST"
