section .text
bits 32

enable_paging:
    pushad
    ; Elevate to 64-bit mode
    mov ecx, 0xC0000080    ; Set ECX to 0xC0000080, which is the EFER MSR.
    rdmsr
    or eax, 1 << 8         ; Set the LM-bit which is the 8th bit
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31        ; Set the PG-bit, which is the 31nd bit
    mov cr0, eax
    
    popad
    ret


