# Quick script to list opcodes for ease of life.

bits = raw_input("Enter opcode bits (x, y where register is): ")

lut = ["000", "001", "010", "011", "100", "101", "110", "111"]
reg_v = ["B", "C", "D", "E", "H", "L", "M", "A"]

#Test if we have two register fields or just one
if (bits.count('y') == 3):
	for i in range(0,8):
		for j in range(0, 8):
			temp = bits.replace("xxx", lut[i])
			temp = temp.replace("yyy", lut[j])
			print "x=" + reg_v[i] + " y=" + reg_v[j] + ": " + temp + " = " + hex(int(temp, 2))
else:
	for i in range(0,8):
		temp = bits.replace("xxx", lut[i])
		print reg_v[i] + ": " + temp + " = " + hex(int(temp, 2))