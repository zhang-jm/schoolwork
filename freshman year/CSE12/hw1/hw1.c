/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Jan 9, 2015
                                                        cs12xre
                                Assignment One

File Name:      hw1.c
Description:    This program tests functions to display output strings and 
                numbers. 
****************************************************************************/
#include <stdio.h>

#define COUNT 8		/* number of hex digits to display */
#define DECIMAL 10	/* to indicate base 10 */
#define HEX 16		/* to indicate base 16 */

/* array used for ASCII conversion */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";


/*--------------------------------------------------------------------------
Function Name:         writeline
Purpose:               Used to output characters to a display.
Description:           Uses a loop to print a character at a time until
                       all the characters are printed.
Input:                 message - what to be displayed
                       stream - where to display (stdout, stderr, etc)
Result:                A message is displayed.
                       int - the length of the message
--------------------------------------------------------------------------*/
int writeline (const char * message, FILE * stream) {
   int index = 0; /* counter used when printing characters */

   /* prints the next character if there are still characters left to 
      print */
   while(message[index]) {
      fputc(message[index], stdout); /* prints chars */
      index++;
   }

   return index; /* returns length of message */
}


/*--------------------------------------------------------------------------
Function Name:         baseout
Purpose:               Takes a number and prints it in a specified base.
Description:           Separates the number into its different digits in the 
                       base, converts the digits from int to char, and puts
		       them into a char array; then prints the digits one
		       at a time to display the number.
Input:                 number - a number to be displayed
                       base - the base the number should be displayed in
		       stream - where to display
Result:                A number in base base is displayed.
                       void - no return value
--------------------------------------------------------------------------*/
void baseout (int number, int base, FILE * stream) {
   int numCopy = number; /* makes a copy of the number */
   int arrayIndex = 0; /* index of array, used to store and print the digits */

   /* creates a large, empty array */
   char arrayDigits[BUFSIZ];

   /* makes the first 8 ints in the array 0s */   
   int i;
   for(i = 0; i < COUNT; i++)
   {
      arrayDigits[i] = digits[0];
   } 

   /* stores digits */
   while(numCopy != 0)
   {
      /* finds the digit by modding, going from back to front and 
         records that digit as a char */      
      arrayDigits[arrayIndex] = digits[numCopy % base];
      /* divides by the base to get the number without the digit that was 
         just found */
      numCopy = numCopy / base;

      /* goes to the next index in the array */
      arrayIndex++;
   }

   /* if arrayIndex was 0 (number is 0), print 0 */
   if(arrayIndex == 0)
   {
       fputc(digits[0], stream);
   }	

   /* if base 16 */
   else if(base == HEX)
   {
       arrayIndex = COUNT - 1; /* first digit is at this index */
  
       while(arrayIndex >= 0)
       {	    
          /* finds and records the digit that needs to be printed next */
          char digitToPrint = arrayDigits[arrayIndex];

          /* calls on fputc to print the digit */
          fputc(digitToPrint, stream);

          /* decrements arrayIndex to print the next digit */
          arrayIndex--;
       }
   }  

   /* any other base */
   else
   {
      /* first digit to be printed is at this index */	
      int lastDigitIndex = arrayIndex - 1;

      /* while the number hasn't finished printing */
      while(lastDigitIndex >= 0)
      {
         /* finds and records the digit that needs to be printed next */
         char digitToPrint = arrayDigits[lastDigitIndex];
 
         /* calls on fputc to print the digit */
         fputc(digitToPrint, stream);

         /* decrements lastDigitIndex to print the next digit */
         lastDigitIndex--;
      }
   }
}


/*--------------------------------------------------------------------------
Function Name:         decout
Purpose:               This function is used to print a number in base 10. 
                       The number is displayed in the parameter stream.
Description:           Calls the baseout function with base 10.
Input:                 number - the number to be displayed
                       stream - where to display the number 
		                (stdout, stderr, etc)
Result:                A number in base 10 is displayed.
                       void - no return value
--------------------------------------------------------------------------*/
void decout (unsigned int number, FILE * stream) {
   /* calls baseout to print a number in base 10 */
   baseout(number, DECIMAL, stream);
}


/*--------------------------------------------------------------------------
Function Name:         hexout
Purpose:               Prints a number in base 16 to the parameter FILE stream
Description:           Goal is achieved via delegating to the baseout function
Input:                 number:  the number to display
                       stream:  where to display, likely stdout or stderr
Result:                Number in base 16 is displayed.
                       No return value.
--------------------------------------------------------------------------*/
void hexout (unsigned int number, FILE * stream) {

   /* Output "0x" for hexidecimal. */
   writeline ("0x", stream);

   /* calls baseout to print a number in base 16 */
   baseout (number, HEX, stream);
}


/*--------------------------------------------------------------------------
Function Name:         newline
Purpose:               Used to print a new line to the parameter stream.
Description:           Calls writeline to display a new line character in the 
                       parameter stream.
Input:                 stream - where to display (stdout, stderr, etc)
Result:                A new line character is displayed.
                       void - no return value
--------------------------------------------------------------------------*/
void newline (FILE * stream) {
   /* Prints a new line character. */
   writeline("\n", stream);
}


int main (int argc, char *const* argv) {
   writeline ("Hello World", stdout);

   fprintf (stderr, "Zdravei Sviat\n");
   newline(stdout);

   decout (123, stdout);
   newline(stdout);

   decout (0, stdout);
   newline(stdout);

   hexout (0xFEEDDAD, stdout);
   newline(stdout);

   return 0;
}
