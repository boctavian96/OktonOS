/*
***Okton OS Kernel***
Version : 0.1
Author : Octavian
*/

#include "types.h"
#include "io.h"
#include "konsole.h"
#include "gdt.h"
#include "port.h"
#include "interrupts.h"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
    {
        (*i)();
    }
}

extern "C" void kernelMain(void* multiboot_structure, uint32_t /* magicnumber */)
{
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    interrupts.Activate();
    printf("Okton v0.1, Hello World, WORKS !!!");
    while(1);
}

