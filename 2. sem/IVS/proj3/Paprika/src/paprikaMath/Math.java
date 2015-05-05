/*
 * File:         Math.java
 * Date:         21.4.2015
 * Author:       Paprika, xvlkto00@fit.vutbr.cz
 * Project:      Project 3 for course IVS
 * Description:  Simple graphical calculator
 */

/**
 * @file Math.java
 * @author Paprika
 * @package paprikaMath
 * @date 21. 4. 2015
 * @brief File contain mathematical library
 */


package paprikaMath;

public class Math {

	public static final double PI=3.14159; /**< Constant of Pi number */

	/**
	 * @brief Function sums two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of summing
	 */
	public static double plus(double x, double y) {
		return x+y;
	}
	
	/**
	 * @brief Function sums two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of summing
	 */
	public static int plus(int x, int y) {
		return x+y;
	}
	
	/**
	 * @brief Function subtracts two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of subtracting
	 */
	public static double minus(double x, double y) {
		return x-y;
	}
	
	/**
	 * @brief Function subtracts two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of subtracting
	 */
	public static int minus(int x, int y) {
		return x-y;
	}
	
	/**
	 * @brief Function divides two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of dividing
	 * @exception  Exception if y == 0
	 */
	public static double divide(double x, double y) throws Exception {
		if(y == 0)
			throw new Exception();
		return x/y;
	}
	
	/**
	 * @brief Function divides two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of dividing
	 * @exception  Exception if y == 0
	 */
	public static int divide(int x, int y) throws Exception {
		if(y == 0)
			throw new Exception();
		return x/y;
	}
	
	/**
	 * @brief Function multiply two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of multiplying
	 */
	public static double multiply(double x, double y) {
		return x*y;
	}
	
	/**
	 * @brief Function multiply two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of multiplying
	 */
	public static int multiply(int x, int y) {
		return x*y;
	}
	
	/**
	 * @brief Function powers two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of powering
	 * @exception  Exception if y < 0
	 */
	public static double pow(double x, int y) throws Exception {
		if(y < 0)
			throw new Exception();
		double result = 1.0;
		for(int i=0; i < y; i++){
			result*=x;
		}
		return result;
	}
	
	/**
	 * @brief Function powers two numbers
	 * @param x First number
	 * @param y Second number
	 * @return Return result of powering
	 * @exception  Exception if y < 0
	 */
	public static int pow(int x, int y) throws Exception {
		if(y < 0)
			throw new Exception();
		int result = 1;
		for(int i=0; i < y; i++){
			result*=x;
		}
		return result;
	}
	
	/**
	 * @brief Function counts factorial of number
	 * @param x Number for counting factorial
	 * @return Return factorial of number
	 * @exception  Exception if x < 0
	 */
	public static int factorial(int x) throws Exception {
		if(x == 0) 
			return 1;
		if(x < 0)
			throw new Exception();
		
		for(int i = x-1; i > 1; i--){
			x*=i;
		}
		return x;
	}
}
