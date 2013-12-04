package com.dwotherspoon.Emu8080;

public class Core8080 {
	/*----------------------------------------------------
	 * 8080 instruction interpreter. No warranty given.
	 * (C) David Wotherspoon 2013
	------------------------------------------------------*/
	
	private IODevice[] devices;
	private boolean halt; //halt flag
	private int pc; //program counter
	private byte[] regs; //A-E,H,L
	private byte flags = 0x02; //harcoded 1
	private byte bytes_left = 0;
	private short cur_opp = 0;
	private int sp = 65535;
	private byte[] memory;
	private byte temp; //use for 2 operand instructions.
	private int res;
	
	
	public Core8080(byte[] memory) {	
		devices = new IODevice[256];
		halt = false;
		pc = 0;
		regs = new byte[7];
		this.memory = memory;
	}
	
	public void run() {
		/*
		for (int i = 0; i < 65535; i++) {
			System.out.printf("%d - 0x%02x\n", i, memory[i]);
		}*/
		while (!halt) {
			//printDebug();
			execute();	
			//System.out.println(Integer.toHexString(pc));
		}
		System.out.println("\nSystem was halted!");
		printDebug();
	}
	
	private void printDebug() {
		System.out.printf("PC: 0x%x  [PC]: 0x%02x  [PC+1]: 0x%02x\n", pc, memory[pc], memory[pc+1]);
		System.out.printf("A: 0x%02x\t F: 0x%02x\n", regs[0], flags);
		System.out.printf("B: 0x%02x\t C: 0x%02x\n", regs[1], regs[2]);
		System.out.printf("D: 0x%02x\t E: 0x%02x\n", regs[3], regs[4]);
		System.out.printf("H: 0x%02x\t L: 0x%02x\n", regs[5], regs[6]);
		System.out.printf("SP: 0x%x [SP]: 0x%02x [SP+1]: 0x%02x\n", sp, memory[sp], memory[(sp == 65535) ? 0 : (sp+1)]);
		System.out.printf("Sign: %b\t Zero: %b\t AC: %b\t Parity:%b\t Carry:%b\n", getSFlag(), getZFlag(), getACFlag(), getPFlag(), getCYFlag());
		System.out.println("---------------------------------------------------");
	}
	
	
	/** Start Flags - Sign **/
	private void setSFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x80); }
		else {
			flags = (byte)(flags & 0x7F); }
	}
	
	private boolean getSFlag() { 
		return ((flags & 0x80) != 0);
	}
	//Zero
	private void setZFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x40); }
		else {
			flags = (byte)(flags & 0xBF); }
	}
	
	private boolean getZFlag() {
		return ((flags & 0x40) != 0);
	}
	//Auxiliary Carry
	private void setACFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x10); }
		else {
			flags = (byte)(flags & 0xEF); }
	}
	
	private boolean getACFlag() {
		return ((flags & 0x10) != 0);
	}
	//Parity
	private void setPFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x04); }
		else {
			flags = (byte)(flags & 0xFB); }
	}
	
	private boolean getPFlag() {
		return ((flags & 0x04) != 0);
	}
	
	private void genPFlag() { //parity
		int count = 0;
		int temp = 0xFF&regs[0];
		for (int bit  = 0; bit < 8; bit++) {
			if ((temp & 0x01) == 1) {
				count++;
			}
			temp = temp >> 1;
		}
		setPFlag((count % 2) == 0);	
	}
	
	private void genPFlag(byte in) { //parity
		int count = 0;
		int temp = 0xFF&in;
		for (int bit  = 0; bit < 8; bit++) {
			if ((temp & 0x01) == 1) {
				count++;
			}
			temp = temp >> 1;
		}
		setPFlag((count % 2) == 0);	
	}
	//Carry
	private void setCYFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x01); }
		else {
			flags = (byte)(flags & 0xFE); }
	}
	
	private boolean getCYFlag() {
		return ((flags & 0x01) != 0);
	}
	
	/** End Flags **/
	
	private void incPC() {
		if (pc < 65535) {
			pc++; }
		else {
			pc = 0; }
	}
	
	public void addDevice(IODevice dev) throws PortAlreadyUsedException { //add a device to the CPU instance.
		for (int port = 0; port < dev.getPorts().length; port++) {
			if (devices[dev.getPorts()[port]] == null) {
				devices[dev.getPorts()[port]] = dev;
			}
			else {
				throw new PortAlreadyUsedException("Error: A device is already using this port!");
			}
		}
	}
	
	private int getHL() {
		return ((regs[5] & 0xFF) << 8)+(regs[6] & 0xFF);
	}
	//Helper functions
	private int bytesToInt(byte high, byte low) {
		return ((high & 0xFF) << 8) + (low & 0xFF);
	}
	
	private void addACC(byte in) { //add a single byte to the acc and generate flags
		int res = (int)(0xFF&regs[0]) + (int)(0xFF&in);
		setCYFlag(res > 255); //More than 255? CARRY YO 
		setACFlag(((regs[0]&0x0F) + (in&0x0F)) > 0x0F); //First 4 bits summed result in half carry?
		setZFlag((0xFF&res) == 0);
		setSFlag((res&0x80) > 0);
		regs[0] = (byte) (0xFF&res);
		genPFlag();
	}
	
	private void adcACC (byte in) { //add + in + carry and gen flags
		int res = (int)(0xFF&regs[0]) + (int)(0xFF&in) + (getCYFlag() ? 1 : 0);
		setCYFlag(res > 255); //More than 255? CARRY YO 
		setACFlag(((regs[0]&0x0F) + (in&0x0F)) > 0x0F); //First 4 bits summed result in half carry? (GNUSim 8085 doesn't include carry in half carry gen so I don't)
		setZFlag((0xFF&res) == 0);
		setSFlag((res&0x80) > 0);
		regs[0] = (byte) (0xFF&res);
		genPFlag();
	}
	
	private void subACC(byte in) { //sub a byte
		in = (byte) ((~in) + 1); //twos complement
		int res = (regs[0] + (0xFF&in));
		setCYFlag((byte)res < 0); //If result is more than 255, clear the borrow flag
		setACFlag(((regs[0]&0x0F) + (in&0x0F)) > 0x0F); //First 4 bits summed result in half carry? (GNUSim8085 doesn't do this)
		setZFlag((0xFF&res) == 0);
		setSFlag((res&0x80) > 0);
		regs[0] = (byte) (0xFF&res);
		genPFlag();
	}
	
	private void sbbACC(byte in) { //sub a byte with borrow
		in = (byte) ((in&0xFF) + (getCYFlag() ? 1 : 0));
		in = (byte) ((~in) + 1); //twos complement
		int res = (regs[0] + (0xFF&in));
		setCYFlag((byte)res < 0); //If result is more than 255, clear the borrow flag
		setACFlag(((regs[0]&0x0F) + (in&0x0F)) > 0x0F); //First 4 bits summed result in half carry? (GNUSim8085 doesn't do this)
		setZFlag((0xFF&res) == 0);
		setSFlag((res&0x80) > 0);
		regs[0] = (byte) (0xFF&res);
		genPFlag();
	}
	//Stack
	private void push(byte h, byte l) {
		sp = (sp == 0) ? 0xFFFF : sp - 1;
		memory[sp] = h;
		sp = (sp == 0) ? 0xFFFF : sp - 1;
		memory[sp] = l;
	}
	
	private int pop() {
		int res = (0xFF & memory[sp]);
		sp = (sp == 0xFFFF) ? 0 : sp + 1;
		res |= (0xFF & memory[sp]) << 8;
		sp = (sp == 0xFFFF) ? 0 : sp + 1;
		return res;
	}
	
	private void execute() { //execute loop
		if (bytes_left == 0) {
			cur_opp = (short)(memory[pc] & 0xFF); 
		}
		switch (cur_opp) {
		/** Start Control **/
			case 0x00: //NOP
				incPC();
				break;
			case 0x37: //STC
				setCYFlag(true);
				incPC();
				break;
			case 0x3F: //CMC
				setCYFlag(!getCYFlag());
				incPC();
				break;
			case 0x76: //HALT
				halt = true;
				break;
			case 0xF3: //DI NOT IMPLEMENTED
				incPC();
				break;
			case 0xFB: //EI NOT IMPLEMENTED
				incPC();
				break;
		/** End Control - Start Data Transfer  **/
			case 0x01: //LXI B
				if (bytes_left == 1) {
					bytes_left--;
					regs[1] = memory[pc]; //B is Most sig last bit read.
					regs[2] = temp; //C
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
				}
				else {
					bytes_left = 2;
				}
				incPC();
				break;
			case 0x02: //STAX B
				memory[bytesToInt(regs[1], regs[2])] = regs[0];
				incPC();
				break;
			case 0x0A: //LDAX B
				regs[0] = memory[bytesToInt(regs[1],regs[2])];
				incPC();
				break;
			case 0x11: //LXI D
				if (bytes_left == 1) {
					bytes_left--;
					regs[3] = memory[pc]; //D is Most sig
					regs[4] = temp; //E
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
				}
				else {
					bytes_left = 2;
				}
				incPC();
				break;
			case 0x12: //STAX D
				memory[bytesToInt(regs[3], regs[4])] = regs[0];
				incPC();
				break;
			case 0x1A: //LDAX D
				regs[0] = memory[bytesToInt(regs[3],regs[4])];
				incPC();
				break;
			case 0x21: //LXI H
				if (bytes_left == 1) {
					bytes_left--;
					regs[5] = memory[pc]; //H is Most sig
					regs[6] = temp; //L
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
				}
				else {
					bytes_left = 2;
				}
				incPC();
				break;
			case 0x22: //SHLD
				if (bytes_left == 1) {
					bytes_left--;
					int base = bytesToInt(memory[pc], temp);
					memory[base] = regs[6]; //Store LOW
					memory[base + 1] = regs[5]; //Store high
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
				}
				else {
					bytes_left = 2;
				}
				incPC();
				break;
			case 0x2A: //LHLD
				if (bytes_left == 1) {
					bytes_left--;
					int base = bytesToInt(memory[pc], temp);
					regs[6] = memory[base];
					regs[5] = memory[base + 1]; //load high
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
				}
				else {
					bytes_left = 2;
				}
				incPC();
				break;
			case 0x32: //STA
				if (bytes_left == 1) {
					bytes_left--;
					memory[bytesToInt(memory[pc], temp)] = regs[0];
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
				}
				else {
					bytes_left = 2;
				}
				incPC();
				break;
			case 0x36: //MVI M
				if (bytes_left == 1) {
					bytes_left--;
					memory[getHL()] = memory[pc];
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0x3A: //LDA
				if (bytes_left == 1) {
					bytes_left--;
					regs[0] = memory[bytesToInt(memory[pc], temp)];
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
				}
				else {
					bytes_left = 2;
				}
				incPC();
				break;
			case 0xD3: //OUT
				if (bytes_left == 1) {
					bytes_left--;
					if (devices[memory[pc]] == null) {
					}
					else {	
					devices[memory[pc]].in(regs[0]);
					}
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xDB: //IN
				if (bytes_left == 1) {
					bytes_left--;
					if (devices[memory[pc]] == null) {
						regs[0] = 0x00;
					}
					else {
					regs[0] = devices[memory[pc]].out();
					}
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xE3: //XTHL (Best guess on functionality - rap around to 0 on SP= 0xFFFF)
				if (sp == 65535) {
					temp = memory[0]; //sp+1
					memory[0] = regs[5]; //H->SP+1
					regs[5] = temp; //SP+1->H
					temp = memory[65535];//SP
					memory[65535] = regs[6]; //L->SP
					regs[6] = temp; //SP->L
				}
				else {
					temp = memory[sp+1]; //sp+1
					memory[sp+1] = regs[5]; //H->SP+1
					regs[5] = temp; //SP+1->H
					temp = memory[sp];//SP
					memory[sp] = regs[6]; //L->SP
					regs[6] = temp; //SP->L
				}
				incPC();
				break;
			case 0xEB: //XCHG
				temp = regs[3]; //D
				regs[3] = regs[5];//H->D
				regs[5] = temp; //D->H
				temp = regs[4]; //E
				regs[4] = regs[6]; //L->E
				regs[6] = temp; //E->L
				incPC();
				break;
			case 0xF9: //SPHL
				sp = bytesToInt(regs[5], regs[6]);
				incPC();
				break;
			/** End Data Transfer - Start Arithmetic  **/
			case 0x09: //DAD B (B&C + H&L)
				res = bytesToInt(regs[1], regs[2]) + bytesToInt(regs[5], regs[6]);//16 bit add
				setCYFlag(res > 0xFFFF); //set the carry flag
				regs[6] = (byte) (res & 0xFF); //lower 8 into L
				regs[5] = (byte) ((res >> 8) & 0xFF); //upper 8 into H
				incPC();
				break;
			case 0x19: //DAD D (D&E + H&L)
				res = bytesToInt(regs[3], regs[4]) + bytesToInt(regs[5], regs[6]);//16 bit add
				setCYFlag(res > 0xFFFF); //set the carry flag
				regs[6] = (byte) (res & 0xFF); //lower 8 into L
				regs[5] = (byte) ((res >> 8) & 0xFF); //upper 8 into H
				incPC();
				break;
			case 0x27: //DAA
				if (((regs[0] & 0x0F) > 9) || getACFlag()) { //LS Nibble > 9? OR AC
					setACFlag(((regs[0] & 0x0F) + 0x06) > 0x0F); //carry out from +6?
					regs[0] = (byte) ((regs[0] & 0xFF) + 0x06);
				}
				
				if (((regs[0] & 0xF0) > 0x90) || getCYFlag()) { //MS Nibble > 9 or CY
					setCYFlag(((regs[0] & 0xFF) + 0x60) > 0xFF); //Carry out?
					regs[0] = (byte) ((regs[0] & 0xFF) + 0x60);
				}
				incPC();
				break;
			case 0x29: //DAD H (H&L * 2)
				res = bytesToInt(regs[5], regs[6]) + bytesToInt(regs[5], regs[6]);//16 bit add
				setCYFlag(res > 0xFFFF); //set the carry flag
				regs[6] = (byte) (res & 0xFF); //lower 8 into L
				regs[5] = (byte) ((res >> 8) & 0xFF); //upper 8 into H
				incPC();
				break;
			case 0x39: //DAD SP (SP + H&L)
				res = sp + bytesToInt(regs[5], regs[6]);//16 bit add
				setCYFlag(res > 0xFFFF); //set the carry flag
				regs[6] = (byte) (res & 0xFF); //lower 8 into L
				regs[5] = (byte) ((res >> 8) & 0xFF); //upper 8 into H
				incPC();
				break;
			case 0xC6: //ADI
				if (bytes_left == 1) {
					bytes_left--;
					addACC(memory[pc]);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xCE: //ACI
				if (bytes_left == 1) {
					bytes_left--;
					adcACC(memory[pc]);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xD6: //SUI
				if (bytes_left == 1) {
					bytes_left--;
					subACC(memory[pc]);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xDE: //SBI
				if (bytes_left == 1) {
					bytes_left--;
					sbbACC(memory[pc]);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			/** End Arithmetic - Start Branch (JUMPS)  **/
			case 0xC2: //JNZ
				if (bytes_left == 1) {
					bytes_left--;
					if (getZFlag()) {
						incPC();
					}
					else {
						pc = bytesToInt(memory[pc], temp);
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xC3: //JMP
				if (bytes_left == 1) {
					bytes_left--;
					pc = bytesToInt(memory[pc], temp);
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xCA: //JZ
				if (bytes_left == 1) {
					bytes_left--;
					if (getZFlag()) {
						pc = bytesToInt(memory[pc], temp);
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xD2: //JNC
				if (bytes_left == 1) {
					bytes_left--;
					if (getCYFlag()) {
						incPC();
					}
					else {
						pc = bytesToInt(memory[pc], temp);
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xDA: //JC
				if (bytes_left == 1) {
					bytes_left--;
					if (getCYFlag()) {
						pc = bytesToInt(memory[pc], temp);
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xE2: //JPO
				if (bytes_left == 1) {
					bytes_left--;
					if (getPFlag()) {
						incPC();
					}
					else {
						pc = bytesToInt(memory[pc], temp);
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xE9: //PCHL
				pc = getHL();
				break;
			case 0xEA: //JPE
				if (bytes_left == 1) {
					bytes_left--;
					if (getPFlag()) {
						pc = bytesToInt(memory[pc], temp);
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xF2: //JP
				if (bytes_left == 1) {
					bytes_left--;
					if (getSFlag()) {
						incPC();
					}
					else {
						pc = bytesToInt(memory[pc], temp);
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xFA: //JM
				if (bytes_left == 1) {
					bytes_left--;
					if (getSFlag()) {
						pc = bytesToInt(memory[pc], temp);
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			/** End Branch (JUMPS) - Start Branch (CALLS)  **/
			case 0xCC: //CZ
				if (bytes_left == 1) {
					bytes_left--;
					if (getZFlag()) {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xC4: //CNZ
				if (bytes_left == 1) {
					bytes_left--;
					if (getZFlag()) {
						incPC();
					}
					else {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xCD: //CALL
				if (bytes_left == 1) {
					bytes_left--;
					res = bytesToInt(memory[pc], temp);
					incPC(); //increment so return will be following instruction
					push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
					pc = res;
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xDC: //CC
				if (bytes_left == 1) {
					bytes_left--;
					if (getCYFlag()) {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xD4: //CNC
				if (bytes_left == 1) {
					bytes_left--;
					if (getCYFlag()) {
						incPC();
					}
					else {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xE4: //CPO
				if (bytes_left == 1) {
					bytes_left--;
					if (getPFlag()) {
						incPC();
					}
					else {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xEC: //CPE
				if (bytes_left == 1) {
					bytes_left--;
					if (getPFlag()) {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xF4: //CP
				if (bytes_left == 1) {
					bytes_left--;
					if (getSFlag()) {
						incPC();
					}
					else {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			case 0xFC: //CM
				if (bytes_left == 1) {
					bytes_left--;
					if (getSFlag()) {
						res = bytesToInt(memory[pc], temp);
						incPC(); //increment so return will be following instruction
						push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
						pc = res;
					}
					else {
						incPC();
					}
				}
				else if (bytes_left == 2) {
					bytes_left--;
					temp = memory[pc];
					incPC();
				}
				else {
					bytes_left = 2;
					incPC();
				}
				break;
			/** End Branch (CALLS) - Start Branch (RETS)  **/
			case 0xC9: //RET
				pc = pop();
				break;
			case 0xD0: //RNC
				if (getCYFlag()) {
					incPC();
				}
				else {
					pc = pop();
				}
				break;
			case 0xD8: //RC
				if (getCYFlag()) {
					pc = pop();
				}
				else {
					incPC();
				}
				break;
			case 0xC0: //RNZ
				if (getZFlag()) {
					incPC();
				}
				else {
					pc = pop();
				}
				break;
			case 0xC8: //RZ
				if (getZFlag()) {
					pc = pop();
				}
				else {
					incPC();
				}
				break;
			case 0xF0: //RP
				if (getSFlag()) {
					incPC();
				}
				else {
					pc = pop();
				}
				break;
			case 0xF8: //RM
				if (getSFlag()) {
					pc = pop();
				}
				else {
					incPC();
				}
				break;
			case 0xE0: //RPO
				if (getPFlag()) {
					incPC();
				}
				else {
					pc = pop();
				}
				break;
			case 0xE8: //RPE
				if (getPFlag()) {
					pc = pop();
				}
				else {
					incPC();
				}
				break;
			/** End Branch - Start Logical  **/
			case 0x07: //RLC
				setCYFlag((regs[0]&0x80) > 0); //copy D7 into CY
				regs[0] <<= 1;
				regs[0] |= (getCYFlag() ? 1 : 0); //OR in D7/CY to D0
				incPC();
				break;
			case 0x0F: //RRC
				setCYFlag((regs[0] & 0x01) > 0); //store D0 in CY
				regs[0] >>= 1;
				regs[0] |= (getCYFlag() ? 0x80 : 0);
				incPC();
				break;
			case 0x17: //RAL
				temp = (byte) (getCYFlag() ? 1 : 0); //store CY
				setCYFlag((regs[0] &0x80) > 0); //put D7 in CY
				regs[0] <<= 1;
				regs[0]|= temp; //copy CY in D0
				incPC();
				break;
			case 0x1F: //RAR
				temp = (byte) (getCYFlag() ? 0x80 : 0); //Store CY
				setCYFlag((regs[0] & 0x01) > 0); //put D0 in CY
				regs[0] >>= 1;
				regs[0] |= temp; //copy CY in HIGH
				incPC();
				break;
			case 0x2F: //CMA
				regs[0] = (byte) ~regs[0];
				incPC();
				break;
			case 0xE6: //ANI
				if (bytes_left == 1) {
					bytes_left--;
					regs[0] &= memory[pc];
					setCYFlag(false);
					setACFlag(false);
					genPFlag();
					setZFlag(regs[0] == 0);
					setSFlag((regs[0] & 0x80) > 0);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xEE: //XRI
				if (bytes_left == 1) {
					bytes_left--;
					regs[0] ^= memory[pc];
					setCYFlag(false);
					setACFlag(false);
					genPFlag();
					setZFlag(regs[0] == 0);
					setSFlag((regs[0] & 0x80) > 0);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xF6: //ORI
				if (bytes_left == 1) {
					bytes_left--;
					regs[0] |= memory[pc];
					setCYFlag(false);
					setACFlag(false);
					genPFlag();
					setZFlag(regs[0] == 0);
					setSFlag((regs[0] & 0x80) > 0);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xFE: //CPI
				if (bytes_left == 1) {
					bytes_left--;
					temp = regs[0];
					subACC(memory[pc]);
					regs[0] = temp;
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			/** End Logical - Start Increment  **/
			case 0x03: //INX B
				res = bytesToInt(regs[1], regs[2]) + 1;
				regs[2] = (byte) (res & 0xFF);
				regs[1] = (byte) ((res >> 8) & 0xFF);
				incPC();
				break;
			case 0x0B: //DCX B
				res = bytesToInt(regs[1], regs[2]) - 1;
				regs[2] = (byte) (res & 0xFF);
				regs[1] = (byte) ((res >> 8) & 0xFF);
				incPC();
				break;
			case 0x13: //INX D
				res = bytesToInt(regs[3], regs[4]) + 1;
				regs[4] = (byte) (res & 0xFF);
				regs[3] = (byte) ((res >> 8) & 0xFF);
				incPC();
				break;
			case 0x1B: //DCX D
				res = bytesToInt(regs[3], regs[4]) - 1;
				regs[4] = (byte) (res & 0xFF);
				regs[3] = (byte) ((res >> 8) & 0xFF);
				incPC();
				break;
			case 0x23: //INX H
				res = bytesToInt(regs[5], regs[6]) + 1;
				regs[6] = (byte) (res & 0xFF);
				regs[5] = (byte) ((res >> 8) & 0xFF);
				incPC();
				break;
			case 0x2B: //DCX H
				res = bytesToInt(regs[5], regs[6]) - 1;
				regs[6] = (byte) (res & 0xFF);
				regs[5] = (byte) ((res >> 8) & 0xFF);
				incPC();
				break;
			case 0x33: //INX SP
				sp = (sp == 0xFFFF) ? 0 : sp + 1;
				incPC();
				break;
			case 0x3B: //DCX SP
				sp = (sp == 0) ? 0xFFFF : sp - 1;
				incPC();
				break;
			/** End Increment - Start Stack  **/
			case 0xC1: //POP B
				res = pop();
				regs[1] = (byte) ((res & 0xFF00) >> 8);
				regs[2] = (byte) (res & 0xFF);
 				incPC();
				break;
			case 0xC5: //PUSH B
				push(regs[1], regs[2]);
				incPC();
				break;
			case 0xD1: //POP D
				res = pop();
				regs[3] = (byte) ((res & 0xFF00) >> 8);
				regs[4] = (byte) (res & 0xFF);
 				incPC();
				break;
			case 0xD5: //PUSH D
				push(regs[3], regs[4]);
				incPC();
				break;
			case 0xE1: //POP H
				res = pop();
				regs[5] = (byte) ((res & 0xFF00) >> 8);
				regs[6] = (byte) (res & 0xFF);
 				incPC();
				break;
			case 0xE5: //PUSH H
				push(regs[5], regs[6]);
				incPC();
				break;
			case 0xF1: //POP PSW
				res = pop();
				regs[0] = (byte) ((res & 0xFF00) >> 8);
				flags = (byte) (res & 0xFF);
 				incPC();
				break;
			case 0xF5: //PUSH PSW
				push(regs[0], flags);
				incPC();
				break;
			default: //instructions with operand in opcode
				if ((cur_opp & 0xC7) == 0x06) { //MVI r/m group
					if (bytes_left == 1) {
						bytes_left--;
						temp = regConv((cur_opp&0x38)>>3); //Separate out reg from op
						if (temp == 7) { //M
							memory[bytesToInt(regs[5],regs[6])] = memory[pc];
						}
						else { //r
							regs[temp] = memory[pc];
						}
					}
					else {
						bytes_left = 1;
					}
				}
				else if ((cur_opp & 0xC0) == 0x40) { //MOV r/m group
					temp =regConv((cur_opp&0x38)>>3); //dest
					if (temp == 7) { //dest is M
						memory[bytesToInt(regs[5],regs[6])] = regs[regConv(cur_opp&0x07)]; //mov m,m would have been caught by halt
					}
					else {
						regs[temp] = (regConv(cur_opp&0x07) == 7)  ?  memory[bytesToInt(regs[5],regs[6])] : regs[regConv(cur_opp&0x07)];
					}
				}
				else if ((cur_opp & 0xF8) == 0x80) { //ADD r/m group
					temp = regConv((cur_opp&0x07));
					addACC( (temp == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[temp]);
				}
				else if ((cur_opp & 0xF8) == 0x88) {// ADC r/m group
					temp = regConv((cur_opp&0x07));
					adcACC( (temp == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[temp]);
				}
				else if ((cur_opp & 0xF8) == 0x90) { //SUB r/m group
					temp = regConv((cur_opp&0x07));
					subACC( (temp == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[temp]);
				}
				else if ((cur_opp & 0xF8) == 0x91) { //SBB r/m group
					temp = regConv((cur_opp&0x07));
					sbbACC( (temp == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[temp]);
				}
				else if ((cur_opp & 0xF8) == 0xA0) { //ANA r/m group
					temp = regConv(cur_opp & 0x07);
					regs[0] &= (temp == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[temp];
					setCYFlag(false);
					setACFlag(false);
					genPFlag();
					setZFlag(regs[0] == 0);
					setSFlag((regs[0] & 0x80) > 0);
				}
				else if ((cur_opp &  0xF8) == 0xA1) { //XRA r/m group
					temp = regConv(cur_opp & 0x07);
					regs[0] ^= (temp == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[temp];
					setCYFlag(false);
					setACFlag(false);
					genPFlag();
					setZFlag(regs[0] == 0);
					setSFlag((regs[0] & 0x80) > 0);					
				}
				else if ((cur_opp & 0xF8) == 0xB0) { //ORA r/m group
					temp = regConv(cur_opp & 0x07);
					regs[0] |= (temp == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[temp];
					setCYFlag(false);
					setACFlag(false);
					genPFlag();
					setZFlag(regs[0] == 0);
					setSFlag((regs[0] & 0x80) > 0);
				}
				else if ((cur_opp & 0xF8) == 0xB1) { //CMP r/m group
					temp = regs[0];
					subACC((regConv(cur_opp & 0x07) == 7) ? memory[bytesToInt(regs[5],regs[6])] : regs[regConv(cur_opp * 0x07)]);
					regs[0] = temp;
				}
				else if ((cur_opp & 0xC7) == 0x04) { //INR r/m group
					temp = regConv((cur_opp & 0x38)>>3);
					if (temp == 7) {
						memory[getHL()] = (byte) ((memory[getHL()] & 0xFF) + 1);
						setZFlag(memory[getHL()] == 0);
						genPFlag(memory[getHL()]);
						setSFlag((memory[getHL()]&0x80) > 0);
						setACFlag((memory[getHL()] & 0x0F) == 0x00); //only condition for CY up 0x0F->0xA0
					}
					else {
						regs[temp] = (byte) ((regs[temp] & 0xFF) + 1);
						setZFlag(regs[temp] == 0);
						genPFlag(regs[temp]);
						setSFlag((regs[temp]&0x80) > 0);
						setACFlag((regs[temp]&0x0F) == 0x00); //only condition for CY up 0x0F->0xA0
					}
				}
				else if ((cur_opp & 0xC7) == 0x05) { //DCR r/m group
					temp = regConv((cur_opp & 0x38)>>3);
					if (temp == 7) {
						memory[getHL()]--;
						setZFlag(memory[getHL()] == 0);
						genPFlag(memory[getHL()]);
						setSFlag((memory[getHL()]&0x80) > 0);
						//setACFlag((memory[getHL()] & 0x0F) == 0x00); //only condition for CY up 0x0F->0xA0
					}
					else {
						regs[temp]--;
						setZFlag(regs[temp] == 0);
						genPFlag(regs[temp]);
						setSFlag((regs[temp]&0x80) > 0);
						//setACFlag((regs[temp]&0x0F) == 0x00); //only condition for CY up 0x0F->0xA0
						//TODO: AC Flag
					}
				}
				else if ((cur_opp & 0xC7) == 0xC7) { //RST v group
					temp = (byte) ((cur_opp&0x38) >> 3);
					incPC();
					push((byte) ((pc>>8)&0xFF), (byte) (pc & 0xFF));
					pc = (temp * 8);
					break;
				}
				incPC();
				break;
		}
	}

byte regConv(int in) { //converts between my register scheme and the opcode assembly scheme
	switch (in) {
		case 0: //B
			return 1;
		case 1: //C
			return 2;
		case 2: //D
			return 3;
		case 3: //E
			return 4;
		case 4: //H
			return 5;
		case 5: //L
			return 6;
		case 6: //M
			return 7;
		case 7: //A
			return 0;
	}
	return 0;
}

}
