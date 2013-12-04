package com.dwotherspoon.Emu8080;

public interface IODevice {
	/*----------------------------------------------------
	 * Interface for Emu8080 IO Devices. No warranty given.
	 * (C) David Wotherspoon 2013
	------------------------------------------------------*/
	
	public void in(byte port, byte data); //function for writing to device.
	
	public byte out(byte port); //function for reading device.
	
	public byte[] getPorts(); //function to return ports used by the device.
}
