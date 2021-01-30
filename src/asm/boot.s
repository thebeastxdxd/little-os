global start ; exports a label (makes it public). as start will be the entry

; allocate small stack
section .bss
align 16
stack_bottom:
resb 16384 ; 16 KiB
stack_top:
 
section .text ; executable code
bits 32 ; specifies that the following lines are 32-bit instructions.
		; It's needed because the CPU is still in Protected mode when
		; GRUB starts our kernel. When we switch to Long mode, we can
		; use bits 64 (64-bit instructions).

start:
	; The bootloader has loaded us into 32-bit protected mode on a x86
	; machine. Interrupts are disabled. Paging is disabled. The processor
	; state is as defined in the multiboot standard. The kernel has full
	; control of the CPU. The kernel can only make use of hardware features
	; and any code it provides as part of itself. There's no printf
	; function, unless the kernel provides its own <stdio.h> header and a
	; printf implementation. There are no security restrictions, no
	; safeguards, no debugging mechanisms, only what the kernel provides
	; itself. It has absolute and complete power over the
	; machine.
 
	; To set up a stack, we set the esp register to point to the top of our
	; stack (as it grows downwards on x86 systems). This is necessarily done
	; in assembly as languages such as C cannot function without a stack.
	mov esp, stack_top
	mov ebp, 0
	; `ebx` points to a boot information structure. We move it to `edi` to
	; pass it to our kernel
	mov edi, ebx
 
	; This is a good place to initialize crucial processor state before the
	; high-level kernel is entered. It's best to minimize the early
	; environment where crucial features are offline. Note that the
	; processor is not fully initialized yet: Features such as floating
	; point instructions and instruction set extensions are not initialized
	; yet. The GDT should be loaded here. Paging should be enabled here.
	; C++ features such as global constructors and exceptions will require
	; runtime support to work as well.
 
	; Enter the high-level kernel. The ABI requires the stack is 16-byte
	; aligned at the time of the call instruction (which afterwards pushes
	; the return pointer of size 4 bytes). The stack was originally 16-byte
	; aligned above and we've since pushed a multiple of 16 bytes to the
	; stack since (pushed 0 bytes so far) and the alignment is thus
	; preserved and the call is well defined.
	; note, that if you are building on Windows, C functions may have "_" prefix in assembly: _kernel_main
	call check_multiboot
    call detect_long_mode
    call init_pt_protected 
    call enable_paging
    ; We are now in long mode!
    ; But we are in 32-bit compatibility submode and we want to enter 64-bit long mode.
    ; To do the we need to load a GDT with the 64-bit flags set in the code and data selectors.

    lgdt [gdt_64_descriptor]
    jmp kernel_code_seg_64:init_lm

; --------------------------------------------------------------------------------------------
bits 32
%define MULTIBOOT2_MAGIC_VALUE 0x36d76289

check_multiboot:
	cmp eax, MULTIBOOT2_MAGIC_VALUE
	jne .no_multiboot
	ret

.no_multiboot:
	mov al, "0"
	jmp error

; --------------------------------------------------------------------------------------------
; Includes
%include "src/asm/elevate.s"
%include "src/asm/gdt.s"
%include "src/asm/init_pt.s"
%include "src/asm/long_mode.s" ; also includes error.s

; --------------------------------------------------------------------------------------------
; 64-bit code below
extern kernel_main

section .text
bits 64

init_lm:

    cli 
    mov ax, kernel_data_seg_64
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
	call kernel_main
 
	; If the system has nothing more to do, put the computer into an
	; infinite loop. To do that:
	; 1) Disable interrupts with cli (clear interrupt enable in eflags).
	;    They are already disabled by the bootloader, so this is not needed.
	;    Mind that you might later enable interrupts and return from
	;    kernel_main (which is sort of nonsensical to do).
	; 2) Wait for the next interrupt to arrive with hlt (halt instruction).
	;    Since they are disabled, this will lock up the computer.
	; 3) Jump to the hlt instruction if it ever wakes up due to a
	;    non-maskable interrupt occurring or due to system management mode.
	cli
.hang:	hlt
	jmp .hang
.end:
