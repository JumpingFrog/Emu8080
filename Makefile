# Emulator Binary
CC = clang
CFLAGS = -Wall -Wextra -Wpedantic -O0 -Isrc/include

EMU_OBJ =  src/main.o src/groups/jump.o src/groups/special.o
EMU_OBJ += src/groups/arithmetic.o src/groups/logical.o
EMU_OBJ += src/groups/data.o src/groups/stack.o src/i8080.o
EMU_OBJ += src/devices/serial.o

EXECNAME=emu

# Test roms
AS = zasm
ASFLAGS = --asm8080 -uwy
TESTS = tests/arithmetic_tests.bin tests/data_tests.bin
TESTS += tests/jump_tests.bin tests/logical_tests.bin
TESTS += tests/logical_tests.bin tests/special_tests.bin
TESTS += tests/stack_tests.bin

# Rules
all: emu tests
	@echo "Build complete."
	
emu: $(EMU_OBJ)
	$(CC) $(CFLAGS) -o $(EXECNAME) $(EMU_OBJ)
	@echo "Emu8080 built."

%.bin: %.asm
	$(AS) $(ASFLAGS) $< $@

tests: $(TESTS)
	@echo "Emu8080 tests built."

.PHONY: clean
clean:
	rm -f $(EXECNAME) $(EMU_OBJ) $(TESTS)
	rm -f tests/*.lst
