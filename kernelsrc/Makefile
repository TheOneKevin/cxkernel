CC=i686-elf-gcc
AS=i686-elf-as
CFLAGS= -ffreestanding -O2 -Wall -Wextra -Iinclude -fdiagnostics-color=auto -fstack-protector-all -std=gnu99
ODIR=../obj/
BDIR=../bin/

SOURCES := $(shell find -name '*.c')
OBJECTS := $(SOURCES:%.c=%.o)
TARGET  := bin

all: $(TARGET)

%.o: %.c
	@echo "\033[1;32mBuilding [$^] \033[0m"
	@$(CC) -c $^ -o $(ODIR)$(notdir $@) $(CFLAGS)

$(TARGET): $(OBJECTS)
	@echo "\n\033[1;36m[Building assembly sources]\033[0m"
	@for b in kernel/assembly/*.asm; do \
		echo "\033[1;32mBuilding [$$b]\033[0m"; \
		nasm -felf $$b -o $$b.o -w-number-overflow; \
	done

	@echo "\n\033[1;36m[Building bootloader]\033[0m"
	@$(AS) boot/start.s -o boot/start.o

	@echo "Moving files around..."
	@mv kernel/assembly/*.o $(ODIR)
	@mv boot/*.o $(ODIR)

	@cd $(ODIR) && make --no-print-directory

	@echo "\033[1;36mMaking initrd...\033[0m\n"
	@cd grub/ramfs && make --no-print-directory

	@echo "\033[1;62mConfiguring GRUB...\033[0m"
	@mkdir -p isodir/boot/grub

	@echo "    -> \033[1;36mCopying files...\033[0m"
	@cp $(BDIR)/LiquiDOS.bin isodir/boot/LiquiDOS.bin
	@cp grub/grub.cfg isodir/boot/grub/grub.cfg
	@cp grub/background.jpg isodir/boot/background.jpg
	@cp grub/ramfs/ramfs.tar isodir/boot/ramfs.tar

	@echo "    -> \033[1;36mBuilding ISO...\033[0m"
	@grub-mkrescue -o LiquiDOS.iso isodir
	@mv LiquiDOS.iso $(BDIR)/LiquiDOS.iso

	@echo "\033[1;32m--Done!--\033[0m"
	@echo "Build is done. Go give yourself a pat on the back."
.PHONY: clean
clean:
	@echo "\033[1;36m[Cleaning all object files]\033[0m"
	@echo -n "\033[1;32mFiles deleted: "
	@rm -rvif $(ODIR)*.o | wc -l
	@echo "\033[0m"

	@echo "\033[1;36m[Removing directories]\033[0m"
	@rm -rf isodir
	@echo "\033[1;36m[Removing binaries]\033[0m"
	@rm -rf $(BDIR)*.bin $(BDIR)*.iso
