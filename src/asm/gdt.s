; Define the Flat Mode Configuration Global Descriptor Table (GDT)
; The flat mode table allows us to read and write code anywhere, without restrictions
section .data
align 4

gdt_64_start:
; Define the null sector for the 64 bit gdt
; Null sector is required for memory integrity check
gdt_64_null:
    dq 0
; Define the code sector for the 64 bit gdt
gdt_64_kernel_code:
    ; Base: 0x0000
    ; Limit: 0x0000
    ; 1st Flags:     0b1001
    ;   Present:     1
    ;   Privelege:   00
    ;   Descriptor:  1
    ; Type Flags:    0b1010
    ;   Code:        1
    ;   conforming:  0
    ;   Readable:    1
    ;   Accessed:    0
    ; 2nd Flags:     0b1100
    ; Granularity:   1
    ; 32-bit Default:0
    ; 64-bit Segment:1
    ; AVL:           0
    dw 0x0000       ; Limit (bits 0-15)
    dw 0x0000       ; Base  (16-32)
    db 0x00         ; Base  (32-40)
    db 0b10011010   ; 1st Flags, Type Flags
    db 0b10100000   ; 2nd Flags, Limit
    db 0x00         ; Base

; Define the data sector for the 64 bit gdt
gdt_64_kernel_data:
    ; Base: 0x0000
    ; Limit: 0x0000
    ; 1st Flags:     0b1001
    ;   Present:     1
    ;   Privelege:   00
    ;   Descriptor:  1
    ; Type Flags:    0b0010
    ;   Code:        0
    ;   conforming:  0
    ;   Readable:    1
    ;   Accessed:    0
    ; 2nd Flags:     0b1100
    ; Granularity:   1
    ; 32-bit Default:0
    ; 64-bit Segment:1
    ; AVL:           0
    dw 0x0000       ; Limit (bits 0-15)
    dw 0x0000       ; Base  (16-32)
    db 0x00         ; Base  (32-40)
    db 0b10010010   ; 1st Flags, Type Flags
    db 0b10100000   ; 2nd Flags, Limit
    db 0x00         ; Base
gdt_64_null_2:
    dq 0
gdt_64_data_code:
    ; Base: 0x0000
    ; Limit: 0x0000
    ; 1st Flags:     0b1001
    ;   Present:     1
    ;   Privelege:   11
    ;   Descriptor:  1
    ; Type Flags:    0b1010
    ;   Code:        1
    ;   conforming:  0
    ;   Readable:    1
    ;   Accessed:    0
    ; 2nd Flags:     0b1100
    ; Granularity:   1
    ; 32-bit Default:0
    ; 64-bit Segment:1
    ; AVL:           0
    dw 0x0000       ; Limit (bits 0-15)
    dw 0x0000       ; Base  (16-32)
    db 0x00         ; Base  (32-40)
    db 0b11110010   ; 1st Flags, Type Flags
    db 0b10100000   ; 2nd Flags, Limit
    db 0x00         ; Base
gdt_64_user_code:
    ; Base: 0x0000
    ; Limit: 0x0000
    ; 1st Flags:     0b1001
    ;   Present:     1
    ;   Privelege:   11
    ;   Descriptor:  1
    ; Type Flags:    0b1010
    ;   Code:        1
    ;   conforming:  0
    ;   Readable:    1
    ;   Accessed:    0
    ; 2nd Flags:     0b1100
    ; Granularity:   1
    ; 32-bit Default:0
    ; 64-bit Segment:1
    ; AVL:           0
    dw 0x0000       ; Limit (bits 0-15)
    dw 0x0000       ; Base  (16-32)
    db 0x00         ; Base  (32-40)
    db 0b11111010   ; 1st Flags, Type Flags
    db 0b10100000   ; 2nd Flags, Limit
    db 0x00         ; Base

gdt_64_end:

; Define the gdt descriptor
; this data structure gives cpu length and start address of gdt
; we will feed this structure to the CPU in order to set the protected mode GDT
gdt_64_descriptor:
    dw gdt_64_end - gdt_64_start - 1     ; Size of GDT, one byte less than true size
    dq gdt_64_start                      ; Start of the 64 bit gdt

    ; Define helpers to find pointers to Code and Data segments
kernel_code_seg_64:              equ gdt_64_kernel_code - gdt_64_start
kernel_data_seg_64:              equ gdt_64_kernel_data - gdt_64_start
user_code_seg_64:                equ gdt_64_user_code   - gdt_64_start
user_data_seg_64:                equ gdt_64_data_code   - gdt_64_start

