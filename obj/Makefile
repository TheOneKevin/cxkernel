# Welcome to hell number 2
# I will not document this well...
#CC=i686-elf-gcc
#LDARGS=-ffreestanding -O2 -nostdlib
CC=i686-elf-ld
OBJ = $(wildcard *.o)
# Link 'em all up
LiquiDOS.bin: $(OBJ)
	$(CC) -T ../kernelsrc/linker.ld -o $@ $(LDARGS) $^
	mv LiquiDOS.bin ../bin
