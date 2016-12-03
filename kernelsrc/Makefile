# Welcome to hell
# I will try to document this to the best of my ability
CC=i686-elf-gcc
AS=i686-elf-as
#LDARGS=-ffreestanding -O2 -nostdlib
CFLAGS= -ffreestanding -O3 -Wall -Wextra -Iinclude -fdiagnostics-color=auto
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
# Here, I got lazy and used loops to loop through each directory and compile the code accordingly
# We recurse through the subdirectories of kernel
	for a in kernel/**/*.c; do \
	    $(CC) -c $$a -o $$a.o -std=gnu99 $(CFLAGS); \
	done
# We recurse through the root directory of kernel
	for b in kernel/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done
	
# We recurse through the root directory of mm
	for b in mm/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done
	
# We recurse through the root directory of tasking
	for b in tasking/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done
	
# We find all the asm files in the assembly folder
	for b in kernel/assembly/*.asm; do \
	    nasm -felf $$b -o $$b.o; \
	done
# We compile the multiboot header
	$(AS) boot/start.asm -o start.o
# Move all the .o files to $(ODIR)
	mv kernel/**/*.o $(ODIR)
	mv kernel/*.o $(ODIR)
	mv mm/*.o $(ODIR)
	mv tasking/*.o $(ODIR)
	mv *.o $(ODIR)
LiquiDOS.bin: $(OBJ) # Target for the binary
# Execute make in the object folder (see other Makefile)
	cd obj && make
# Make the GRUB iso
	echo Setting up GRUB...
	mkdir -p isodir/boot/grub
	cp LiquiDOS.bin isodir/boot/LiquiDOS.bin
	cp grub/grub.cfg isodir/boot/grub/grub.cfg
	cp grub/background.jpg isodir/boot/background.jpg
	echo Making GRUB iso...
	grub-mkrescue -o LiquiDOS.iso isodir
	echo Cleaning up current mess...
#rm -r isodir
.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
	rm -f kernel/**/.o
	rm -f kernel/*.o
	rm -f *.bin
	rm -f *.iso
	rm -f boot/*.o
	rm -r isodir
	echo Done!
