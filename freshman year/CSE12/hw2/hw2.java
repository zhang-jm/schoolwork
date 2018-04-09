/****************************************************************************

                                                        Jamie Zhang
                                                        CSE 12, Winter 2015
                                                        January 12, 2015
                                                        cs12xre
                                Assignment Two

File Name:      hw2.java
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.

****************************************************************************/

/**
 * The hw2 class is a direct port of hw2.c to java.
 * As you already know in java when you pass literal strings like
 * <P>
 *   writeline("a literal string\n", stream);
 * <P>
 * in C is considered a char[], but in java it's automatically
 * converted and treated as a String object.  Therefore 
 * the function writeline accepts literal strings and 
 * String types.  The getaline function returns a String type.
 */

import java.io.*;        // System.in and System.out
import java.util.*;      // Stack

/*
 * Class Name: MyLibCharacter
 * Description: Not written by me. I don't really know what it does, but I 
 *              assume it makes the program work.
 */
class MyLibCharacter {
   private Character character;

   public MyLibCharacter (int ch) {
      character = new Character ( (char) ch );
   }

   public char charValue () {
      return character.charValue ();
   }

   public String toString () {
      return "" + character;
   }
}

/*
 * Class Name: hw2
 * Description: Reads strings and integers from the user, processes them, 
 *              and prints them back to the user.
 */
public class hw2 {
   private static final int ASCII_ZERO = 48;
   private static final int COUNT = 8;		// # of hex digits

   private static final int CR = 13;		// Carriage Return
   private static final int MAXLENGTH = 80;	// Max string length

   private static final int EOF = -1;		// process End Of File

   private static final int DECIMAL = 10;		// to indicate base 10
   private static final int HEX = 16;		// to indicate base 16

   private static final char digits[] = 	// for ASCII conversion
   new String("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ").toCharArray();

   private static final String DEBUG_GETALINE = 
   "[*DEBUG:  The length of the string just entered is ";

   private static final String DIGIT_STRING = "digit ";
   private static final String REENTER_NUMBER ="\nPlease reenter number: ";
   private static final String OUT_OF_RANGE = " out of range!!!\n";
   private static final String CAUSED_OVERFLOW = " caused overflow!!!\n";
   private static final String DEBUG_WRITELINE =
   "\n[*DEBUG:  The length of the string displayed is ";

   private static Stack<MyLibCharacter> InStream =
   new Stack<MyLibCharacter>();

   private static boolean debug_on = false;
   private static int hexCounter = 0; // counter for the number hex digits

   private static final int NUMARRAYDIGITS = 1000; //size of array in baseout
   
   //difference between array index and string length in getaline
   private static final int LENGTHDISCREP = 2; 

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
   * Clears the System.in buffer when there are extra characters.
   *
   * @param    character   The character to be removed from System.in buffer
   */
   public static void clrbuf (int character) {
      //loops through System.in and grabs the characters to clear the buffer
      while (character != '\n')
      {
         character = fgetc(System.in);
      }
   }

