#include "vga.h"
#include "system.h"
#include <stdarg.h>

void terminal_initialize(void) {
    terminal_buffer = (uint16_t*)VGA_MEMORY;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_clear();
}

void terminal_clear(void) {
    terminal_row = 0;
    terminal_column = 0;
    memsetw(terminal_buffer, vga_entry(' ', terminal_color),
            VGA_HEIGHT * VGA_WIDTH);
    move_csr(terminal_column, terminal_row);
}

void terminal_setcolor(uint8_t color) { terminal_color = color; }

void move_csr(uint8_t x, uint8_t y) {
    const uint16_t index = (y * VGA_WIDTH) + x;
    outb(0x3D4, 14);
    outb(0x3D5, index >> 8);
    outb(0x3D4, 15);
    outb(0x3D5, index);
}

void scroll(void) {

    uint8_t temp;
    if (terminal_row >= VGA_HEIGHT) {
        temp = terminal_row - VGA_HEIGHT + 1;
        // shift the terminal one row up
        memcpy((uint8_t*)terminal_buffer,
               (uint8_t*)terminal_buffer + (temp * VGA_WIDTH * 2),
               (VGA_HEIGHT - temp) * VGA_WIDTH * 2);
        // blank the last line
        memsetw(terminal_buffer + ((VGA_HEIGHT - temp) * VGA_WIDTH),
                vga_entry(' ', terminal_color), VGA_WIDTH);
        terminal_row = VGA_HEIGHT - temp;
    }
}

void terminal_putentryat(char c, uint8_t color, uint8_t x, uint8_t y) {
    const uint16_t index = (y * VGA_WIDTH) + x;
    terminal_buffer[index] = vga_entry(c, color);
}

void terminal_putchar(char c) {

    // backspace
    if (c == 0x08) {
        if (terminal_column != 0)
            terminal_column--;
    }
    // tab
    // make sure it is divisible by 8
    else if (c == 0x09) {
        terminal_column = (terminal_column + 8) & ~(8 - 1);
    }
    // Carriage Return
    else if (c == '\r') {
        terminal_column = 0;
    }
    // new line, treat it as if a 'CR' was also there
    else if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else if (c >= ' ') {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        terminal_column++;
    }

    if (terminal_column >= VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
    scroll();
    move_csr(terminal_column, terminal_row);
}

void terminal_write(const char* data, size_t size) {
    for (uint16_t i = 0; i < (uint16_t)size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) {
    terminal_write(data, strlen(data));
}

void _putchar(char character) { terminal_putchar(character); }
