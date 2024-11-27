#!/bin/bash
# bash script for compiling 3_26 task
for file in "$@"; do
	nasm -f elf "$file" -o "$file.o"
done
ld -m elf_i386 "${@/%/.o}" -o main
rm *.o
