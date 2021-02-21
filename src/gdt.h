#ifndef __GDT_H__
#define __GDT_H__

#include <stdint.h>

#define KERNEL_BASE_SELECTOR 0x08
#define USER_BASE_SELECTOR 0x18

typedef struct {
    uint16_t limit15_0;
    uint16_t base15_0;
    uint16_t base23_16;
    uint8_t type;
    uint8_t limit19_16_and_flags;
    uint8_t base31_24;
} __attribute__((packed)) gdt_descriptor_t;

typedef struct {
    gdt_descriptor_t null_1;
    gdt_descriptor_t kernel_code;
    gdt_descriptor_t kernel_data;
    gdt_descriptor_t null_2;
    gdt_descriptor_t user_data;
    gdt_descriptor_t user_code;
} __attribute__((packed)) gdt_table_t;

#endif
