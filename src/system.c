#include <stddef.h>
#include "system.h"

uint8_t* memcpy(uint8_t* dest, uint8_t* src, size_t count) {

    if ((dest == NULL) || (src == NULL)) {
        return NULL;
    }
    for (size_t i = 0; i < count; i++) {
        dest[i] = src[i];
    }
    return dest;
}

uint8_t* memset(uint8_t* dest, uint8_t val, size_t count) {
    
    if (dest == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < count; i++) {
        dest[i] = val;
    }
    return dest;
}

uint16_t* memsetw(uint16_t* dest, uint16_t val, size_t count) {
    
    if (dest == NULL) {
        return NULL;
    }
    for (size_t i = 0; i < count; i++) {
        dest[i] = val;
    }
    return dest;
}

size_t strlen(const char* str) {

    size_t len = 0;
    while (str[len] != '\0')
        len++;
    return len;
}

uint8_t inportb(uint16_t _port) {

    uint8_t rv;
    __asm__ __volatile__ ("inb %[port], %[rv]" : [rv] "=a" (rv) : [port] "dN"(_port));
    return rv;
}

void outportb(uint16_t _port, uint8_t _data) {

    __asm__ __volatile__ ("outb %[data], %[port]" : : [port] "dN" (_port), [data] "a" (_data));
}
