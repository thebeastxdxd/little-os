; Check if CPUID is supported by attempting to flip the ID bit (bit 21) in
; the FLAGS register. If we can flip it, CPUID is available.
bits 32
detect_long_mode:
    pushad
    
    pushfd
    pop eax

    ; Copy to Ecx as well for comparing later on
    mov ecx, eax

    ; Flip the ID bit
    xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
    push eax
    popfd

    ; Copy Flags back to eax (with the flipped bit if CPUID is supported)
    pushfd
    pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the ID bit
    ; back if it was ever flipped).
    push ecx
    popfd

    ; Compare EAX and ECX. If they are equal then that means the bit wasn't
    ; flipped, and CPUID isn't supported.
    xor eax, ecx
    jz .no_CPUID

    ; check CPUID extended functionality by setting EAX's highest bit
    ; this returns the highest extended function implemented
    ; if it is below 0x80000001 then there is no extended functionality
    mov eax, 0x80000000 
    cpuid              ; CPU identification.
    cmp eax, 0x80000001
    jb .no_CPUID

    mov eax, 0x80000001
    cpuid
    test edx, 1 << 29
    jz .no_lm

    popad
    ret

    
.no_CPUID:
    mov al, "1"
    jmp error

.no_lm:
    mov al, "2"
    jmp error

%include "src/asm/error.s"
