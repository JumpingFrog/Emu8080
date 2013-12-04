package com.dwotherspoon.Emu8080;

import java.util.Queue;
import java.util.concurrent.ConcurrentLinkedQueue;

import javax.swing.JTextPane;

public class DevConsole implements IODevice {
	/*----------------------------------------------------
	 * Example console device. No warranty given.
	 * (C) David Wotherspoon 2013
	 -----------------------------------------------------*/
	private byte[] ports; //first port is data, second is ctrl
	private Queue<Character> inQ = new ConcurrentLinkedQueue<Character>();
	private JTextPane cout;
	
	public DevConsole(JTextPane cout, byte data, byte ctrl) {
		ports = new byte[2];
		ports[0] = data;
		ports[1] = ctrl;
		this.cout = cout;
	}
	
	@Override
	public void in(byte port, byte data) {
		//System.out.print((char)data);
		cout.setText(cout.getText() + (char) data);
	}

	@Override
	public byte out(byte port) {
		if (port == ports[0]) { //send data
			Character tmp = inQ.poll();
			return (tmp == null) ? (byte) 0x00 : (byte) tmp.charValue();
		}
		else { //send status
			Character tmp = inQ.peek();
			return (tmp == null) ? (byte) 0x00 : (byte) 0xFF;
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
