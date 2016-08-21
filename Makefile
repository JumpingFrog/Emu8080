CFLAGS=-Wall -Wextra -Wpedantic -O0 -Isrc/include

OBJ =	src/main.o src/groups/jump.o src/groups/special.o
OBJ +=	src/groups/arithmetic.o src/groups/logical.o
OBJ +=	src/groups/data.o src/groups/stack.o src/i8080.o
OBJ +=	src/devices/serial.o

CC=clang

EXECNAME=emu

all: main
	@echo "Emu8080 built."

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXECNAME) $(OBJ)

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(EXECNAME)
	rm -f src/*.o
	rm -f src/groups/*.o
	rm -f src/devices/*.o
