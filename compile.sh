#!/bin/bash

nasm -f elf stub.asm -o stub.o

fpc -Aelf -n -O3 -Op3 -Si -Sc -Sg -Xd -CX -XXs -Rintel -Tlinux kernel.pas

i386-elf-ld --gc-sections -s -Tlinker.script -o kernel.obj stub.o kernel.o multiboot.o system.o console.o
