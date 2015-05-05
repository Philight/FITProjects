/*
 * File:         Actions.java
 * Date:         21.4.2015
 * Author:       Paprika, xvlkto00@fit.vutbr.cz
 * Project:      Project 3 for course IVS
 * Description:  Simple graphical calculator
 */

/**
 * @file Actions.java
 * @author Paprika
 * @package paprikaCalc
 * @date 21. 4. 2015
 * @brief File contain listener class for all buttons of GUI
 */


package paprikaCalc;

import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import paprikaMath.Math;

/**
 * @brief Function decides which action/operation will proceed
 */
public class Actions implements ActionListener{
	
	private String operation;	 /**< Remembers which last operation was chosen */
	private int operand1 = -1;	 /**< Variable for the first non-decimal operand */
	private double doubleOperand1= -1;	 /**< Variable for the first decimal operand */
	private int operand2= -1;	 /**< Variable for the second non-decimal operand */
	private double doubleOperand2= -1;	 /**< Variable for the second decimal operand */
	private boolean intOrDouble = false;	 /**< Decimal/Non-decimal operations trigger */
	private boolean clearTrigger = false;	 /**< Trigger for clearing TextField */
	
	public void actionPerformed(ActionEvent e) {
		
		String action = e.getActionCommand();
		String alreadyDisplayed = GUI.textField.getText();
		String panelText = "";
		
		if (clearTrigger == true) { // Trigger to clear screen
			alreadyDisplayed = "";
			clearTrigger = false;
		}
		if (action == "POW" | action == "FACT" | action == "PLUS" | action == "MINUS" | action == "MULT" | action == "DIV") {
			if (!alreadyDisplayed.isEmpty()) {
				try {
					operand1 = Integer.parseInt(alreadyDisplayed);	// int operation
					intOrDouble = true;
				} catch(NumberFormatException e1) { }
				
				if (intOrDouble == false)	 
					doubleOperand1 = Double.parseDouble(alreadyDisplayed); // double operation
			}
				
			operation = action;
			clearTrigger = true;
		}
		
		switch(action) {
		case "0":
			panelText="0";
			break;
		case "1":
			panelText="1";
			break;
		case "2":
			panelText="2";
			break;
		case "3":
			panelText="3";
			break;
		case "4":
			panelText="4";
			break;
		case "5":
			panelText="5";
			break;
		case "6":
			panelText="6";
			break;
		case "7":
			panelText="7";
			break;
		case "8":
			panelText="8";
			break;
		case "9":
			panelText="9";
			break;
			
		case "PI":
			panelText = String.valueOf(Math.PI);
			alreadyDisplayed = "";
			break;	
		
		case "FACT":
			try {
				panelText = String.valueOf(Calc.calculate(operation, operand1, operand2));
			} catch (Exception e1) { GUI.textField.setText("Syntax Error"); }
			alreadyDisplayed = "";
			break;
			
		case "EQUALS":
			String Result = "";
			
			if (!alreadyDisplayed.isEmpty()) 
				if (intOrDouble == false)
					doubleOperand2 = Double.parseDouble(alreadyDisplayed); // double operation
				else if (intOrDouble == true)		// int operation
					try {
						operand2 = Integer.parseInt(alreadyDisplayed); 
					} catch (Exception e1) { 		// else double operation
						doubleOperand1 = Double.valueOf(operand1);
						doubleOperand2 = Double.parseDouble(alreadyDisplayed);
						intOrDouble = false; 
						} 

		//------------------------------------------OPERATIONS HANDLE-------------------------------------------------
			
			if (operation  == "DIV" & intOrDouble == true) 	// Double value calc for DIV
				if (operand1 % operand2 > 0) { 
					try { 
						Result = String.valueOf(Calc.calculate(operation, Double.valueOf(operand1), Double.valueOf(operand2)));
					} catch (Exception e1) { GUI.textField.setText("Syntax Error"); } 
				}
				else {
					try {
						Result = String.valueOf(Calc.calculate(operation, operand1, operand2));
					} catch (Exception e1) { GUI.textField.setText("Syntax Error"); }
				}
		
			else if (intOrDouble == false) try {	// Calc double values
				Result = String.valueOf(Calc.calculate(operation, doubleOperand1, doubleOperand2));
			} catch (Exception e1) {e1.printStackTrace();}
			
			
			else if (intOrDouble == true) try { 	// Calc integer values
				Result = String.valueOf(Calc.calculate(operation, operand1, operand2));
			} catch (Exception e1) { e1.printStackTrace(); }	
		
			
			alreadyDisplayed = "";
			panelText = Result;
			clearTrigger = true;
			break;
		}

		GUI.textField.setText(alreadyDisplayed + panelText); // Display result
	    
	}
}
