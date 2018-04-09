/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Jan 9, 2015
                                                        cs12xre
                                Assignment One

File Name:      hw1.java
Description:    This program tests functions to display output strings and 
                numbers. 
****************************************************************************/

/**
 * The hw1 class is a direct port of hw1.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.
 */

import java.io.*;      // System.out

/*
* Class Name: hw1
* Class Description: This program takes in numbers or strings and outputs
* them to a display.
*/
public class hw1 {
   private static final int COUNT = 8;	// number of hex digits to display
   private static final int DECIMAL = 10;	// to indicate base 10
   private static final int HEX = 16;		// to indicate base 16

   private static final char digits[] = 	// used for ASCII conversion
   new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

   private static int hexCounter = 0; // counter for the number hex digits

   //used to create array in baseout  
   private static final int NUMARRAYDIGITS = 1000; 

   /**
   * Takes in a positive number and displays in a given base.
   *
   * @param    number   Numeric value to be displayed.
   * @param    base     Base used to display number.
   * @param    stream   Where to display, likely System.out or System.err.
   */
   private static void baseout (int number, int base, PrintStream stream) {
      int numCopy = number; //makes a copy of the number
      int arrayIndex = 0; //index of array, used to store and print the digits

      //creates a large, empty array
      char[] arrayDigits = new char[NUMARRAYDIGITS];
    
      //makes the first 8 ints in the array 0s
      for(int i = 0; i < COUNT; i++)
      {
        arrayDigits[i] = digits[0];
      } 

      while(numCopy != 0)
      {
        //finds the digit by modding, going from back to front and 
        //records that digit as a char
        arrayDigits[arrayIndex] = digits[numCopy % base];
        //divides by the base to get the number without the digit that was 
        //just found
        numCopy = numCopy / base;

        //goes to the next index in the array
        arrayIndex++;
      }

      //if arrayIndex was 0 (number is 0), print 0
      if(arrayIndex == 0)
      {
         fputc(digits[0], stream);
      }	

      //if base 16
      else if(base == HEX)
      {
         arrayIndex = COUNT - 1; //first digit is at this index
  
         while(arrayIndex >= 0)
         {	    
            //finds and records the digit that needs to be printed next
            char digitToPrint = arrayDigits[arrayIndex];

            //calls on fputc to print the digit
            fputc(digitToPrint, stream);

            //decrements arrayIndex to print the next digit
            arrayIndex--;
         }
      }

      //any base except base 16
      else
      {
	 //first digit to be printed is at this index
         int lastDigitIndex = arrayIndex - 1;

         //while the number hasn't finished printing
         while(lastDigitIndex >= 0)
         {
            //finds and records the digit that needs to be printed next
            char digitToPrint = arrayDigits[lastDigitIndex];
 
            //calls on fputc to print the digit
            fputc(digitToPrint, stream);

            //decrements lastDigitIndex to print the next digit
            lastDigitIndex--;
         }
      }	
   }


   /**
   * Takes in a positive number and displays it in decimal.
   *
   * @param    number   positive numeric value to be displayed.
   * @param    stream   Where to display, likely System.out or System.err.
   */
   public static void decout (int number, PrintStream stream) {
      //calls baseout to print a number in base 10
      baseout(number, DECIMAL, stream);   
   }


   /**
   * Displays a single character.
   *
   * @param    CharToDisp  Character to display.
   * @param    stream      Where to display, likely System.out or System.err.
   */
   public static void fputc(char CharToDisp, PrintStream stream) {
      // Print a single character.
      stream.print (CharToDisp);   

      // Flush the system.out buffer, now. 
      stream.flush ();
   }

   /**
   * Takes in a positive number and displays it in hex.
   *
   * @param    number   A positive numeric value to be displayed in hex.
   * @param    stream   Where to display, likely System.out or System.err.
   */
   public static void hexout (int number, PrintStream stream) {
      // Output "0x" for hexidecimal.
      writeline ("0x", stream);
     
      //calls baseout with base 16
      baseout (number, HEX, stream);
   }


   /**
   * Prints out a newline character.
   * @param    stream  Where to display, likely System.out or System.err.
   *
   */
   public static void newline ( PrintStream stream ) {
      //outputs a new line character  
      writeline("\n", stream);
   }


   /**
   * Prints out a string.
   *
   * @param    message  A string to print out.
   * @param    stream   Where to display, likely System.out or System.err.
   * @return   <code>int</code> - The length of the string.
   */
   public static int writeline (String message, PrintStream stream) {        
      int index = 0; //what character to print next

      //prints the next character if there are still characters left to 
      //print
      while(index < message.length())
      {
         fputc(message.charAt(index), stream); //prints chars
         index++;
      }

      return message.length(); //returns length of message
   }


  public static void main( String[] args ) {
    System.err.print("Zdravei Sviat\n");

    writeline ("Hello World", System.out);
    newline(System.out);

    decout (123, System.out);
    newline(System.out);

    decout (0, System.out);
    newline(System.out);

    hexout (0xFEEDDAD, System.out);
    newline(System.out);
  }
}

