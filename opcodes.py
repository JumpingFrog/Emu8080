# Quick script to list opcodes for ease of life.

bits = raw_input("Enter opcode bits (x where register is): ")

lut = ["000", "001", "010", "011", "100", "101", "110", "111"]
reg_v = ["B", "C", "D", "E", "H", "L", "M", "A"]

for i in range(0,8):
	temp = bits.replace("xxx", lut[i])
	print reg_v[i] + ": " + temp + " = " + hex(int(temp, 2))