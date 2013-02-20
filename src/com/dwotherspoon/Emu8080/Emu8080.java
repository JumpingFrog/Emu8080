package com.dwotherspoon.Emu8080;

import java.io.File;

public class Emu8080 {

	/**
	 * @param args
	 */
	public static void main(String[] args) {
		
		String path = "/media/david/1E4EB4844EB455ED/TimeLapse";
		File folder = new File(path);
		File[] list = folder.listFiles();
		
		for (int i=0; i < list.length; i++)  {
			String name = list[i].getName();
			String num = name.substring(5, name.length());
			name = "image"+gen_zeroes(9 - num.length())+num;
			System.out.println(name);
			//list[i].renameTo(new File(path+"/"+name));
		}
		
		/*
		byte test = 0;
		for (int i=0; i < 256; i++) {
			test++;
			System.out.println(test);
		}
		Core8080 cpu = new Core8080();
		
		try {
			cpu.addDevice(new DevConsole((byte)0,(byte)1));
		} catch (PortAlreadyUsedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		try {
			cpu.addDevice(new DevConsole((byte)0,(byte)1));
		} catch (PortAlreadyUsedException e) {
			System.out.println("Adding second device threw a PortAlreadyUsedException!");
		}
		
		System.out.println("Int max =" + Integer.MAX_VALUE); */
		
		

	}
	
	private static String gen_zeroes(int n) {
		String result = "";
		for (int i = 0; i < n; i++) {
			result += "0";
		}
		return result;
	}

}
