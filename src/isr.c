#include "isr.h"
#include "idt.h"
#include "logging.h"
#include "pic.h"

void isr_default_handler(isr_stack_t* stack);

const char* exception_messages[] = {"Division By Zero",
                                    "Debug",
                                    "Non Maskable Interrupt",
                                    "Breakpoint",
                                    "Into Detected Overflow",
                                    "Out of Bounds",
                                    "Invalid Opcode",
                                    "No Coprocessor",

                                    "Double Fault",
                                    "Coprocessor Segment Overrun",
                                    "Bad TSS",
                                    "Segment Not Present",
                                    "Stack Fault",
                                    "General Protection Fault",
                                    "Page Fault",
                                    "Unknown Interrupt",

                                    "Coprocessor Fault",
                                    "Alignment Check",
                                    "Machine Check",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",

                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved",
                                    "Reserved"};

static isr_handler_t handlers[IDT_ENTRIES] = {0};

void isr_init() {

    PIC_remap(IRQ_BASE, IRQ_BASE + 8);
    idt_register_interrupt(0, (uint64_t)interrupt_handler_0);
    idt_register_interrupt(1, (uint64_t)interrupt_handler_1);
    idt_register_interrupt(2, (uint64_t)interrupt_handler_2);
    idt_register_interrupt(3, (uint64_t)interrupt_handler_3);
    idt_register_interrupt(4, (uint64_t)interrupt_handler_4);
    idt_register_interrupt(5, (uint64_t)interrupt_handler_5);
    idt_register_interrupt(6, (uint64_t)interrupt_handler_6);
    idt_register_interrupt(7, (uint64_t)interrupt_handler_7);
    idt_register_interrupt(8, (uint64_t)interrupt_handler_8);
    idt_register_interrupt(9, (uint64_t)interrupt_handler_9);
    idt_register_interrupt(10, (uint64_t)interrupt_handler_10);
    idt_register_interrupt(11, (uint64_t)interrupt_handler_11);
    idt_register_interrupt(12, (uint64_t)interrupt_handler_12);
    idt_register_interrupt(13, (uint64_t)interrupt_handler_13);
    idt_register_interrupt(14, (uint64_t)interrupt_handler_14);
    idt_register_interrupt(15, (uint64_t)interrupt_handler_15);
    idt_register_interrupt(16, (uint64_t)interrupt_handler_16);
    idt_register_interrupt(17, (uint64_t)interrupt_handler_17);
    idt_register_interrupt(18, (uint64_t)interrupt_handler_18);
    idt_register_interrupt(19, (uint64_t)interrupt_handler_19);
    idt_register_interrupt(20, (uint64_t)interrupt_handler_20);
    idt_register_interrupt(21, (uint64_t)interrupt_handler_21);
    idt_register_interrupt(22, (uint64_t)interrupt_handler_22);
    idt_register_interrupt(23, (uint64_t)interrupt_handler_23);
    idt_register_interrupt(24, (uint64_t)interrupt_handler_24);
    idt_register_interrupt(25, (uint64_t)interrupt_handler_25);
    idt_register_interrupt(26, (uint64_t)interrupt_handler_26);
    idt_register_interrupt(27, (uint64_t)interrupt_handler_27);
    idt_register_interrupt(28, (uint64_t)interrupt_handler_28);
    idt_register_interrupt(29, (uint64_t)interrupt_handler_29);
    idt_register_interrupt(30, (uint64_t)interrupt_handler_30);
    idt_register_interrupt(31, (uint64_t)interrupt_handler_31);

    // Hardware interrupts
    idt_register_interrupt(IRQ0, (uint64_t)irq0);
    idt_register_interrupt(IRQ1, (uint64_t)irq1);
    idt_register_interrupt(IRQ2, (uint64_t)irq2);
    idt_register_interrupt(IRQ3, (uint64_t)irq3);
    idt_register_interrupt(IRQ4, (uint64_t)irq4);
    idt_register_interrupt(IRQ5, (uint64_t)irq5);
    idt_register_interrupt(IRQ6, (uint64_t)irq6);
    idt_register_interrupt(IRQ7, (uint64_t)irq7);
    idt_register_interrupt(IRQ8, (uint64_t)irq8);
    idt_register_interrupt(IRQ9, (uint64_t)irq9);
    idt_register_interrupt(IRQ10, (uint64_t)irq10);
    idt_register_interrupt(IRQ11, (uint64_t)irq11);
    idt_register_interrupt(IRQ12, (uint64_t)irq12);

    for (int i = 0; i < IDT_ENTRIES; i++) {
        isr_register_handler(i, isr_default_handler);
    }
    idt_load();

    isr_enable_interrupts();
}

void isr_enable_interrupts() { __asm__("sti"); }

void isr_disable_interrupts() { __asm__("cli"); }

void isr_int_handler(isr_stack_t stack) {
    isr_handler_t handler = {0};

    if (handlers[stack.id] != 0) {
        handler = handlers[stack.id];
        handler(&stack);
        return;
    }

    // TODO: panic
}

void isr_irq_handler(isr_stack_t stack) {
    isr_handler_t handler = {0};

    if (handlers[stack.id] != 0) {
        handler = handlers[stack.id];
        handler(&stack);
    }

    PIC_send_EOI(stack.id - IRQ_BASE);
}
void isr_register_handler(uint64_t id, isr_handler_t handler) {
    handlers[id] = handler;
}

void isr_default_handler(isr_stack_t* stack) {
    DEBUG("Exception: DEFAULT \n"
          "  intterupt_id        = %#x\n"
          "  instruction_pointer = %p\n"
          "  code_segment        = %#x\n"
          "  cpu_flags           = %#x\n"
          "  stack_pointer       = %p\n"
          "  stack_segment       = %#x\n"
          "\n"
          "  rax = 0x%08x    rbx = 0x%08x    rcx = 0x%08x\n"
          "  rdx = 0x%08x    rsi = 0x%08x    rdi = 0x%08x\n"
          "  rbp = 0x%08x    r8  = 0x%08x    r9  = 0x%08x\n"
          "  r10 = 0x%08x    r11 = 0x%08x    r12 = 0x%08x\n"
          "  r13 = 0x%08x    r14 = 0x%08x    r15 = 0x%08x",
          stack->id, stack->instruction_pointer, stack->code_segment, stack->cpu_flags,
          stack->stack_pointer, stack->stack_segment, stack->rax, stack->rbx,
          stack->rcx, stack->rdx, stack->rsi, stack->rdi, stack->rbp, stack->r8,
          stack->r9, stack->r10, stack->r11, stack->r12, stack->r13, stack->r14,
          stack->r15);
}
