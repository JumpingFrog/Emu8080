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
	private int sp = 65536;
	private byte[] memory;
	private byte temp; //use for 2 operand instructions.
	
	
	public Core8080() {	
		devices = new IODevice[256];
		halt = false;
		pc = 0;
		regs = new byte[7];
		memory = new byte[65536];
			}
	
	public void run() {
		while (!halt) {
			execute();	
			System.out.println(Integer.toHexString(pc));
		}
		System.out.println("System was halted!");
	}
	/** Start Flags **/
	private void setSFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x80); }
		else {
			flags = (byte)(flags & 0x7F); }
	}
	
	private boolean getSFlag() {
		return ((flags & 0x80) != 0);
	}
	
	private void setZFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x40); }
		else {
			flags = (byte)(flags & 0xBF); }
	}
	
	private boolean getZFlag() {
		return ((flags & 0x40) != 0);
	}
	
	private void setACFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x10); }
		else {
			flags = (byte)(flags & 0xEF); }
	}
	
	private boolean getACFlag() {
		return ((flags & 0x10) != 0);
	}
	
	private void setPFlag(boolean value) {
		if (value) {
			flags = (byte)(flags | 0x04); }
		else {
			flags = (byte)(flags & 0xFB); }
	}
	
	private boolean getPFlag() {
		return ((flags & 0x04) != 0);
	}
	
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
	
	private void execute() {
		if (bytes_left == 0) {
			cur_opp = (short)(memory[pc] & 0xFF); }
		switch (cur_opp) {
		/** Start Control **/
			case 0: //NOP
				incPC();
				break;
			case 55: //STC
				setCYFlag(true);
				incPC();
				break;
			case 63: //CMC
				setCYFlag(!getCYFlag());
				incPC();
				break;
			case 118: //HALT
				halt = true;
				break;
			case 243: //DI
				incPC();
				break;
			case 251: //EI
				incPC();
				break;
		/** End Control - Start Data Transfer  **/
			case 1: //LXI B
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
			case 2: //STAX B
				memory[bytesToInt(regs[1], regs[2])] = regs[0];
				incPC();
				break;
			case 10: //LDAX B
				regs[0] = memory[bytesToInt(regs[1],regs[2])];
				incPC();
				break;
			case 17: //LXI D
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
			case 18: //STAX D
				memory[bytesToInt(regs[3], regs[4])] = regs[0];
				incPC();
				break;
			case 26: //LDAX D
				regs[0] = memory[bytesToInt(regs[3],regs[4])];
				incPC();
				break;
			case 33: //LXI H
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
			case 34: //SHLD
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
			case 42: //LHLD
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
			case 50: //STA
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
			case 54: //MVI M
				if (bytes_left == 1) {
					bytes_left--;
					memory[getHL()] = memory[pc];
				}
				else {
					bytes_left = 1;
				}
				incPC();
				break;
			case 58: //LDA
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
			case 235: //XCHG
				temp = regs[3]; //D
				regs[3] = regs[5];//H->D
				regs[5] = temp; //D->H
				temp = regs[4]; //E
				regs[4] = regs[6]; //L->E
				regs[6] = temp; //E->L
				incPC();
				break;
		}
		
	}

}
