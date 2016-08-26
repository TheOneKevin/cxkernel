# Welcome to hell
# I will try to document this to the best of my ability
CC=i686-elf-gcc
AS=i686-elf-as
LDARGS=-ffreestanding -O2 -nostdlib
CFLAGS= -ffreestanding -O2 -Wall -Wextra -Iinclude
OBJ = $(wildcard *.o)
LiquiDOS.bin: $(OBJ)
	$(CC) -T ../linker.ld -o $@ $(LDARGS) $^ -lgcc
	mv LiquiDOS.bin ../
.PHONY: clean
clean:
	rm -f *.o
