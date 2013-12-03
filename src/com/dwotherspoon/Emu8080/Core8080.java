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
		setCYFlag(res < 255); //If result is more than 255, clear the borrow flag
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
		setCYFlag(res < 255); //If result is more than 255, clear the borrow flag
		setACFlag(((regs[0]&0x0F) + (in&0x0F)) > 0x0F); //First 4 bits summed result in half carry? (GNUSim8085 doesn't do this)
		setZFlag((0xFF&res) == 0);
		setSFlag((res&0x80) > 0);
		regs[0] = (byte) (0xFF&res);
		genPFlag();
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
			/** End Arithmetic - Start Branch  **/
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
