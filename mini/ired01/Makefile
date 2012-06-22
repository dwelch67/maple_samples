
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : ired01.gcc.thumb.bin ired01.gcc.thumb2.bin ired01.clang.thumb.norm.bin ired01.clang.thumb.opt.bin ired01.clang.thumb2.norm.bin ired01.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

ired01.gcc.thumb.o : ired01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c ired01.c -o ired01.gcc.thumb.o

ired01.gcc.thumb2.o : ired01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c ired01.c -o ired01.gcc.thumb2.o

ired01.gcc.thumb.bin : memmap vectors.o ired01.gcc.thumb.o
	$(ARMGNU)-ld -o ired01.gcc.thumb.elf -T memmap vectors.o ired01.gcc.thumb.o
	$(ARMGNU)-objdump -D ired01.gcc.thumb.elf > ired01.gcc.thumb.list
	$(ARMGNU)-objcopy ired01.gcc.thumb.elf ired01.gcc.thumb.bin -O binary

ired01.gcc.thumb2.bin : memmap vectors.o ired01.gcc.thumb2.o
	$(ARMGNU)-ld -o ired01.gcc.thumb2.elf -T memmap vectors.o ired01.gcc.thumb2.o
	$(ARMGNU)-objdump -D ired01.gcc.thumb2.elf > ired01.gcc.thumb2.list
	$(ARMGNU)-objcopy ired01.gcc.thumb2.elf ired01.gcc.thumb2.bin -O binary


ired01.clang.bc : ired01.c
	clang $(LOPS) -c ired01.c -o ired01.clang.bc

ired01.clang.thumb.norm.bin : memmap vectors.o ired01.clang.bc
	llc $(LLCOPS) ired01.clang.bc -o ired01.clang.thumb.norm.s
	$(ARMGNU)-as ired01.clang.thumb.norm.s -o ired01.clang.thumb.norm.o
	$(ARMGNU)-ld -o ired01.clang.thumb.norm.elf -T memmap vectors.o ired01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D ired01.clang.thumb.norm.elf > ired01.clang.thumb.norm.list
	$(ARMGNU)-objcopy ired01.clang.thumb.norm.elf ired01.clang.thumb.norm.bin -O binary

ired01.clang.thumb.opt.bin : memmap vectors.o ired01.clang.bc
	opt $(OOPS) ired01.clang.bc -o ired01.clang.thumb.opt.bc
	llc $(LLCOPS) ired01.clang.thumb.opt.bc -o ired01.clang.thumb.opt.s
	$(ARMGNU)-as ired01.clang.thumb.opt.s -o ired01.clang.thumb.opt.o
	$(ARMGNU)-ld -o ired01.clang.thumb.opt.elf -T memmap vectors.o ired01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D ired01.clang.thumb.opt.elf > ired01.clang.thumb.opt.list
	$(ARMGNU)-objcopy ired01.clang.thumb.opt.elf ired01.clang.thumb.opt.bin -O binary

ired01.clang.thumb2.norm.bin : memmap vectors.o ired01.clang.bc
	llc $(LLCOPS2) ired01.clang.bc -o ired01.clang.thumb2.norm.s
	$(ARMGNU)-as ired01.clang.thumb2.norm.s -o ired01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o ired01.clang.thumb2.norm.elf -T memmap vectors.o ired01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D ired01.clang.thumb2.norm.elf > ired01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy ired01.clang.thumb2.norm.elf ired01.clang.thumb2.norm.bin -O binary

ired01.clang.thumb2.opt.bin : memmap vectors.o ired01.clang.bc
	opt $(OOPS) ired01.clang.bc -o ired01.clang.thumb2.opt.bc
	llc $(LLCOPS2) ired01.clang.thumb2.opt.bc -o ired01.clang.thumb2.opt.s
	$(ARMGNU)-as ired01.clang.thumb2.opt.s -o ired01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o ired01.clang.thumb2.opt.elf -T memmap vectors.o ired01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D ired01.clang.thumb2.opt.elf > ired01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy ired01.clang.thumb2.opt.elf ired01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


