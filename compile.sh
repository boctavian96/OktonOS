#!/bin/bash

nasm -f elf64 stub.asm -o stub.o

fpc -Aelf -n -O3 -Op3 -Si -Sc -Sg -Xd -CX -XXs -Rintel -Tlinux kernel.pas 

ld --gc-sections -s -Tlinker.script -o kernel.obj stub.o kernel.o multiboot.o system.o console.o
