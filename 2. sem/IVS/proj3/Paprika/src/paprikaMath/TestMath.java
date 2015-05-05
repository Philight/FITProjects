/*
 * File:         TestMath.java
 * Date:         21.4.2015
 * Author:       Paprika, xvlkto00@fit.vutbr.cz
 * Project:      Project 3 for course IVS
 * Description:  Simple graphical calculator
 */

/**
 * @file TestMath.java
 * @author Paprika
 * @package paprikaMath
 * @date 21. 4. 2015
 * @brief File contain tests for mathematical library
 */


package paprikaMath;

import static org.junit.Assert.*;

import org.junit.Test;


public class TestMath {

	@Test
    public void testPlusInt() {
        assertEquals(42, Math.plus(40, 2));
    }
	
	@Test
    public void testPlusDouble() {
        assertEquals(42.0, Math.plus(40.0, 2.0), 0.000001);
    }
	
	@Test
    public void testMinusInt() {
        assertEquals(42, Math.minus(44, 2));
    }
	
	@Test
    public void testMinusDouble() {
        assertEquals(42.0, Math.minus(44.0, 2.0), 0.000001);
    } 
	
	@Test
    public void testDivideInt() {
        try {
			assertEquals(42, Math.divide(84, 2));
		} catch (Exception e) {
			fail();
		}
    }
	
	@Test
    public void testDivideDouble() {
        try {
			assertEquals(42.0, Math.divide(84.0, 2.0), 0.000001);
		} catch (Exception e) {
			fail();
		}
    } 
	
	@Test
    public void testMultiplyInt() {
        assertEquals(42, Math.multiply(21, 2));
    }
	
	@Test
    public void testMultiplyDouble() {
        assertEquals(42.0, Math.multiply(21.0, 2.0), 0.000001);
    } 	
	
	@Test
    public void testPowInt() {
        try {
			assertEquals(16, Math.pow(4, 2));
		} catch (Exception e) {
			fail();
		}
    }
	
	@Test
    public void testPowDouble() {
        try {
			assertEquals(16.0, Math.pow(4.0, 2), 0.000001);
		} catch (Exception e) {
			fail();
		}
    } 	

	@Test
    public void testPowByZeroInt() {
        try {
			assertEquals(1, Math.pow(4, 0));
		} catch (Exception e) {
			fail();
		}
    }
	
	@Test
    public void testPowByZeroDouble() {
        try {
			assertEquals(1.0, Math.pow(4.0, 0), 0.000001);
		} catch (Exception e) {
			fail();
		}
    } 	
	
	@Test(expected = Exception.class)  
    public void testPowByNegativeInt() throws Exception {
		Math.pow(42, -3);
    }
	
	@Test(expected = Exception.class)  
    public void testPowByNegativeDouble() throws Exception {
		Math.pow(42, -3);
    } 	
	
    @Test(expected = Exception.class)  
    public void testDivideByZeroInt() throws Exception {
		Math.divide(42, 0);
    }
    
    @Test(expected = Exception.class)  
    public void testDivideByZeroDouble() throws Exception {
		Math.divide(42.0, 0.0);
    }
    
	@Test
    public void testFactorial() {
        try {
			assertEquals(24, Math.factorial(4));
		} catch (Exception e) {
			fail();
		}
    } 
	
	@Test
    public void testFactorialZero() {
        try {
			assertEquals(1, Math.factorial(0));
		} catch (Exception e) {
			fail();
		}
    } 
    
    @Test(expected = Exception.class)  
    public void testFactorialNegative() throws Exception {
		Math.factorial(-4);
    }
}
