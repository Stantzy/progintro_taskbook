%macro savestr 1

  %assign n 1
  %strlen sl %1

  %rep sl
    %substr char %1 n
    dd char
    %assign n n+1
  %endrep

%endmacro

section	.data
string savestr "TEST"