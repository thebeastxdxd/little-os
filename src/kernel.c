#include "logging.h"
#include "system.h"
#include "vga.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "isr.h"

/*
 * the differnece between quote include and bracket include is 
 * the search path used. qoute include first searches the current directory.
 * Bracket include searches a standard list or first directories included using the -I flag
 */

/* check if the compiler thinks you are targeting the wrong operating system */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble."
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__x86_64__)
#error "This tutorial needs to be compiled with a ix86/x86_64-elf compiler"
#endif


void kernel_main(void) {
    /* Initialize terminal interface */
    terminal_initialize();
    DEBUG("Hello, Kernel World!");
    
    DEBUG("initializing interrupt service routine");
    isr_init();
    DEBUG("DONE initializing interrupt service routine");
    
}
