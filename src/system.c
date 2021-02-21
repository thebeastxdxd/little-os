#include "system.h"
#include <stdarg.h>
#include <stddef.h>

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
    __asm__ __volatile__("inb %[port], %[rv]"
                         : [rv] "=a"(rv)
                         : [port] "dN"(_port));
    return rv;
}

void outportb(uint16_t _port, uint8_t _data) {

    __asm__ __volatile__("outb %[data], %[port]"
                         :
                         : [port] "dN"(_port), [data] "a"(_data));
}

char* reverse_str(char* buf) {
    size_t buf_len = strlen(buf);
    size_t i, j;
    char tmp;
    for (i = 0, j = (buf_len - 1); i < (buf_len / 2); i++, j--) {
        tmp = buf[i];
        buf[i] = buf[j];
        buf[j] = tmp;
    }
    return buf;
}

char* itoa_base10(int val, char* buf) {

    char c = 0;
    int index = 0;

    while (val != 0) {
        c = (val % 10) + '0';
        buf[index++] = c;
        val /= 10;
    }
    buf[index] = '\0';
    reverse_str(buf);

    return buf;
}

char* concat_str(char* str1, const char* str2) {
    int i = 0;
    int j = 0;
    while (str1[i] != '\0') {
        i++;
    }
    while (str2[j] != '\0') {
        str1[i++] = str2[j++];
    }
    str1[i] = '\0';
    return str1;
}
