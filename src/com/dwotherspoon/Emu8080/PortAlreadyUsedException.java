package com.dwotherspoon.Emu8080;

public class PortAlreadyUsedException extends Exception {
	/**
	 * Exception thrown if a device is added to an already used port;
	 */
	private static final long serialVersionUID = -5612009651712062234L;

	public PortAlreadyUsedException(String message) {
		super(message);
	}
	
}