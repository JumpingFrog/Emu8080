package com.dwotherspoon.Emu8080;

import java.util.LinkedList;
import java.util.Queue;

public class DevConsole implements IODevice {
	/*----------------------------------------------------
	 * Example console device. No warranty given.
	 * (C) David Wotherspoon 2013
	 -----------------------------------------------------*/
	private byte[] ports;
	private Queue<Character> inQ = new LinkedList<Character>();
	
	public DevConsole(byte rx, byte tx) {
		ports = new byte[2];
		ports[0] = rx;
		ports[1] = tx;
	}
	
	@Override
	public void in(byte data) {
		System.out.print((char)data);
	}

	@Override
	public byte out() {
		Character tmp = inQ.poll();
		if (tmp == null) {
			return 0x00;
		}
		else {
			return (byte) tmp.charValue();
		}
	}
	
	public void addChar(char in) {
		inQ.add(in);
	}

	@Override
	public byte[] getPorts() {
		return ports;
	}

}
