bits 32
; Identity map the first 2 megabytes.
; Set up four tables at 0x1000 (assuming that this is free to use)
; PML4T -> 0x1000 (Page Map Level 4 Table)
; PDPT  -> 0x2000 (Page Directory Pointer Table)
; PDT   -> 0x3000 (Page Directory Table)
; PT    -> 0x4000 (Page Table)
;
; PML4T[0] -> PDPT 
; PDPT[0] -> PDT
; PDT[0] -> PT
; PT -> 0x00000000 - 0x00200000
; Remember that pagetables need to be aligned to 4096 bytes, the lowest 12 bits
; can be used for flags in the table. 
; Notice that we're setting our flags to "0x003", because we care most about bits 0 and 1.
; Bit 0 is the "exists" bit, and is only set if the entry corresponds to another page table
; (for PML4T, PDPT, PDT) and a physical page for PT.
; Bit 1 is "read/write" for which allows us to view and modify the given entry. Since we
; want our OS to have full control, we'll set this as well.
;
; Each table in the page table has 512 entries, all of which are 8 bytes
; (one quadword or 64 bits) long. 
; Note that this only requires one page table, so the upper 511 entries in the 
; PML4T, PDPT, and PDT will be NULL.
; 
init_pt_protected:
    pushad
    
    mov edi, 0x1000 ; Set the destination index to 0x1000
    mov cr3, edi    ; Set control register 3 to the destination index
    xor eax, eax 
    mov ecx, 4096   
    rep stosd       ; clear memory

    mov edi, cr3

    mov DWORD [edi], 0x2003  ; Set PML4T[0] to address 0x2000 (PDPT) with flags 0x0003
    add edi, 0x1000
    mov DWORD [edi], 0x3003  ; Set PDPT[0] to address  0x3000 (PDT)  with flags 0x0003
    add edi, 0x1000
    mov DWORD [edi], 0x4003  ; Set PDT[0] to address   0x4000 (PT)   with flags 0x0003

    ; Fill in the final page table
    ; NOTE: edi is at 0x3000

    add edi, 0x1000      ; Go to PT[0]
    mov ebx, 0x00000003  ; EBX has address 0x0000 with flags 0x0003
    mov ecx, 512         ; Do the operation 512 times

    add_page_entry_protected:
        ; a = address, x = index of page table, flags are entry flags
        mov DWORD [edi], ebx    ; Write EBX to PT[x] = a.append(flags)
        add ebx, 0x1000         ; Increment address of ebx (a+1)
        add edi, 8              ; Increment page table location (since entries are 8 bytes)
                                ; x++
        loop add_page_entry_protected

    ; Set up PAE paging, but don't enable it quite yet
    ;
    ; Here we're basically telling the CPU that we want to use paging, but not quite yet.
    ; We're enabling the feature, but not using it
    mov eax, cr4
    or eax, 1 << 5 ; Set the PAE-bit, which is the 5th bit
    mov cr4, eax

    popad
    ret


disable_pm_paging:
    pushad

    mov eax, cr0
    and eax, 01111111111111111111111111111111b ; Clear the PG-bit, which is bit 31.
    mov cr0, eax
    
    popad
    ret
