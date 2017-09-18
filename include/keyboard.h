#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#include "types.h"
#include "interrupts.h"
#include "port.h"
#include "driver.h"

class KeyboardDriver : public InterruptHandler
{
    Port8Bit dataport;
    Port8Bit commandport;
public:
    KeyboardDriver(InterruptManager* manager, KeyboardEventHandler* handler);  
    ~KeyboardDriver();
    virtual uint32_t HandleInterrupt(uint32_t esp);
    virtual void Activate();
};

class KeyboardEventHandler
{
public:
    KeyboardEventHandler();
    
    virtual void OnKeyDown(char);
    virtual void OnKeyUp(char);
};

#endif