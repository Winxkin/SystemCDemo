NAME=hello

TOOLCHAIN_PREFIX=riscv32-unknown-elf

CC=$(TOOLCHAIN_PREFIX)-gcc
OC=$(TOOLCHAIN_PREFIX)-objcopy

all:
	$(CC) -nostdinc -nostdlib -O2 -I. -c start.s -o start.o
	$(CC) -nostdinc -nostdlib -O2 -I. -c main.c -o main.o
	$(CC) -nostdinc -nostdlib -O2 -Xlinker -Map=output.map -I. start.o main.o -o $(NAME).elf -Wl,-Tlinker.ld
	$(OC) -Obinary $(NAME).elf $(NAME).bin

clean:
	rm -f *.o *.elf *.bin