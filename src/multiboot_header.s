section .multiboot_header
align 8

; Declare constants for the multiboot2 header.
%define MULTIBOOT2_MAGIC_NUMBER 0xe85250d6
%define PROTECTED_MODE_CODE     0 ; architecture 0 (protected mode i386)
                                  ; architecture 4 (MIPS)
 
; Declare a multiboot2 header that marks the program as a kernel. These are magic
; values that are documented in the multiboot2 standard. The bootloader will
; search for this signature in the first 32 KiB of the kernel file, aligned at a
; 64-bit boundary. The signature is in its own section so the header can be
; forced to be within the first 32 KiB of the kernel file.
header_start:
    dd MULTIBOOT2_MAGIC_NUMBER
    dd PROTECTED_MODE_CODE
	dd header_end - header_start ; header length

    ;checksum
    dd 0x100000000 - (MULTIBOOT2_MAGIC_NUMBER + PROTECTED_MODE_CODE + (header_end - header_start))

    ; required end tag
    ; `dw` means `define word` (word = 16 bits on x86_64)
    dw 0  ; type
    dw 0  ; flags
    dd 8  ; size
header_end:
