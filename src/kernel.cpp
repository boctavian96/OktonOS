/*
***Okton OS Kernel***
Version : 0.1
Author : Octavian
*/

#include <types.h>
#include <io.h>
#include <konsole.h>
#include <gdt.h>
#include <port.h>
#include <interrupts.h>
#include <keyboard.h>
#include <driver.h>
#include <mouse.h>

class PrintfKeyboardEventHandler : public PrintfKeyboardEventHandler
{
public:
    void OnKeyDown(char c)
    {
        char* tst = " ";
        tst[0] = c;
        printf(tst);
    }
};

class MouseToConsole : public MouseEventHandler
{
    int8_t x, y; 
    
public:
    
    MouseToConsole(){}
    
    virtual void OnActivate()
    {
        uint16_t* VideoMemory = (uint16_t*)0xb8000;
        x = 40;
        y = 12;
        
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4 | (VideoMemory[80*y+x] & 0xF000) >> 4 | (VideoMemory[80*y+x] & 0x00FF);
    }
    
    virtual void OnMouseMove(int xoffset, int yoffset)
    {
        static uint16_t* VideoMemory = (uint16_t*)0xb8000;
        
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4 | (VideoMemory[80*y+x] & 0xF000) >> 4 | (VideoMemory[80*y+x] & 0x00FF);
        
        x += xoffset;
        if(x >= 80)
            x = 79;
        
        if(x < 0)
            x = 0;
        
        y+= yoffset;
        if(y >= 25)
            y = 24;
        
        if(y < 0)
            y = 0;
        
        VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0x0F00) << 4 | (VideoMemory[80*y+x] & 0xF000) >> 4 | (VideoMemory[80*y+x] & 0x00FF);
    }
};

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
    printf("Okton v0.1, Hello World, WORKS !!!\n");
    
    
    
    GlobalDescriptorTable gdt;
    InterruptManager interrupts(0x20, &gdt);
    //KeyboardDriver keyboard(&interrupts);
    DriverManager drvManager;
    
    PrintfKeyboardEventHandler kbhandler;
    KeyboardDriver keyboard(&interrupts, &kbhandler);
    drvManager.AddDriver(&keyboard);
    
    MouseToConsole mousehandler;
    MouseDriver mouse(&interrupts, &mousehandler);
    drvManager.AddDriver(&mouse);
    
    printf("Initializing HardWare \n");
        drvManager.ActivateAll();
        
    printf("Initializing HardWare \n Succes");
    
    
    
    
    interrupts.Activate();
    while(1);
}

