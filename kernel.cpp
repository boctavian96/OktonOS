/*
***Okton OS Kernel***
Version : 0.1
Author : Octavian
*/

#include "types.h"
#include "io.h"
#include "konsole.h"
#include "gdt.h"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;

/*
void printf(char* str)
{
    unsigned short* VideoMemory = (unsigned short*) 0xb8000;

    for(int i=0; str[i] != '\0'; i++)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}
*/

extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t /* magicnumber */)
{
    //GlobalDescriptorTable gdt;
    printf("Okton v0.1, Hello World, WORKS !!!");
    while(1);
}

