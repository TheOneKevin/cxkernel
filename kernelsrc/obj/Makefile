# Welcome to hell number 2
# I will not document this well...
CC=i686-elf-gcc
LDARGS=-ffreestanding -O2 -nostdlib
OBJ = $(wildcard *.o)
# Link 'em all up
LiquiDOS.bin: $(OBJ)
	$(CC) -T../linker.ld -o $@ $(LDARGS) $^ -lgcc
	mv LiquiDOS.bin ../