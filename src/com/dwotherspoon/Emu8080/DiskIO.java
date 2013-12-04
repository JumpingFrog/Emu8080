package com.dwotherspoon.Emu8080;

public class DiskIO implements IODevice {
	/*----------------------------------------------------
	 * Example DiskIO device. No warranty given.
	 * (C) David Wotherspoon 2013
	 -----------------------------------------------------*/
	private byte[] ports;
	
	
	public DiskIO(byte rx, byte tx) {
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
		// TODO Auto-generated method stub
		return 0;
	}

	@Override
	public byte[] getPorts() {
		return ports;
	}

}
