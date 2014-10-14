CFLAGS=-Wall -Wextra -Wpedantic -O0
OBJ=core/groups/jump_group.o core/groups/special_group.o core/i8080.o
CC=gcc
EXECNAME=bin/myapp

all: main
	@echo "Emu8080 built."

main: $(OBJ)
	$(CC) $(CFLAGS) -o $(EXECNAME) $(OBJ)

clean:
	rm -f *.out
	rm -f *.o
	rm -f $(EXECNAME)
	rm -f core/*.o
	rm -f core/group/*.o