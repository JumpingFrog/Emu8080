package com.dwotherspoon.Emu8080;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;

import javax.swing.JButton;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.border.LineBorder;

public class EmuGui {
	private JFrame f;
	private JMenuBar menu;
	private JMenuItem open;
	private JMenuItem dump;
	private JTextPane cout;
	private JTextField cin;
	private JButton	csend;
	private Core8080 cpu;
	private DevConsole cons;
	private DiskIO dio;
	
	public EmuGui() {
		cpu = new Core8080();
		f = new JFrame("Emu8080 Console");
		f.setLayout(null);
		f.setSize(400, 400);
		f.setResizable(false);
		
		menu = new JMenuBar();
		menu.setLocation(0, 0);
		menu.setLayout(new FlowLayout(FlowLayout.LEFT));
		
		open = new JMenuItem("Open");
		open.addActionListener(new ActionListener () {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				 JFileChooser chooser = new JFileChooser();
	                if (chooser.showOpenDialog(f) == JFileChooser.APPROVE_OPTION) {
	                	File rom = new File(chooser.getSelectedFile().getAbsolutePath());
	                	if (rom.isFile()) {
	            			byte[] mem = new byte[65536];
	            			BufferedInputStream input;
	            			try {
	            				input = new BufferedInputStream(new FileInputStream(rom));
	            				input.read(mem, 0, (int) rom.length());
	            				input.close();
	            				cpu.setMem(mem);
	            				run();
	            			} catch (FileNotFoundException e1) {
	            				// TODO Auto-generated catch block
	            				e1.printStackTrace();
	            			} catch (IOException e) {
	            				// TODO Auto-generated catch block
	            				e.printStackTrace();
	            			} 
	               }
			}
			}
		});
		
		cout = new JTextPane();
		cout.setBorder(new LineBorder(Color.gray, 2));
		cout.setEditable(false);

		JScrollPane jsp = new JScrollPane(cout);
		jsp.setSize(380, 280);
		jsp.setLocation(10, 20);
		
		cin = new JTextField();
		cin.setLocation(10, 310);
		cin.setSize(300, 20);
		
		csend = new JButton("Send");
		csend.setLocation(315,310);
		csend.setSize(80,20);
		csend.addActionListener(new ActionListener() {

			@Override
			public void actionPerformed(ActionEvent arg0) {
				byte[] temp = cin.getText().getBytes();
				for (int i = 0; i < temp.length; i++) {
					cons.addChar((char)temp[i]);
				}
				cin.setText("");
				
			}
			
		});
		
		menu.add(open);
		f.add(cin);
		f.add(jsp);
		f.add(csend);
		f.setJMenuBar(menu);
		f.setVisible(true);
		//Form done, construct emu
		cons = new DevConsole(cout, (byte)0,(byte)1);
		dio = new DiskIO((byte)2, (byte)3);
		try {
			cpu.addDevice(cons);
			cpu.addDevice(dio);
		} catch (PortAlreadyUsedException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
	}
	
	private void run() {
		cpu.start();
	}
}
