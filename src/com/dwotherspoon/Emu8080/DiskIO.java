package com.dwotherspoon.Emu8080;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

public class DiskIO implements IODevice {
	/*----------------------------------------------------
	 * Example DiskIO device. No warranty given.
	 * Attempt to emulate 720k drive, 160 tracks, 9 sectors per track, 512 byte sectors.
	 * (C) David Wotherspoon 2013
	 -----------------------------------------------------*/
	
	//TODO: Write too
	private byte[] ports;
	private int tracks = 160;
	private int sectors = 9;
	private int sector_size = 512;
	private int lbas = tracks*sectors; //1440
	
	private byte cmd = 0; //0 = nop, 1 = Track Set, 2 = Sector Set, 3 = home
	private int track = 0;
	private int sector = 0;
	private int cur = 0; //current byte position
	private byte[][] data = new byte[lbas][512]; //1440 LB's, 512 each
	
	public DiskIO(byte cmd, byte d) {
		File rom = new File("/home/david/disk.bin");
    	if (rom.isFile()) {
			BufferedInputStream input;
			try {
				input = new BufferedInputStream(new FileInputStream(rom));
				for (int lba = 0; lba < lbas; lba++) { //read in disk
					input.read(data[lba], 0, sector_size); //read a sector
				}
				input.close();
			} catch (FileNotFoundException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
    	}
		
		ports = new byte[2];
		ports[0] = cmd; //cmd
		ports[1] = d; ;//data
	}
	
	@Override
	public void in(byte port, byte data) {
		if (port == ports[0]) { //cmd word
			cmd = data;
			if (cmd == 3) {
				track = 0;
				sector = 0;
				cur = 0;
			}
		}
		else { //data for command
			switch (cmd) {
			case 0x01: //track set
				track = (0xFF & data);
				cur = 0;
				System.out.println("Disk track set to " + track);
				break;
			case 0x02: //sector set
				sector = (0xFF & data);
				cur = 0;
				System.out.println("Disk sector set to " + sector);
				break;
			}
		}
	}

	@Override
	public byte out(byte port) {
		return data[track*9+sector][cur++];
	}

	@Override
	public byte[] getPorts() {
		return ports;
	}

}
