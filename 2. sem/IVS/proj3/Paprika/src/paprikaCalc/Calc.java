/*
 * File:         Calc.java
 * Date:         21.4.2015
 * Author:       Paprika, xvlkto00@fit.vutbr.cz
 * Project:      Project 3 for course IVS
 * Description:  Simple graphical calculator
 */

/**
 * @file Calc.java
 * @author Paprika
 * @package paprikaCalc
 * @date 21. 4. 2015
 * @brief File contain the main body of the program
 */


package paprikaCalc;

import paprikaMath.Math;

public class Calc {
		
	/**
	 * @brief Function initialize GUI of calculator
	 * @param args Arguments of program
	 */
	public static void main(String[] args) {
		new GUI();	
	}
	
	/**
	 * @brief Function calculate using mathematical library
	 * @param x First number
	 * @param y Second number
	 * @param operation How operate with numbers. For example "+", "-", ..
	 * @return Return calculate result (in double)
	 */
	public static double calculate(String operation, Double x, Double y) throws Exception {
		if(operation.contentEquals("PLUS")){
			return Math.plus(x, y);
		}
		else if(operation.contentEquals("MINUS")){
			return Math.minus(x, y);
		}
		else if(operation.contentEquals("MULT")){
			return Math.multiply(x, y);
		}
		else if(operation.contentEquals("DIV")){
			return Math.divide(x, y);
		}
		else 
			throw new Exception();
	}
	
	/**
	 * @brief Function calculate using mathematical library
	 * @param x First number (non-decimal)
	 * @param y Second number (non-decimal)
	 * @param operation How operate with numbers. For example "+", "-", ..
	 * @return Return calculate result (in integer)
	 */
	public static int calculate(String operation, int x, int y) throws Exception {
		if(operation.contentEquals("PLUS")) {
			return Math.plus(x, y);
		}
		else if(operation.contentEquals("MINUS")){
			return Math.minus(x, y);
		}
		else if(operation.contentEquals("DIV")){
			return Math.divide(x, y);
		}
		else if(operation.contentEquals("MULT")){
			return Math.multiply(x, y);
		}
		else if(operation.contentEquals("POW")){
			return Math.pow(x, y);
		}
		else if(operation.contentEquals("FACT")){
			return Math.factorial(x);
		}
		else 
			throw new Exception();
	
	}
	
}