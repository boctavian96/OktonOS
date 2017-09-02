/*
***Okton OS Kernel***
Version : 0.1
Author : Octavian
*/

#include "types.h"

void printf(int8_t* str);
void waitForInput();
void getInput();
int getCommand(int8_t* input);

void printf(int8_t* str)
{
    uint16_t* VideoMemory = (uint16_t*) 0xb8000;

    for(int i=0; str[i] != '\0'; i++)
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
}

void waitForInput()
{
    getInput();
}

void getInput()
{
    int8_t* input = "hello";
    if (1)
    {
        printf("FAILURE");
    }
    else 
    {
        printf("SUCCES");
    }
}

int getCommand(int8_t* input)
{
    if(1)
    {
        printf("Ich liebe Ade");        
    }
    else
    {
        return 1;
    }
    return 0;
}

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
    printf("Okton v0.1, Hello World, WORKS !!!");
    while(1);
}

