8080 Emulator in C
===
Does not attempt any kind of cyclic emulation. Uses a pointer array for fast instruction decoding.

Currently implemented:
- Data ops
- Stack ops
- Others here and there
- Live disassembly/debugger

Tests:
- Some basic (mostly decode only) tests
- To build these you will need ZASM (https://github.com/Megatokio/zasm)
