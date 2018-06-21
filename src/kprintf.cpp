#include <kprintf.h>
#include <common/types.h>

using namespace myos::common;

void kprintf(const char* str)
{
	 /* Specific memory location: whatever we write here, VGA will put on screen */
	 /* TO-DO: add scrolling */
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;
    static uint8_t x=0,y=0;

    for(int i = 0; str[i] != '\0'; ++i)
    {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
				/* Avoid overriding the high byte of the color information in memory */
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }
}

void kprintfHex(uint8_t key)
{
    char* foo = "00";
    char* hex = "0123456789ABCDEF";
    foo[0] = hex[(key >> 4) & 0xF];
    foo[1] = hex[key & 0xF];
    kprintf(foo);
}

void kprintfHex16(uint16_t key)
{
    kprintfHex((key >> 8) & 0xFF);
    kprintfHex( key & 0xFF);
}

void kprintfHex32(uint32_t key)
{
    kprintfHex((key >> 24) & 0xFF);
    kprintfHex((key >> 16) & 0xFF);
    kprintfHex((key >> 8) & 0xFF);
    kprintfHex( key & 0xFF);
}