# Welcome to hell number 2
# I will not document this well...
#CC=i686-elf-gcc
#LDARGS=-ffreestanding -O2 -nostdlib
CC=i686-elf-ld
OBJ = $(wildcard *.o)
# Link 'em all up
LiquiDOS.bin: $(OBJ)
	@echo "\n\033[1;36m[Linking object files]\033[0m"
	@$(CC) -T linker.ld -o $@ $(LDARGS) $^
	@mv LiquiDOS.bin ../bin
	@echo "\033[1;32mBinary file produced!\033[0m\n"
