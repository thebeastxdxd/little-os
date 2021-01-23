TARGET = myos.bin
TARGET_ISO = $(TARGET:%.bin=%.iso)
CC = i686-elf-gcc
ASM = i686-elf-as
LINKER = i686-elf-gcc
SHELL ::=/bin/bash

SRCDIR = src
BINDIR = bin
OBJDIR = obj
ISODIR = isodir

SOURCES := $(wildcard $(SRCDIR)/*.c)
ASM_SRC := $(wildcard $(SRCDIR)/*.s)
INCLUDES := $(wildcard $(SRCDIR)/*.h)
LD_SCRIPT := $(SRCDIR)/linker.ld
OBJECTS := $(SOURCES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
ASM_OBJ := $(ASM_SRC:$(SRCDIR)/%.s=$(OBJDIR)/%.o)

CFLAGS := -ffreestanding -O2 

.PHONY: all
all:  $(BINDIR)/$(TARGET)
	@echo multiboot state is $(shell grub-file --is-x86-multiboot $(BINDIR)/$(TARGET); echo $$?)
	cp $(BINDIR)/$(TARGET) $(ISODIR)/boot/$(TARGET)
	grub-mkrescue -o $(TARGET_ISO) $(ISODIR)

$(BINDIR)/$(TARGET): $(OBJECTS) $(ASM_OBJ) $(INCLUDES)
	$(LINKER) -T $(LD_SCRIPT) -o $@ $(CFLAGS) -nostdlib $^ -lgcc

$(ASM_OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.s
	$(ASM) $< -o $@
	
$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	$(CC) -c $< -o $@ $(CFLAGS) -std=gnu99 -Wall -Wextra

.PHONY: clean
clean:
	rm -rf $(BINDIR)/$(TARGET) $(ASM_OBJ) $(OBJECTS)
	
