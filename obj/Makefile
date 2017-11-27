TARGET_FOLDER=x86
CC=i686-elf-gcc
LD=arch/$(TARGET_FOLDER)/linker.ld
LDARGS=-lgcc -nostdlib -ffreestanding
OBJ = $(wildcard *.o)

.PHONY: all
all: LiquiDOS.bin

# Link 'em all up
LiquiDOS.bin: $(OBJ)
	@echo "\n\033[1;36m[Linking object files]\033[0m"
	@$(CC) -T $(LD) -o $@ $^ $(LDARGS)
	@mv LiquiDOS.bin ../bin
ifeq ($(TARGET_FOLDER),raspi)
	@arm-none-eabi-objcopy ../bin/LiquiDOS.bin -O binary ../bin/kernel.img
endif
	@echo "\033[1;32mBinary file produced!\033[0m\n"
