# Welcome to hell
# I will try to document this to the best of my ability
CC=i686-elf-gcc
AS=i686-elf-as
LDARGS=-ffreestanding -O2 -nostdlib
CFLAGS= -ffreestanding -O2 -Wall -Wextra -Iinclude
# ODIR is going to be the directory storing all the .o files
# It is important to make sure NONE of the source files have the same name, i.e., kernel/foo.c and kernel/foobar/foo.c
# or even kernel/foo.c and kernel/foo.s cannot co-exist

ODIR=obj
# Here, we set out an recursive search for .c and .s files in the src folder
SOURCES := $(wildcard *.c) $(wildcard **/*.c)
ASM := $(wildcard *.s) $(wildcard **/*.s)
BOOT:=$(wildcard boot/*.asm)
# Here we remove the .c ending and .s ending and replace it with a .o ending and add the ODIR to it
OBJ := $(addprefix $(ODIR)/,$(notdir $(SOURCES:.c=.o))) $(addprefix $(ODIR)/,$(notdir $(ASM:.s=.o))) $(addprefix $(ODIR)/,$(notdir $(BOOT:.asm=.o)))
# Then we compile all the stuff, and I suck balls, cuz it still isn't recursive
$(ODIR)/%.o: $(SOURCES)
	for a in kernel/**/*.c; do \
	    $(CC) -c $$a -o $$a.o -std=gnu99 $(CFLAGS); \
	done
	for b in kernel/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done
	for b in kernel/assembly/*.s; do \
	    nasm -felf $$b -o $$b.o; \
	done
	$(AS) boot/start.asm -o start.o
	mv kernel/**/*.o $(ODIR)
	mv kernel/*.o $(ODIR)
	mv *.o $(ODIR)
LiquiDOS.bin: $(OBJ)
	cd obj && make
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
	rm -f kernel/**/.o
	rm -f kernel/*.o
	rm -f *.bin
