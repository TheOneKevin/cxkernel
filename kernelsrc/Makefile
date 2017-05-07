# Welcome to hell
# I will try to document this to the best of my ability
CC=i686-elf-gcc
AS=i686-elf-as
#LDARGS=-ffreestanding -O2 -nostdlib
CFLAGS= -ffreestanding -O3 -Wall -Wextra -Iinclude -fdiagnostics-color=auto -fstack-protector-strong
# ODIR is going to be the directory storing all the .o files
# It is important to make sure NONE of the source files have the same name, i.e., kernel/foo.c and kernel/foobar/foo.c
# or even kernel/foo.c and kernel/foo.s cannot co-exist
ODIR=../obj
BDIR=../bin

# Here, we set out an recursive search for .c and .s files in the src folder
SOURCES := $(wildcard *.c) $(wildcard **/*.c)
ASM := $(wildcard *.s) $(wildcard **/*.s)
BOOT:=$(wildcard boot/*.asm)
# Here we remove the .c ending and .s ending and replace it with a .o ending and add the ODIR to it
OBJ := $(addprefix $(ODIR)/,$(notdir $(SOURCES:.c=.o))) $(addprefix $(ODIR)/,$(notdir $(ASM:.s=.o))) $(addprefix $(ODIR)/,$(notdir $(BOOT:.asm=.o)))
# Then we compile all the stuff, and I suck balls, cuz it still isn't recursive
#$(ODIR)/%.o: $(SOURCES)
.PHONY: all
all:
# Here, I got lazy and used loops to loop through each directory and compile the code accordingly
# We iterate through the subdirectories of kernel
	for a in kernel/**/*.c; do \
	    $(CC) -c $$a -o $$a.o -std=gnu99 $(CFLAGS); \
	done
# We iterate through the root directory of kernel
	for b in kernel/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done

# We iterate through the root directory of mm
	for b in mm/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done

# We iterate through the root directory of tasking
	for b in tasking/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done

# We iterate through the root directory of fs
	for b in fs/*.c; do \
	    $(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done

	for b in fs/**/*.c; do \
		$(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done

# We iterate throught the root directory of gdb
	for b in gdb/*.c; do \
		$(CC) -c $$b -o $$b.o -std=gnu99 $(CFLAGS); \
	done

# We compile the CSPRNG (random number generator) and other security features without SSP
	for b in security/*.c; do \
		$(CC) -c $$b -o $$b.o -std=gnu99 -ffreestanding -O3 -Wall -Wextra -Iinclude -fdiagnostics-color=auto; \
	done

# We find all the asm files in the assembly folder
	for b in kernel/assembly/*.asm; do \
	    nasm -felf $$b -o $$b.o -w-number-overflow; \
	done

# We compile the multiboot header
	$(AS) boot/start.s -o start.o

# Move all the .o files to $(ODIR)
	mv kernel/**/*.o $(ODIR)
	mv kernel/*.o $(ODIR)
	mv mm/*.o $(ODIR)
	mv fs/*.o $(ODIR)
	mv fs/**/*.o $(ODIR)
	mv gdb/*.o $(ODIR)
	mv tasking/*.o $(ODIR)
	mv Security/*.o $(ODIR)
	mv *.o $(ODIR)

# Execute make in the object folder (see other Makefile)
	cd ../obj && make
# Make the initial ramdisk
	echo Making ram disk...
	cd grub/ramfs && make
# mv ramfs.tar grub/ramfs.tar
# Make the GRUB iso
	echo Setting up GRUB...
	mkdir -p isodir/boot/grub
	cp $(BDIR)/LiquiDOS.bin isodir/boot/LiquiDOS.bin
	cp grub/grub.cfg isodir/boot/grub/grub.cfg
	cp grub/background.jpg isodir/boot/background.jpg

	echo Adding InitRam FS...
	cp grub/ramfs/ramfs.tar isodir/boot/ramfs.tar

	echo Making GRUB iso...
	grub-mkrescue -o LiquiDOS.iso isodir
	mv LiquiDOS.iso $(BDIR)/LiquiDOS.iso

.PHONY: clean
clean:
	rm -f $(ODIR)/*.o
	rm -f $(BDIR)/*.bin
	rm -f $(BDIR)/*.o
	rm -f $(BDIR)/*.iso
	rm -f kernel/**/.o
	rm -f kernel/*.o
	rm -f mm/*.o
	rm -f tasking/*.o
	rm -f fs/*.o
	rm -f fs/**/*.o
	rm -f *.bin
	rm -f *.iso
	rm -f boot/*.o
	rm -rf isodir
	rm -f boot/*.tar
	rm -f gdb/*.o
	echo Done!
