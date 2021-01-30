ARCH ?= x86_64
KERNEL = myos-$(ARCH).bin
ISO = $(KERNEL:%.bin=%.iso)
CC = x86_64-elf-gcc
ASM = x86_64-elf-as
NASM = nasm
LD = x86_64-elf-ld
QEMU = qemu-system-x86_64
SHELL :=/bin/bash

SRCDIR = src
ASMDIR = src/asm
BINDIR = bin
OBJDIR = obj
ISODIR = isodir

SOURCES := $(wildcard $(SRCDIR)/*.c)
ASM_SRC := $(wildcard $(ASMDIR)/*.s)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
LD_SCRIPT := $(SRCDIR)/linker.ld
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ASM_OBJ := $(ASM_SRC:$(ASMDIR)/%.s=$(OBJDIR)/%.o)

CFLAGS := -Wall -Wextra -std=gnu99 -pedantic -O2 -MD -ffreestanding -nostdlib -lgcc \
		-fno-builtin -fstack-protector -mno-red-zone -mno-sse2 -fno-omit-frame-pointer
QEMU_OPTIONS = -m 500M \

.PHONY: all
all:  $(BINDIR)/$(KERNEL)
	@echo multiboot state is $(shell grub-file --is-x86-multiboot2 $(BINDIR)/$(KERNEL); echo $$?)
	cp $(BINDIR)/$(KERNEL) $(ISODIR)/boot/$(KERNEL)
	grub-mkrescue -o $(ISO) $(ISODIR)

$(BINDIR)/$(KERNEL): $(OBJECTS) $(ASM_OBJ) 
	$(LD) --nmagic --output=$@ -T $(LD_SCRIPT) $^

$(ASM_OBJ): $(OBJDIR)/%.o : $(ASMDIR)/%.s 
	$(NASM) -f elf64 $< -o $@
	
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) 

-include %(OBJECTS:.o=.d)

.PHONY: clean
clean:
	rm -rf $(BINDIR)/$(KERNEL) $(ASM_OBJ) $(OBJECTS) $(ISO)
	rm -rf $(OBJECTS:.o=.d)

.PHONY: run
run: $(ISO)
	$(QEMU) -cdrom $< -serial file:./log/debug.log -d int --no-reboot
	
