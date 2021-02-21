#include <stdint.h>

/* 
 * without this functions where gdb adds the stack guard
 * fail.
 */
// TODO: Make this value randomized
#define STACK_CHK_GUARD 0x595e9fbd94fda766

uintptr_t __stack_chk_guard = STACK_CHK_GUARD;

/**
 * Basic stack smashing protector implementation
 * Based on https://wiki.osdev.org/Stack_Smashing_Protector
 */
void __stack_chk_fail(void) {
    // TODO: do something
    return;
}
