#ifndef __ISR_H__
#define __ISR_H__

#include <stdint.h>

#define IRQ_BASE 0x20

// exceptions, cf. http://wiki.osdev.org/Exceptions
#define EXCEPTION_DE 0
#define EXCEPTION_DB 1
#define EXCEPTION_BP 3
#define EXCEPTION_OF 4
#define EXCEPTION_BR 5
#define EXCEPTION_UD 6
#define EXCEPTION_NM 7
#define EXCEPTION_DF 8
#define EXCEPTION_TS 10
#define EXCEPTION_NP 11
#define EXCEPTION_SS 12
#define EXCEPTION_GP 13
#define EXCEPTION_PF 14
// 15 is "reserved"
#define EXCEPTION_MF 16
#define EXCEPTION_AC 17

// Hardware interrupts
#define IRQ0  32
#define IRQ1  33
#define IRQ2  34
#define IRQ3  35
#define IRQ4  36
#define IRQ5  37
#define IRQ6  38
#define IRQ7  39
#define IRQ8  40
#define IRQ9  41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44


// These functions are declared in the 'isr.asm' file
extern void interrupt_handler_0();
extern void interrupt_handler_1();
extern void interrupt_handler_2();
extern void interrupt_handler_3();
extern void interrupt_handler_4();
extern void interrupt_handler_5();
extern void interrupt_handler_6();
extern void interrupt_handler_7();
extern void interrupt_handler_8();
extern void interrupt_handler_9();
extern void interrupt_handler_10();
extern void interrupt_handler_11();
extern void interrupt_handler_12();
extern void interrupt_handler_13();
extern void interrupt_handler_14();
extern void interrupt_handler_15();
extern void interrupt_handler_16();
extern void interrupt_handler_17();
extern void interrupt_handler_18();
extern void interrupt_handler_19();
extern void interrupt_handler_20();
// reserved
extern void interrupt_handler_21();
extern void interrupt_handler_22();
extern void interrupt_handler_23();
extern void interrupt_handler_24();
extern void interrupt_handler_25();
extern void interrupt_handler_26();
extern void interrupt_handler_27();
extern void interrupt_handler_28();
extern void interrupt_handler_29();
extern void interrupt_handler_30();
extern void interrupt_handler_31();

// Hardware intterupts
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();

typedef struct {
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rbp;
    uint64_t rdi;
    uint64_t rsi;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;
    uint64_t id;
    uint64_t error_code;
    uint64_t instruction_pointer;
    uint64_t code_segment;
    uint64_t cpu_flags;
    uint64_t stack_pointer;
    uint64_t stack_segment;
} __attribute__((packed)) isr_stack_t;

// type for interrupt handler.
typedef void (*isr_handler_t)(isr_stack_t* stack);

/**
 * Initializes the interrupt service routine.
 */
void isr_init();

/**
 * Enables hardware interrupts.
 */
void isr_enable_interrupts();

/**
 * Disables hardware interrupts.
 */
void isr_disable_interrupts();

/**
 * This is the handler for software interrupts and exceptions.
 *
 * - Exceptions: These are generated internally by the CPU and used to alert
 *   the running kernel of an event or situation which requires its attention.
 *   On x86 CPUs, these include exception conditions such as Double Fault, Page
 *   Fault, General Protection Fault, etc.
 * - Software Interrupt: This is an interrupt signalled by software running on
 *   a CPU to indicate that it needs the kernel's attention. These types of
 *   interrupts are generally used for System Calls.
 *
 * @param s the interrupt stack
 */
void isr_int_handler(isr_stack_t s) __asm__("isr_handler");

void isr_irq_handler(isr_stack_t s) __asm__("irq_handler");

/**
 * Registers a handler for a given interrupt. It does not matter whether the
 * interrupt is an exception, a hardware or software interrupt.
 *
 * @param id an interrupt id
 * @param handler the handler to attach to the interrupt
 */
void isr_register_handler(uint64_t id, isr_handler_t handler);

#endif
