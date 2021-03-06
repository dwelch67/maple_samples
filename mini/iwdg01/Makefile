
ARMGNU = arm-none-eabi

AOPS = --warn --fatal-warnings 
COPS = -Wall -Werror -O2 -nostdlib -nostartfiles -ffreestanding 

LOPS = -Wall -m32 -emit-llvm
LLCOPS = -march=thumb
LLCOPS2 = -march=thumb -mcpu=cortex-m3 
COPS = -Wall  -O2 -nostdlib -nostartfiles -ffreestanding
OOPS = -std-compile-opts

all : iwdg01.gcc.thumb.bin iwdg01.gcc.thumb2.bin iwdg01.clang.thumb.norm.bin iwdg01.clang.thumb.opt.bin iwdg01.clang.thumb2.norm.bin iwdg01.clang.thumb2.opt.bin

vectors.o : vectors.s
	$(ARMGNU)-as vectors.s -o vectors.o

iwdg01.gcc.thumb.o : iwdg01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -c iwdg01.c -o iwdg01.gcc.thumb.o

iwdg01.gcc.thumb2.o : iwdg01.c
	$(ARMGNU)-gcc $(COPS) -mthumb -mcpu=cortex-m3 -march=armv7-m -c iwdg01.c -o iwdg01.gcc.thumb2.o

iwdg01.gcc.thumb.bin : memmap vectors.o iwdg01.gcc.thumb.o
	$(ARMGNU)-ld -o iwdg01.gcc.thumb.elf -T memmap vectors.o iwdg01.gcc.thumb.o
	$(ARMGNU)-objdump -D iwdg01.gcc.thumb.elf > iwdg01.gcc.thumb.list
	$(ARMGNU)-objcopy iwdg01.gcc.thumb.elf iwdg01.gcc.thumb.bin -O binary

iwdg01.gcc.thumb2.bin : memmap vectors.o iwdg01.gcc.thumb2.o
	$(ARMGNU)-ld -o iwdg01.gcc.thumb2.elf -T memmap vectors.o iwdg01.gcc.thumb2.o
	$(ARMGNU)-objdump -D iwdg01.gcc.thumb2.elf > iwdg01.gcc.thumb2.list
	$(ARMGNU)-objcopy iwdg01.gcc.thumb2.elf iwdg01.gcc.thumb2.bin -O binary


iwdg01.clang.bc : iwdg01.c
	clang $(LOPS) -c iwdg01.c -o iwdg01.clang.bc

iwdg01.clang.thumb.norm.bin : memmap vectors.o iwdg01.clang.bc
	llc $(LLCOPS) iwdg01.clang.bc -o iwdg01.clang.thumb.norm.s
	$(ARMGNU)-as iwdg01.clang.thumb.norm.s -o iwdg01.clang.thumb.norm.o
	$(ARMGNU)-ld -o iwdg01.clang.thumb.norm.elf -T memmap vectors.o iwdg01.clang.thumb.norm.o
	$(ARMGNU)-objdump -D iwdg01.clang.thumb.norm.elf > iwdg01.clang.thumb.norm.list
	$(ARMGNU)-objcopy iwdg01.clang.thumb.norm.elf iwdg01.clang.thumb.norm.bin -O binary

iwdg01.clang.thumb.opt.bin : memmap vectors.o iwdg01.clang.bc
	opt $(OOPS) iwdg01.clang.bc -o iwdg01.clang.thumb.opt.bc
	llc $(LLCOPS) iwdg01.clang.thumb.opt.bc -o iwdg01.clang.thumb.opt.s
	$(ARMGNU)-as iwdg01.clang.thumb.opt.s -o iwdg01.clang.thumb.opt.o
	$(ARMGNU)-ld -o iwdg01.clang.thumb.opt.elf -T memmap vectors.o iwdg01.clang.thumb.opt.o
	$(ARMGNU)-objdump -D iwdg01.clang.thumb.opt.elf > iwdg01.clang.thumb.opt.list
	$(ARMGNU)-objcopy iwdg01.clang.thumb.opt.elf iwdg01.clang.thumb.opt.bin -O binary

iwdg01.clang.thumb2.norm.bin : memmap vectors.o iwdg01.clang.bc
	llc $(LLCOPS2) iwdg01.clang.bc -o iwdg01.clang.thumb2.norm.s
	$(ARMGNU)-as iwdg01.clang.thumb2.norm.s -o iwdg01.clang.thumb2.norm.o
	$(ARMGNU)-ld -o iwdg01.clang.thumb2.norm.elf -T memmap vectors.o iwdg01.clang.thumb2.norm.o
	$(ARMGNU)-objdump -D iwdg01.clang.thumb2.norm.elf > iwdg01.clang.thumb2.norm.list
	$(ARMGNU)-objcopy iwdg01.clang.thumb2.norm.elf iwdg01.clang.thumb2.norm.bin -O binary

iwdg01.clang.thumb2.opt.bin : memmap vectors.o iwdg01.clang.bc
	opt $(OOPS) iwdg01.clang.bc -o iwdg01.clang.thumb2.opt.bc
	llc $(LLCOPS2) iwdg01.clang.thumb2.opt.bc -o iwdg01.clang.thumb2.opt.s
	$(ARMGNU)-as iwdg01.clang.thumb2.opt.s -o iwdg01.clang.thumb2.opt.o
	$(ARMGNU)-ld -o iwdg01.clang.thumb2.opt.elf -T memmap vectors.o iwdg01.clang.thumb2.opt.o
	$(ARMGNU)-objdump -D iwdg01.clang.thumb2.opt.elf > iwdg01.clang.thumb2.opt.list
	$(ARMGNU)-objcopy iwdg01.clang.thumb2.opt.elf iwdg01.clang.thumb2.opt.bin -O binary







clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.bc
	rm -f *.opt.s
	rm -f *.norm.s


