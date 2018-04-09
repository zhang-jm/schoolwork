/****************************************************************************

                                                        Jamie Zhang
                                                        CSE 12, Winter 2015
                                                        January 12, 2015
                                                        cs12xre
                                Assignment Two

File Name:      hw2.c
Description:    This program reads strings and integers from the user,
                processes them, and prints them back to the user.  Program
                terminates when user enters ^D.  At termination, program
                outputs sizes of various types of C/C++ pre defined types.

****************************************************************************/

/* declare fputc/fgetc */
#include <stdio.h>
#include <getopt.h>

/* define some program-wide constants */
#define ASCII_ZERO '0'
#define COUNT ((int) (sizeof (int) << 1))
#define ERROR -1
#define FALSE 0
#define MAXLENGTH 80
#define OFFSET ('a' - 'A')
#define SPACE ' '
#define TRUE 1

#define DECIMAL 10
#define HEX 16

/* define the keyword "NULL" as 0 */
#ifdef NULL
#undef NULL
#endif
#define NULL 0

/* declarations for functions defined in this file */
void baseout (int, int, FILE *);
void clrbuf (int);
int decin (void);
void decout (unsigned int, FILE *);
void digiterror (int, int *, const char *);
int getaline (char *, int);
void hexout (unsigned int, FILE *);
void newline (FILE *);
int writeline (const char *, FILE *);

