#ifndef __MYOS__KPRINTF_H
#define __MYOS__KPRINTF_H

#include <common/types.h>

	/* Simplest implementation of printing the text to the screen in kernel space.*/
	void kprintf(const char* str);
	void kprintfHex(myos::common::uint8_t key);
	void kprintfHex16(myos::common::uint16_t key);
	void kprintfHex32(myos::common::uint32_t key);

#endif