#!/bin/bash
# bash script for compiling solved tasks
nasm -f elf $1.asm
ld -m elf_i386 $1.o -o $1
rm $1.o