   /**
   * Takes in an inputted number, converts it from characters into an int, 
   * and returns that number.
   *
   * @return     <code> int </code> - the number that was inputted
   */
   public static int decin() {
      int character; //holds character 
      int number = 0; //holds the number to be returned
      int oldnumber = 0; //used to compare and check for overflow

      //keep grabbing characters until the user enters the new line character
      do{
         character = fgetc(System.in); //grabs the next character

	 //ends the program if the character is EOF
	 if(character == EOF)
	 {
            return EOF;
	 }

	 //if the character is not a digit, return an error and prompt
	 //the user to enter a new number
	 if((character > '9' || character < '0') && 
            (character != 10 && character != 13))
	 {
	    digiterror(character, OUT_OF_RANGE);
	    number = 0;
	 }

	 //if the character is a digit, add it to the number to be returned
	 else if(character <= '9' && character >= '0')
	 {	 
	    number = number * DECIMAL;
            number = number + ((char)character - '0');

	    //checks if the number is too big by comparing it to before and 
	    //after values and outputs an error if it is
	    if(((number - ((char)character - '0')) / DECIMAL) !=  oldnumber)
	    {
               digiterror(character, CAUSED_OVERFLOW);
	       number = 0;
	    }
	 }

	 //sets the number to be compared to next
	 oldnumber = number;

      }while(character != '\n');

      return number; //returns the inputted number
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


  /*--------------------------------------------------------------------------
    Method Name:            digiterror
    Purpose:                This function handles erroneous user input.
    Description:            This function  displays and error message to the 
                            user, and asks for fresh input.
    Input:                  character:  The character that began the problem.
			    message:  The message to display to the user.
    Result:                 The message is displayed to the user.
			    The result in progress needs to be set to 0 in
			    decin after the call to digiterror.
   -------------------------------------------------------------------------*/
   public static void digiterror (int character, String message) {

      //handle error
      clrbuf (character);

      //output error message
      writeline (DIGIT_STRING, System.err);
      fputc ( (char)character, System.err);
      writeline (message, System.err);

      writeline (REENTER_NUMBER, System.err);
   }

   /**
   * Takes in user's input and saves it in an array of chars, which 
   * can then be outputted. 
   *
   * @param      message     The array the input is saved in.
   * @param      maxlength   The max amount of characters inputted that can
   *                         be stored in the array, including the last
   *                         null character.
   * @return     <code> int </code> - the length of the saved input
   */
   public static int getaline( char message[], int maxlength ) {
      int character; //holds the next character to be saved
      int index = 0; //index where the next char will be stored in the array
      
      //loops while the next inputted character isn't the new line char
      do{
         character = fgetc(System.in); //grabs the next character

	 message[index] = (char)character; //stores the character
	 index++;

	 //quits the program if the user enters EOF
	 if(character == EOF)
	 {
            return EOF;
	 }
	
	 //clears the rest of the input buffer if the user types more than
	 //maxlength characters
	 if(index == maxlength - 1)
	 {
	   clrbuf(character);
	   break;
	 }
      }while (character != '\n');

      //inserts null characters into the rest of the array 
      //returns length of inputted message
      //runs when the user goes over maxlength inputs
      //will print debug message if option is set
      if((index == maxlength - 1) && (message[index - 1] != '\n'))
      {
	 for(int i = index; i < message.length; i++)
	 {	 
            message[i] = '\0';
	 }
	    
	 if(debug_on) 
         {
            System.err.print(DEBUG_GETALINE);
            decout(index, System.err);
            fputc(']', System.err);
            fputc('\n', System.err);
         }

	 return index;
      }

      //inserts null characters into the rest of the array 
      //returns length of inputted message
      //runs when the user enters less than maxlength inputs
      //will print debug message if option is set
      else
      {
	 for(int i = index - 1; i < message.length; i++)
	 {	 
            message[i] = '\0';
	 }

	 if(debug_on) 
         {
            System.err.print(DEBUG_GETALINE);
            decout(index - 1, System.err);
            fputc(']', System.err);
            fputc('\n', System.err);
         }

	 return index - 1;
      }      
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
   * Returns a character from the input stream.
   *
   * @return  <code>char</code> 
   */
   public static int fgetc(InputStream stream) {

      char ToRet = '\0';

      // Check our local input stream first.
      // If it's empty read from System.in
      if (InStream.isEmpty ()) {

         try {
            // Java likes giving the user the
            // CR character too. Dumb, so just 
            // ignore it and read the next character
            // which should be the '\n'.                  
            ToRet = (char) stream.read ();
            if (ToRet == CR) {
               ToRet = (char) stream.read ();
            }  
            
	    // check for EOF
            if ((int) ToRet == 0xFFFF) {
               return EOF;
            }
	 }

         // Catch any errors in IO.
         catch (EOFException eof) {

            // Throw EOF back to caller to handle
            return EOF;
         }

         catch (IOException ioe) {
            writeline ("Unexpected IO Exception caught!\n",
                                                        System.out);
            writeline (ioe.toString (), System.out);
         }

      }

      // Else just pop it from the InStream.
      else {
         ToRet = ((MyLibCharacter) InStream.pop ()).charValue ();
      }

      return ToRet;
   }


   /**
   * Displays a single character.
   *
   * @param    CharToDisp    Character to display.
   * @param    stream        Where to display.
   */
   public static void fputc(char CharToDisp, PrintStream stream) {

      // Print a single character.
      stream.print (CharToDisp);   

      // Flush the system.out buffer, now. 
      stream.flush ();
   }


   /**
   * Prints out a newline character.
   *
   * @param    stream  Where to display, likely System.out or System.err.
   */
   public static void newline ( PrintStream stream ) {
      //outputs a new line character  
      fputc('\n', stream);
   }


   /**
   * Prints out a string. Will print debug messages if option is set.
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

      //prints debug statement if option is set
      if(debug_on) 
      {
         System.err.print(DEBUG_WRITELINE);
         decout(index, System.err);
         fputc(']', System.err);
         fputc('\n', System.err);
      }      

      return message.length(); //returns length of message
   }


   /**
   * Places back a character into the input stream buffer.
   *
   * @param    A character to putback into the input buffer stream.
   */
   public static void ungetc (int ToPutBack) {

      //push the char back on our local input stream buffer.
      InStream.push (new MyLibCharacter (ToPutBack));
   }


   public static void main( String[] args ) {

      char buffer[] = new char[MAXLENGTH];       //to hold string

      int number;                  //to hold number entered
      int strlen;                  //length of string

      //initialize debug states
      debug_on = false;

      //check command line options for debug display
      for (int index = 0; index < args.length; ++index) {
         if (args[index].equals("-x")) {
	   debug_on = true;
	 }
      } 

      //infinite loop until user enters ^D
      while (true) {
         writeline ("\nPlease enter a string:  ", System.out);

         strlen = getaline (buffer, MAXLENGTH);
	 newline (System.out);

	 //check for end of input
	 if ( EOF == strlen ) {
	    break;
	 }

	 writeline ("The string is:  ", System.out);
	 writeline ( new String(buffer), System.out);

	 writeline ("\nIts length is ", System.out);
	 decout (strlen, System.out);
	 newline (System.out);

	 writeline ("\nPlease enter a decimal number:  ", System.out);
	 if ((number = decin ()) == EOF) {
	    break;
	 }

         writeline ("Number entered is:  ", System.out);
	 decout (number, System.out);

	 writeline ("\nAnd in hexidecimal is:  ", System.out);
	 hexout (number, System.out);

         writeline ("\nNumber entered multiplied by 8 is:  ", System.out);
	 decout (number << 3, System.out);
	 writeline ("\nAnd in hexidecimal is:  ", System.out);
	 hexout (number << 3, System.out);

       	 newline (System.out);
      }
   }
}
