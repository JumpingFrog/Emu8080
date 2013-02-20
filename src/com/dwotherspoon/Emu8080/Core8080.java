package com.dwotherspoon.Emu8080;

public class Core8080 {
	/*----------------------------------------------------
	 * 8080 instruction interpreter. No warranty given.
	 * (C) David Wotherspoon 2013
	------------------------------------------------------*/
	
	private IODevice[] devices;
	private boolean halt; //halt flag
	private int pc; //program counter
	
	
	public Core8080() {	
		devices = new IODevice[256];
		halt = false;
		pc = 0;
	}
	
	public void run() {
		while (!halt) {
			execute();
		}
		System.out.println("System was halted!");
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
	
	private void execute() {
		
	}

}
