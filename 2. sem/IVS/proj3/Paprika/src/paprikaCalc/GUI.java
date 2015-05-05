/*
 * File:         GUI.java
 * Date:         21.4.2015
 * Author:       Paprika, xvlkto00@fit.vutbr.cz
 * Project:      Project 3 for course IVS
 * Description:  Simple graphical calculator
 */

/**
 * @file GUI.java
 * @author Paprika
 * @package paprikaCalc
 * @date 21. 4. 2015
 * @brief File contain calculator GUI
 */


package paprikaCalc;

import javax.swing.JFrame;

import java.awt.GridBagLayout;

import javax.swing.JTextField;

import java.awt.GridBagConstraints;

import javax.swing.JButton;

import java.awt.Insets;

import javax.swing.SwingConstants;

import java.awt.Font;

import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JMenu;


public class GUI {

	private JFrame frame;	 /**< Main window of calculator */
	public static JTextField textField;	 /**< Text field for showing numbers and operators */

	/**
	 * Create the application.
	 */
	public GUI() {
		initialize();
	}

	/**
	 * Initialize the contents of the frame.
	 */
	private void initialize() {
		frame = new JFrame();
		frame.setBounds(100, 100, 450, 300);
		frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		Actions listener = new Actions();
		GridBagLayout gridBagLayout = new GridBagLayout();
		gridBagLayout.columnWidths = new int[] {0, 0, 0, 0, 0, 0, 0};
		gridBagLayout.rowHeights = new int[] {0, 0, 0, 0, 0, 0};
		gridBagLayout.columnWeights = new double[]{1.0, 0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		gridBagLayout.rowWeights = new double[]{0.0, 0.0, 0.0, 0.0, 0.0, Double.MIN_VALUE};
		frame.getContentPane().setLayout(gridBagLayout);
		
		textField = new JTextField();
		textField.setEditable(false);
		textField.setFont(new Font("Dialog", Font.BOLD, 22));
		textField.setHorizontalAlignment(SwingConstants.RIGHT);
		GridBagConstraints gbc_textField = new GridBagConstraints();
		gbc_textField.weighty = 1.0;
		gbc_textField.weightx = 1.0;
		gbc_textField.fill = GridBagConstraints.BOTH;
		gbc_textField.gridwidth = 6;
		gbc_textField.insets = new Insets(5, 5, 5, 5);
		gbc_textField.gridx = 0;
		gbc_textField.gridy = 0;
		frame.getContentPane().add(textField, gbc_textField);
		textField.setColumns(10);
		
		JButton btnPi = new JButton("PI");
		btnPi.setActionCommand("PI");
		btnPi.addActionListener(listener);
	    GridBagConstraints gbc_btnPi = new GridBagConstraints();
		gbc_btnPi.fill = GridBagConstraints.BOTH;
		gbc_btnPi.weighty = 1.0;
		gbc_btnPi.weightx = 1.0;
		gbc_btnPi.insets = new Insets(5, 5, 5, 5);
		gbc_btnPi.gridx = 5;
		gbc_btnPi.gridy = 2;
		frame.getContentPane().add(btnPi, gbc_btnPi);
		
		JButton btn1 = new JButton("1");
		btn1.setActionCommand("1");
		btn1.addActionListener(listener);
		GridBagConstraints gbc_btn1 = new GridBagConstraints();
		gbc_btn1.fill = GridBagConstraints.BOTH;
		gbc_btn1.weighty = 1.0;
		gbc_btn1.weightx = 1.0;
		gbc_btn1.insets = new Insets(5, 5, 5, 5);
		gbc_btn1.gridx = 0;
		gbc_btn1.gridy = 1;
		frame.getContentPane().add(btn1, gbc_btn1);
		
		JButton btn2 = new JButton("2");
		btn2.setActionCommand("2");
		btn2.addActionListener(listener);
		GridBagConstraints gbc_btn2 = new GridBagConstraints();
		gbc_btn2.fill = GridBagConstraints.BOTH;
		gbc_btn2.weighty = 1.0;
		gbc_btn2.weightx = 1.0;
		gbc_btn2.insets = new Insets(5, 5, 5, 5);
		gbc_btn2.gridx = 1;
		gbc_btn2.gridy = 1;
		frame.getContentPane().add(btn2, gbc_btn2);
		
		JButton btn3 = new JButton("3");
		btn3.setActionCommand("3");
		btn3.addActionListener(listener);
		GridBagConstraints gbc_btn3 = new GridBagConstraints();
		gbc_btn3.fill = GridBagConstraints.BOTH;
		gbc_btn3.weighty = 1.0;
		gbc_btn3.weightx = 1.0;
		gbc_btn3.insets = new Insets(5, 5, 5, 5);
		gbc_btn3.gridx = 2;
		gbc_btn3.gridy = 1;
		frame.getContentPane().add(btn3, gbc_btn3);
		
		JButton btnPlus = new JButton("+");
		btnPlus.setActionCommand("PLUS");
		btnPlus.addActionListener(listener);
		GridBagConstraints gbc_btnPlus = new GridBagConstraints();
		gbc_btnPlus.fill = GridBagConstraints.BOTH;
		gbc_btnPlus.weighty = 1.0;
		gbc_btnPlus.weightx = 1.0;
		gbc_btnPlus.insets = new Insets(5, 5, 5, 5);
		gbc_btnPlus.gridx = 3;
		gbc_btnPlus.gridy = 1;
		frame.getContentPane().add(btnPlus, gbc_btnPlus);
		
		JButton btnMinus = new JButton("-");
		btnMinus.setActionCommand("MINUS");
		btnMinus.addActionListener(listener);
		btnMinus.setFont(new Font("Dialog", Font.BOLD, 12));
		GridBagConstraints gbc_btnMinus = new GridBagConstraints();
		gbc_btnMinus.fill = GridBagConstraints.BOTH;
		gbc_btnMinus.weighty = 1.0;
		gbc_btnMinus.weightx = 1.0;
		gbc_btnMinus.insets = new Insets(5, 5, 5, 5);
		gbc_btnMinus.gridx = 4;
		gbc_btnMinus.gridy = 1;
		frame.getContentPane().add(btnMinus, gbc_btnMinus);
		
		JButton btn4 = new JButton("4");
		btn4.setActionCommand("4");
		btn4.addActionListener(listener);
		GridBagConstraints gbc_btn4 = new GridBagConstraints();
		gbc_btn4.fill = GridBagConstraints.BOTH;
		gbc_btn4.weighty = 1.0;
		gbc_btn4.weightx = 1.0;
		gbc_btn4.insets = new Insets(5, 5, 5, 5);
		gbc_btn4.gridx = 0;
		gbc_btn4.gridy = 2;
		frame.getContentPane().add(btn4, gbc_btn4);
		
		JButton btn5 = new JButton("5");
		btn5.setActionCommand("5");
		btn5.addActionListener(listener);
		GridBagConstraints gbc_btn5 = new GridBagConstraints();
		gbc_btn5.fill = GridBagConstraints.BOTH;
		gbc_btn5.weighty = 1.0;
		gbc_btn5.weightx = 1.0;
		gbc_btn5.insets = new Insets(5, 5, 5, 5);
		gbc_btn5.gridx = 1;
		gbc_btn5.gridy = 2;
		frame.getContentPane().add(btn5, gbc_btn5);
		
		JButton btn6 = new JButton("6");
		btn6.setActionCommand("6");
		btn6.addActionListener(listener);
		GridBagConstraints gbc_btn6 = new GridBagConstraints();
		gbc_btn6.fill = GridBagConstraints.BOTH;
		gbc_btn6.weighty = 1.0;
		gbc_btn6.weightx = 1.0;
		gbc_btn6.insets = new Insets(5, 5, 5, 5);
		gbc_btn6.gridx = 2;
		gbc_btn6.gridy = 2;
		frame.getContentPane().add(btn6, gbc_btn6);
		
		JButton btnMul = new JButton("*");
		btnMul.setActionCommand("MULT");
		btnMul.addActionListener(listener);
		GridBagConstraints gbc_btnMul = new GridBagConstraints();
		gbc_btnMul.fill = GridBagConstraints.BOTH;
		gbc_btnMul.weighty = 1.0;
		gbc_btnMul.weightx = 1.0;
		gbc_btnMul.insets = new Insets(5, 5, 5, 5);
		gbc_btnMul.gridx = 3;
		gbc_btnMul.gridy = 2;
		frame.getContentPane().add(btnMul, gbc_btnMul);
		
		JButton btnDiv = new JButton("/");
		btnDiv.setActionCommand("DIV");
		btnDiv.addActionListener(listener);
		GridBagConstraints gbc_btnDiv = new GridBagConstraints();
		gbc_btnDiv.fill = GridBagConstraints.BOTH;
		gbc_btnDiv.weighty = 1.0;
		gbc_btnDiv.weightx = 1.0;
		gbc_btnDiv.insets = new Insets(5, 5, 5, 5);
		gbc_btnDiv.gridx = 4;
		gbc_btnDiv.gridy = 2;
		frame.getContentPane().add(btnDiv, gbc_btnDiv);
		
		JButton btn7 = new JButton("7");
		btn7.setActionCommand("7");
		btn7.addActionListener(listener);
		GridBagConstraints gbc_btn7 = new GridBagConstraints();
		gbc_btn7.fill = GridBagConstraints.BOTH;
		gbc_btn7.weighty = 1.0;
		gbc_btn7.weightx = 1.0;
		gbc_btn7.insets = new Insets(5, 5, 5, 5);
		gbc_btn7.gridx = 0;
		gbc_btn7.gridy = 3;
		frame.getContentPane().add(btn7, gbc_btn7);
		
		JButton btn8 = new JButton("8");
		btn8.setActionCommand("8");
		btn8.addActionListener(listener);
		GridBagConstraints gbc_btn8 = new GridBagConstraints();
		gbc_btn8.fill = GridBagConstraints.BOTH;
		gbc_btn8.weighty = 1.0;
		gbc_btn8.weightx = 1.0;
		gbc_btn8.insets = new Insets(5, 5, 5, 5);
		gbc_btn8.gridx = 1;
		gbc_btn8.gridy = 3;
		frame.getContentPane().add(btn8, gbc_btn8);
		
		JButton btn9 = new JButton("9");
		btn9.setActionCommand("9");
		btn9.addActionListener(listener);
		GridBagConstraints gbc_btn9 = new GridBagConstraints();
		gbc_btn9.fill = GridBagConstraints.BOTH;
		gbc_btn9.weighty = 1.0;
		gbc_btn9.weightx = 1.0;
		gbc_btn9.insets = new Insets(5, 5, 5, 5);
		gbc_btn9.gridx = 2;
		gbc_btn9.gridy = 3;
		frame.getContentPane().add(btn9, gbc_btn9);
		
		JButton btnFactorial = new JButton("!");
		btnFactorial.setActionCommand("FACT");
		btnFactorial.addActionListener(listener);
		GridBagConstraints gbc_btnFactorial = new GridBagConstraints();
		gbc_btnFactorial.fill = GridBagConstraints.BOTH;
		gbc_btnFactorial.weighty = 1.0;
		gbc_btnFactorial.weightx = 1.0;
		gbc_btnFactorial.insets = new Insets(5, 5, 5, 5);
		gbc_btnFactorial.gridx = 4;
		gbc_btnFactorial.gridy = 3;
		frame.getContentPane().add(btnFactorial, gbc_btnFactorial);
		
		JButton btnEquals = new JButton("=");
		btnEquals.setActionCommand("EQUALS");
		btnEquals.addActionListener(listener);
		GridBagConstraints gbc_btnEquals = new GridBagConstraints();
		gbc_btnEquals.fill = GridBagConstraints.BOTH;
		gbc_btnEquals.insets = new Insets(5, 5, 5, 5);
		gbc_btnEquals.weighty = 1.0;
		gbc_btnEquals.weightx = 1.0;
		gbc_btnEquals.gridx = 5;
		gbc_btnEquals.gridy = 3;
		frame.getContentPane().add(btnEquals, gbc_btnEquals);
		
		JButton btnPow = new JButton("pow");
		btnPow.setActionCommand("POW");
		btnPow.addActionListener(listener);
		GridBagConstraints gbc_btnPow = new GridBagConstraints();
		gbc_btnPow.fill = GridBagConstraints.BOTH;
		gbc_btnPow.weighty = 1.0;
		gbc_btnPow.weightx = 1.0;
		gbc_btnPow.insets = new Insets(5, 5, 5, 5);
		gbc_btnPow.gridx = 5;
		gbc_btnPow.gridy = 1;
		frame.getContentPane().add(btnPow, gbc_btnPow);
		
		JButton btn0 = new JButton("0");
		btn0.setActionCommand("0");
		btn0.addActionListener(listener);
		GridBagConstraints gbc_btn0 = new GridBagConstraints();
		gbc_btn0.fill = GridBagConstraints.BOTH;
		gbc_btn0.insets = new Insets(5, 5, 5, 5);
		gbc_btn0.gridx = 3;
		gbc_btn0.gridy = 3;
		frame.getContentPane().add(btn0, gbc_btn0);
		
		JMenuBar menuBar = new JMenuBar();
		frame.setJMenuBar(menuBar);
		
		JMenu mnHehelp = new JMenu("Menu");
		menuBar.add(mnHehelp);
		
		JMenuItem mntmHelp = new JMenuItem("Help");
		mnHehelp.add(mntmHelp);
		mntmHelp.setHorizontalAlignment(SwingConstants.LEFT);
		frame.setVisible(true);
	}
}
