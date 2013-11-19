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
	private byte flags = 2;
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
		for (int i = 0; i < 65535; i++) {
			System.out.printf("%d - 0x%02x\n", i, memory[i]);
		}
		while (!halt) {
			printDebug();
			execute();	
			//System.out.println(Integer.toHexString(pc));
		}
		System.out.println("\nSystem was halted!");
	}
	
	private void printDebug() {
		System.out.printf("PC: 0x%x  [PC]: 0x%02x  [PC+1]: 0x%02x\n", pc, memory[pc], memory[pc+1]);
		System.out.printf("A: 0x%02x\t F: 0x%02x\n", regs[0], flags);
		System.out.printf("B: 0x%02x\t C: 0x%02x\n", regs[1], regs[2]);
		System.out.printf("D: 0x%02x\t E: 0x%02x\n", regs[3], regs[4]);
		System.out.printf("H: 0x%02x\t L: 0x%02x\n", regs[5], regs[6]);
		System.out.printf("SP: 0x%x [SP]: 0x%02x [SP+1]: 0x%02x\n", sp, memory[sp], memory[(sp == 65535) ? 0 : (sp+1)]);
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
	
	private int bytesToInt(byte high, byte low) {
		return ((high & 0xFF) << 8) + (low & 0xFF);
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
			case 0xC6: //ADI
				if (bytes_left == 1) {
					bytes_left--;
					res = (int)(0xFF&regs[0]) + (int)(0xFF&memory[pc]);
					//TODO: Compress to ternary
					if (res > 256) { //CY
						setCYFlag(true);
					}
					else {
						setCYFlag(false);
					}
					
					if (((regs[0]&0x0F) + (memory[pc]&0x0F)) > 0x0F) { //half carry (AC)
						setACFlag(true);
					}
					else {
						setACFlag(false);
					}
					
					if (temp == 0) { //Z Flag
						setZFlag(true);
					}
					else {
						setZFlag(false);
					}
					
					if ((temp&0x80) > 0) { //S Flag
						setSFlag(true);
					}
					else {
						setSFlag(false);
					}
					//P Flag
					regs[0] = (byte)(temp);
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 0xCE: //ACI
				incPC();
				break;
			default: //instructions with operand in opcode
				if ((cur_opp & 0xC7) == 6) { //MVI r/m group
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
				else if ((cur_opp & 0xC0) == 64) { //MOV r/m group
					temp =regConv((cur_opp&0x38)>>3); //dest
					if (temp == 7) { //dest is M
						memory[bytesToInt(regs[5],regs[6])] = regs[regConv(cur_opp&0x07)]; //mov m,m would have been caught by halt
					}
					else {
						regs[temp] = (regConv(cur_opp&0x07) == 7)  ?  memory[bytesToInt(regs[5],regs[6])] : regs[regConv(cur_opp&0x07)];
					}
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