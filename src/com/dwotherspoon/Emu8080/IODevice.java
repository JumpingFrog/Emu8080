package com.dwotherspoon.Emu8080;

public interface IODevice {
	
	public void in(byte data); //function for writing to device.
	
	public byte out(); //function for reading device.
	
	public byte[] getPorts(); //function to return ports used by the device.
}
