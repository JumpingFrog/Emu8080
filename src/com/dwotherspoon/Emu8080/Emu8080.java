package com.dwotherspoon.Emu8080;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.util.Scanner;

public class Emu8080 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		System.out.println("Emu8080 - (C) David Wotherspoon 2013");
		System.out.println("Please enter the name of a ROM file to load (current directory).");
		Scanner console_in = new Scanner(System.in);
		String fname = console_in.next();
		File rom = new File(fname);
		if (rom.isFile()) {
			byte[] mem = new byte[65536];
			BufferedInputStream input;
			try {
				input = new BufferedInputStream(new FileInputStream(rom));
				input.read(mem, 0, (int) rom.length());
				input.close();
			} catch (FileNotFoundException e1) {
				// TODO Auto-generated catch block
				e1.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} 
			Core8080 cpu = new Core8080(mem);
			DevConsole console = new DevConsole((byte)0,(byte)1);
			console.addChar('v');
			console.addChar('d');
			console.addChar('0');
			console.addChar('0');
			console.addChar('0');
			console.addChar('0');
			try {
				cpu.addDevice(console);
			} catch (PortAlreadyUsedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			
			cpu.run();
		}
	}
}
