#=====================================================================
# 	$@ is the target (produced file)
# 		%.bin: %.c my_header.h -> Matches %.bin
#
# 	$< is the first dependency
# 		%.bin: %.c my_header.h -> Matches %.c
#
#   $^ all dependencies
# 		%.bin: %.c my_header.h -> Matches %.c + my_header.h
#
#   $* is the stem
# 		%.bin: %.c my_header.h -> Matches % (if %.bin == path/to/myfile.bin -> matches "path/to/myfile")
#=====================================================================


#
#	This Makefile just creates executable binaries for eache test scripts
#   It assumes -for now- that all scripts are just a single C file located
#   in it's own directory
#

SRCS := $(wildcard */*.c)
BINS := $(SRCS:.c=.bin)	# Change all the */test_*.c into */test_*.bin

all: compile_boot $(BINS)
	@echo "All done !"

compile_boot:
	@echo "### Compiling boot assembly script..."
	riscv64-linux-gnu-gcc -march=rv32im -mabi=ilp32 -nostdlib -ffreestanding -c global/boot.s -o global/boot.o
	@echo "### Boot script compiled !\n"



%.o: %.c
	@echo "############    $<    ############"
	@echo "### Creating object file for $<..."
	riscv64-linux-gnu-gcc -march=rv32im -mabi=ilp32 -nostdlib -ffreestanding -c $< -o $@
	@echo "### $@ created !\n"

%.elf: %.o
	@echo "### Linking $< to boot script..."
	riscv64-linux-gnu-ld -m elf32lriscv -T global/linker.ld global/boot.o $< -o $@
	@echo "### Done linking !\n"

%.bin: %.elf
	@echo "### Generating $@ from $<..."
	riscv64-linux-gnu-objcopy -O binary $< $@
	riscv64-linux-gnu-objdump -d $<     > $*.info
	@echo "\n============================\n" >> $*.info
	riscv64-linux-gnu-objdump -t  $<   >> $*.info
	@echo "### $@ compiled !"
	@echo "####################################################\n\n"


clean:
	rm -f global/boot.o $(wildcard */*.o) $(wildcard */*.elf) $(BINS) $(wildcard */*.info)