#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include <stddef.h>
#include <stdint.h>

uint8_t* memcpy(uint8_t* dest, uint8_t* src, size_t count);
uint8_t* memset(uint8_t* dest, uint8_t val, size_t count);
uint16_t* memsetw(uint16_t* dest, uint16_t val, size_t count);
size_t strlen(const char* str);
uint8_t inportb(uint16_t _port);
void outportb(uint16_t _port, uint8_t _data);

#endif
