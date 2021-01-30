section .text
bits 32
; Prints `ERR: ` and the given error code to screen and hangs.
;
; Parameter: error code (in ascii) in al
;
; 0 = no multiboot
; 1 = no CPUID
; 2 = no long mode
error:
	mov dword [0xb8000], 0x4f524f45
	mov dword [0xb8004], 0x4f3a4f52
	mov dword [0xb8008], 0x4f204f20
	mov byte  [0xb800a], al
	hlt 