/* array for input checking and for output */
const char digits[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* messages */
const char CAUSED_OVERFLOW[] =  " caused overflow!!!\n";
const char DIGIT_STRING[] = "digit ";
const char REENTER_NUMBER[] = "\nPlease reenter number: ";
const char OUT_OF_RANGE[] = " out of range!!!\n";

/* debug messages */
const char DEBUG_GETALINE[] =
        "[*DEBUG:  The length of the string just entered is ";
const char DEBUG_WRITELINE[] =
        "\n[*DEBUG:  The length of the string displayed is ";
static int debug_on = FALSE;


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
       /* first digit to be printed is at this index */		
       arrayIndex = COUNT - 1;
  
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

   else
   {
       /* first digit to be printed is at this index */	
       int lastDigitIndex = arrayIndex - 1;

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
Function Name:          clrbuf 
Purpose:                When called, this function will clear stdin. 
Description:            This function checks to see if the incoming 
                        parameter is already '\n' indicating that stdin 
                        is already clear.  If stdin is already clear, this 
                        function does nothing.  Otherwise, this function 
                        calls "fgetc" in a loop until stdin is clear. 
Input:                  character:  the most recent character received from a 
                                previous call to fgetc. 
Result:                 stdin cleared.  Nothing returned. 
--------------------------------------------------------------------------*/  
void clrbuf (int character) {  
   /* loops through stdin and grabs the characters to clear the buffer */
   while (character != '\n')
   {
      character = fgetc(stdin);
   }	
}


/*--------------------------------------------------------------------------
Function Name:          decin
Purpose:                This function accepts integer input from from the user.
Description:            This function processes user input in a loop that ends
                        when the user enters either a valid number or EOF.
                        If EOF is entered, EOF is returned.  Otherwise each
                        character entered is checked to verify that it is
                        numeric.  Non-numeric input is identified, the user
                        is notified, reprompted, and the loop begins again.
                        Once the input is verified to be valid, a series
                        of multiplication by 10 and addition can take
                        place to convert the ASCII characters entered into
                        a numeric quantity.
Input:                  None.
Result:                 The number entered or EOF.
--------------------------------------------------------------------------*/
int decin (void) {
   int character; /* holds character */
   int number = 0; /* holds the number to be returned */
   int oldnumber = 0; /* used to compare and check for overflow */

   /* keep grabbing characters until the user enters the new line character */
   do{
      character = fgetc(stdin); /* grabs the next character */

      /* ends the program if the character is EOF */
      if(character == EOF)
      {
         return EOF;
      }

      /* if the character is not a digit, return an error and prompt
      the user to enter a new number */
      if((character > 57 || character < 48) && 
      (character != 10 && character != 13))
      {
         digiterror(character, &number, OUT_OF_RANGE);
      }

      /* if the character is a digit, add it to the number to be returned */
      else if(character <= 57 && character >= 48)
      {	 
         number = number * DECIMAL;
         number = number + ((char)character - '0');

	 /* checks if the number is too big by comparing it to before and 
	 after values and outputs an error if it is */
	 if(((number - ((char)character - '0')) / DECIMAL) !=  oldnumber)
	 {
            digiterror(character, &number, CAUSED_OVERFLOW);
	 }
      }

      /* sets the number to be compared to next */
      oldnumber = number;

   }while(character != '\n');

   return number; /* returns the inputted number */
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
   /* calls baseout with base 10 */
   baseout(number, DECIMAL, stream);
}


/*--------------------------------------------------------------------------
Function Name:          digiterror
Purpose:                This function handles erroneous user input.
Description:            This function reinitializes sum, displays and
                        error message to the user, and asks for fresh
                        input.
Input:                  character:  The character that began the problem.
                        sum:  A pointer to the sum to reinitialize.
                        message:  The message to display to the user.
Result:                 The message is displayed to the user.  sum is 0.
--------------------------------------------------------------------------*/
void digiterror (int character, int * sum, const char * message) {

	/* handle error */
	clrbuf (character);

	/* reset sum */
	*sum = 0;

	/* output error message */
	writeline (DIGIT_STRING, stderr);
        fputc ((char)character, stderr);
	writeline (message, stderr);

	writeline (REENTER_NUMBER, stdout);
}


/*--------------------------------------------------------------------------
Function Name:          getaline
Purpose:                This function will read a string from the user.
Description:            This function gets input from the user via
                        calls to fgetc up to some maximum number of
                        characters.  Input is terminated when either the
                        maximum number of characters are entered, or
                        a newline character is detected.  If the user
                        enters more characters than the maximum, clrbuf
                        is called to remove extra characters which are
                        ignored.  Since this is routine accepts input,
                        if EOF is detected EOF is passed back to the main
                        program.
Input:                  message:  the destination array where input is stored.
                        maxlength:  the maximum number of non-NULL characters
                                allowed in the string + 1 for the NULL char.
Result:                 User input is stored in message.
                        EOF is returned when the user enters ^D.
                        Otherwise, the length of the message is returned.
--------------------------------------------------------------------------*/
int getaline (char * message, int maxlength) {
   int character; /* holds the next character to be saved */
   int index = 0; /* index where the next char will be stored in the array */
      
   /* loops while the next inputted character isn't the new line char */
   do{
      character = fgetc(stdin); /* grabs the next character */

      message[index] = (char)character; /* stores the character */
      index++;

      /* quits the program if the user enters EOF */
      if(character == EOF)
      {
         return EOF;
      }
	
      /* clears the rest of the input buffer if the user types more than
      maxlength characters */
      if(index == maxlength - 1)
      {
         clrbuf(character);
         break;
      }

   }while (character != '\n');

   /* inserts null characters into the rest of the array 
   returns length of inputted message
   runs when the user goes over maxlength inputs
   will print debug message if option is set */
   if((index == maxlength - 1) && (message[index - 1] != '\n'))
   {
      message[index] = '\0';	

      if(debug_on) 
      {
      /* debug */
      fprintf(stderr, DEBUG_GETALINE);
      decout(index, stderr);
      fputc(']', stderr);
      fputc('\n', stderr);
      }

      return index;
   }

   /* inserts null characters into the rest of the array 
   returns length of inputted message
   runs when the user enters less than maxlength inputs
   will print debug message if option is set */
   else
   {
      message[index - 1] = '\0';

      if(debug_on) 
      {
      /* debug */
      fprintf(stderr, DEBUG_GETALINE);
      decout((index - 1), stderr);
      fputc(']', stderr);
      fputc('\n', stderr);
      }

      return index - 1;
   }      
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
   baseout (number, HEX, stream);
}


/*--------------------------------------------------------------------------
Function Name:         newline
Purpose:               Used to print a new line to the parameter stream.
Description:           Calls fputc to display a new line character in the 
                       parameter stream.
Input:                 stream - where to display (stdout, stderr, etc)
Result:                A new line character is displayed.
                       void - no return value
--------------------------------------------------------------------------*/
void newline (FILE * stream) {
   /* Prints a new line character. */
   fputc('\n', stream);
}


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

   /* prints message */
   while(message[index]) {
      fputc(message[index], stream);
      index++;
   }

   /* prints debug message if option is set */
   if(debug_on) 
   {
   fprintf(stderr, DEBUG_WRITELINE);
   decout(index, stderr);
   fputc(']', stderr);
   fputc('\n', stderr);
   }

   return index; /* returns length of message */
}


/*--------------------------------------------------------------------------
Function Name:          main
Description:            This function asks for input and displays output
Purpose:                This program asks the user to enter a string
                        and a number.  Computations are performed on the 
                        strings and numbers, and the results are displayed.
Description:            This function asks for input and displays output
                        in an infinite loop until EOF is detected.  Once EOF
                        is detected, the lengths of the types are displayed.
Input:                  None.
--------------------------------------------------------------------------*/
int main (int argc, char *const* argv) {
   char buffer[MAXLENGTH];      /* to hold string */
   int number;                  /* to hold number entered */
   int strlen;                  /* length of string */

   int array[10];               /* to show user where memory is allocated */
   int * ap = array;		/* to show user about addresses in memory */
   int ** app = &ap;		/* to show user about addresses in memory */
   int * apx = &array[0];	/* to show user about addresses in memory */
   char option;                 /* the command line option */

   /* initialize debug states */
   debug_on = FALSE;

   /* check command line options for debug display */
   while ((option = getopt (argc, argv, "x")) != EOF) {
      switch (option) {
 	 case 'x': debug_on = TRUE; break;
      }
   }

   /* infinite loop until user enters ^D */
   while (1) {
      writeline ("\nPlease enter a string:  ", stdout);
      strlen = getaline (buffer, MAXLENGTH);
      newline (stdout);

      /* check for end of input */
      if (strlen == EOF)
         break;

      writeline ("The string is:  ", stdout);
      writeline (buffer, stdout);

      writeline ("\nIts length is ", stdout);
      decout (strlen, stdout);
      newline (stdout);

      writeline ("\nPlease enter a decimal number:  ", stdout);
      if ((number = decin ()) == EOF)
   	 break;

      writeline ("Number entered is:  ", stdout);
      decout (number, stdout);

      writeline ("\nAnd in hexidecimal is:  ", stdout);
      hexout (number, stdout);

      writeline ("\nNumber entered multiplied by 8 is:  ", stdout);
      decout (number << 3, stdout);
      writeline ("\nAnd in hexidecimal is:  ", stdout);
      hexout (number << 3, stdout);

      newline (stdout);
   }

   writeline ("\nThe value of ap is:  ", stdout);
   decout ((int) ap, stdout);
   writeline ("\nAnd in hexidecimal is:  ", stdout);
   hexout ((int) ap, stdout);
   newline (stdout);

   writeline ("The value of app is:  ", stdout);
   decout ((int) app, stdout);
   writeline ("\nAnd in hexidecimal is:  ", stdout);
   hexout ((int) app, stdout);
   newline (stdout);

   writeline ("The value of apx is:  ", stdout);
   decout ((int) apx, stdout);
   writeline ("\nAnd in hexidecimal is:  ", stdout);
   hexout ((int) apx, stdout);
   newline (stdout);

   writeline ("The value of ap + 1 is:  ", stdout);
   decout ((int) (ap+1), stdout);
   writeline ("\nAnd in hexidecimal is:  ", stdout);
   hexout ((int) (ap+1), stdout);
   newline (stdout);

   writeline ("The address of array[0] is:  ", stdout);
   decout ((int) &array[0], stdout);
   newline (stdout);

   writeline ("The address of array[1] is:  ", stdout);
   decout ((int) &array[1], stdout);
   newline (stdout);

   writeline ("The size of a float is:  ", stdout);
   decout (sizeof (float), stdout);
   newline (stdout);

   writeline ("The size of a double is:  ", stdout);
   decout (sizeof (double), stdout);
   newline (stdout);

   writeline ("The size of a long double is:  ", stdout);
   decout (sizeof (long double), stdout);
   newline (stdout);

   writeline ("The size of a char is:  ", stdout);
   decout (sizeof (char), stdout);
   newline (stdout);

   writeline ("The size of an int is:  ", stdout);
   decout (sizeof (int), stdout);
   newline (stdout);

   writeline ("The size of a short is:  ", stdout);
   decout (sizeof (short), stdout);
   newline (stdout);

   writeline ("The size of a short int is:  ", stdout);
   decout (sizeof (short int), stdout);
   newline (stdout);

   writeline ("The size of a long is:  ", stdout);
   decout (sizeof (long), stdout);
   newline (stdout);

   writeline ("The size of a long int is:  ", stdout);
   decout (sizeof (long int), stdout);
   newline (stdout);
  
   writeline ("The size of a long long is:  ", stdout);
   decout (sizeof (long long), stdout);
   newline (stdout);

   writeline ("The size of a signed is:  ", stdout);
   decout (sizeof (signed), stdout);
   newline (stdout);

   writeline ("The size of a signed char is:  ", stdout);
   decout (sizeof (signed char), stdout);
   newline (stdout);

   writeline ("The size of a signed short is:  ", stdout);
   decout (sizeof (signed short), stdout);
   newline (stdout);

   writeline ("The size of a signed short int is:  ", stdout);
   decout (sizeof (signed short int), stdout);
   newline (stdout);

   writeline ("The size of a signed int is:  ", stdout);
   decout (sizeof (signed int), stdout);
   newline (stdout);

   writeline ("The size of a signed long is:  ", stdout);
   decout (sizeof (signed long), stdout);
   newline (stdout);

   writeline ("The size of a signed long int is:  ", stdout);
   decout (sizeof (signed long int), stdout);
   newline (stdout);

   writeline ("The size of a signed long long is:  ", stdout);
   decout (sizeof (signed long long), stdout);
   newline (stdout);

   writeline ("The size of an unsigned is:  ", stdout);
   decout (sizeof (unsigned), stdout);
   newline (stdout);

   writeline ("The size of an unsigned char is:  ", stdout);
   decout (sizeof (unsigned char), stdout);
   newline (stdout);

   writeline ("The size of an unsigned short is:  ", stdout);
   decout (sizeof (unsigned short), stdout);
   newline (stdout);

   writeline ("The size of an unsigned short int is:  ", stdout);
   decout (sizeof (unsigned short int), stdout);
   newline (stdout);

   writeline ("The size of an unsigned int is:  ", stdout);
   decout (sizeof (unsigned int), stdout);
   newline (stdout);

   writeline ("The size of an unsigned long is:  ", stdout);
   decout (sizeof (unsigned long), stdout);
   newline (stdout);

   writeline ("The size of an unsigned long int is:  ", stdout);
   decout (sizeof (unsigned long int), stdout);
   newline (stdout);

   writeline ("The size of an unsigned long long is:  ", stdout);
   decout (sizeof (unsigned long long), stdout);
   newline (stdout);

   writeline ("The size of a void pointer is:  ", stdout);
   decout (sizeof (void *), stdout);
   newline (stdout);

   writeline ("The size of a character pointer is:  ", stdout);
   decout (sizeof (char *), stdout);
   newline (stdout);

   writeline ("The size of an int pointer is:  ", stdout);
   decout (sizeof (int *), stdout);
   newline (stdout);

   writeline ("The size of a long pointer is:  ", stdout);
   decout (sizeof (long *), stdout);
   newline (stdout);

   writeline ("The size of a float pointer is:  ", stdout);
   decout (sizeof (float *), stdout);
   newline (stdout);

   writeline ("The size of a double pointer is:  ", stdout);
   decout (sizeof (double *), stdout);
   newline (stdout);
 
   writeline ("The size of a long double pointer is:  ", stdout);
   decout (sizeof (long double *), stdout);
   newline (stdout);
 
   newline (stdout);

   return 0;
}
