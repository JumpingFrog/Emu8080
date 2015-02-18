CFLAGS=-Wall -Wextra -Wpedantic -O0 -Isrc/groups/include -Isrc/

OBJ =	src/main.o src/groups/jump_group.o src/groups/special_group.o
OBJ +=	src/groups/arithmetic_group.o src/groups/logical_group.o
OBJ +=	src/groups/data_group.o src/groups/stack_group.o src/i8080.o

CC=clang

EXECNAME=bin/emu

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
