/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Jan 25, 2015
                                                        cs12xre
                                Assignment Four

File Name:      calc.c
Description:    This program uses two stacks to manipulate a series of 
                positive numbers and operators and return the result
		of that expression as a single number. The operations that 
		the calculator can evaluate are addition, subtraction, 
		multiplication, division, exponents, and factorials.
****************************************************************************/

#include <ctype.h>
#include <stdio.h>
#include "calc.h"
#include "mylib.h"
#include "stack.h"

#define CHARACTER(word) ((char) ((word) & 0x000000FF))
#define INDEX(word) (((word) & 0x0000FF00) >> 8)
#define PRIORITY(word) ((word) & 0x0000FE00)
#define FACT_OP2 1 /* op2/second param for the factorial function */
#define STACK_SIZE_INDEX (-2) /* index of the space holding stack size */
#define BYTE 8 /* size of byte */
#define SIGN_BIT 0x80000000 /* sign bit used to distinguish operators from 
			       numbers */

static char operators[] = "()+-*/^ !";

static long add (long, long);
static long divide (long, long);
static long exponent (long, long);
static long fact (long, long);
static long mult (long, long);
static long setupword (int);
static long sub (long, long);

static long (*functions[]) (long, long) =
    { NULL, NULL, add, sub, mult, divide, exponent, NULL, fact };


/*--------------------------------------------------------------------------
Function Name:         eval
Purpose:               Evaluates the expression that was entered.
Description:           A local stack stack2 is created and the numbers and 
                       operators from stack1 are reversed onto this new stack. 
		       The new stack is repeatedly popped until empty. For 
		       each pop, if what was popped was a number, it gets 
		       pushed onto stack1. If what was popped was the 
		       factorial operator, one number is popped from stack2, 
		       the factorial of that number is evaluated, and the 
		       result is pushed back onto stack1. If what was popped
		       was any other operator, two numbers are popped from 
		       stack1, the operation is performed, and the result
		       is popped back onto stack1. Eventually, only 1 result
		       remains on stack1, which is the final result to be
		       returned.
Input:                 stack1 - a pointer to the stack that holds the
                                numbers and operators to be evaluated
				in "postfix" order	
Result:                The result of the expression is returned
                       long - the final result of the expression
--------------------------------------------------------------------------*/
long eval (Stack * stack1) {

   long result; /* the result to be returned */

   long op1; /* the first operand */
   long op2; /* the second operand */

   /* creates a local stack */
   Stack * stack2 = new_Stack(stack1[STACK_SIZE_INDEX]); 

   /* reverses stack 1 onto stack 2 */
   while(!isempty_Stack(stack1))
   {
      long popped; /* holds the number or operator popped from stack 1 */

      /* pops number from stack1 and pushes onto stack2 */
      pop(stack1, &popped); 
      push(stack2, popped);
   }

   /* evaluates the expression */
   while(!isempty_Stack(stack2))
   {
      long popped; /* holds the number or operator popped from stack2 */

      pop(stack2, &popped); /* pops the number or operator from stack2 */

      /* checks if what was popped was a number and pushes it onto stack 1
         if it was */
      if(popped >= 0)
      {
         push(stack1, popped);
      }

      /* if what was popped was the factorial operator, only one number is
         popped from stack1 and the factorial operation is performed */
      else if(CHARACTER(popped) == '!')
      {
         pop(stack1, &op1);

	 /* factorial operation */
	 result = functions[INDEX(popped)](op1, FACT_OP2);

	 push(stack1, result); /* push result back onto stack1 */
      }

      /* if what was popped was any other operator, two numbers are popped 
       * from stack1 and the operation is performed using the numbers */
      else
      {
         pop(stack1, &op1);
	 pop(stack1, &op2);

	 /* performs operation based on what the popped operator was */
	 result = functions[INDEX(popped)](op1, op2);

	 push(stack1, result); /* push result back onto stack1 */
      }
   }

   delete_Stack(&stack2); /* deallocates stack2 */

   if(!isempty_Stack(stack1))
   {
      pop(stack1, &result); /* pops the final result and returns it */
      return result;   
   }
   else
   {
      return 0;
   }
}


/*--------------------------------------------------------------------------
Function Name:         intopost
Purpose:               Takes the input and converts it into "postfix" order.
Description:           A local stack stack2 is created. A while loop 
                       repeatedly calls fgetc until all characters have been
		       retrieved from stdin. If a character is EOF, the
		       program ends. If the character is a space, the program
		       ignores it. If the character is a digit, ungetc is 
		       called to push the character back onto stdin and then
		       decin is called to retrieve the whole number and push
		       it onto stack1. If the character is '(', it gets pushed
		       onto stack2. If the character is ')', stack2 is
		       repeatedly popped and pushed onto stack1 until the 
		       matching '(' is found. If the character is any other 
		       operator, stack2 is popped and pushed onto stack1 until
		       the priority of the next operator in stack2 is lower
		       than the character's priority or stack2 is empty. The
		       character is then pushed onto stack2. After all the
		       characters are retrieved from stdin, anything remaining
		       on stack2 is pushed to stack1. 
Input:                 stack1 - a pointer to the stack to hold the numbers
                                and operators in postfix order, reversed
Result:                stack1 holds the expression in post-fix order, in 
                       reverse
                       long - returns a non-zero value if intopost succeeded,
		              0 if failed, 
		              EOF if "^D" was entered
--------------------------------------------------------------------------*/
long intopost (Stack * stack1) {

   /* creates a local stack */
   Stack * stack2 = new_Stack(stack1[STACK_SIZE_INDEX]); 

   long numberToPush; /* holds a number returned from decin to be pushed */
   char character = fgetc(stdin); /* holds the character grabbed from stdin */


   while(character != '\n')
   {
      /* ends program if user enters EOF */
      if(character == EOF)
      {
         delete_Stack(&stack2);	/* deallocates memory */

         return EOF;
      }
     
      /* does nothing if the character is a space */
      else if(character == ' ')
      {

      }
     
      /* if the character is a digit */	 
      else if(character >= '0' && character <= '9') 
      {
	 /* sends the character back to stdin and grabs the whole number
	    using decin, then pushes it onto stack1 */
         ungetc(character, stdin);
	 numberToPush = decin();

	 push(stack1, numberToPush);
      }

      /* pushes character onto stack2 if it's a left parenthesis */
      else if(character == '(')
      {
         push(stack2, character);
      }

      
      else if(character == ')')
      {
         long poppedOper; /* holds the operator popped from stack2 */
         pop(stack2, &poppedOper); /* pops an operator */

         while(poppedOper != '(')
         {
	    /* pushes the popped operator onto stack1 and pops a new one 
	      from stack2 until the left parenthesis is found */
            push(stack1, setupword(poppedOper));
            poppedOper = 0; /* resets the popped operator */

            pop(stack2, &poppedOper);
         }
      }

      /* if the character from stdin is any other operator */
      else
      {
         long poppedOper; /* holds the operator popped from stack2 */

	 /* keeps popping while stack2 isn't empty */
         while(!isempty_Stack(stack2))
         {
            pop(stack2, &poppedOper);

	    /* checks if the priority of the popped operator is greater than
	       the priority of the character and if there are still operators
	       left on stack2 */
            if((PRIORITY(setupword(character)) < 
	       PRIORITY(setupword(poppedOper)))&& 
	       !isempty_Stack(stack2))
            {
               /* pushes the popped operator onto stack1, and pops a new 
		  operator from stack2 */
 	       push(stack1, setupword(poppedOper));
               poppedOper = 0;  /* resets the popped operator */

               pop(stack2, &poppedOper);
            }

	    /* checks if the priority of the popped operator is greater than
	       the priority of the character and if there are no more 
	       operators left on stack2 */
	    else if((PRIORITY(setupword(character)) < 
	            PRIORITY(setupword(poppedOper))) && 
	            isempty_Stack(stack2))
	    {
	       /* pushes the popped operator onto stack1 and resets 
		  poppedOper */
               push(stack1, setupword(poppedOper));
	       poppedOper = 0;
	    }

	    /* if the priority of the popped operator was less than the
	       priority of the character */
	    else
	    {
	       /* pushes the popped operator and the character back onto
		  stack2 and resets poppedOper */
               push(stack2, poppedOper);
	       poppedOper = 0;

               push(stack2, character);
	       break; /* breaks out of the while loop */
	    }
         }

	 /* if the stack is empty and a '(' has been popped */
	 if(isempty_Stack(stack2) && (poppedOper == '('))
	 {
	    /* pushes the '(' and the character back onto
	       stack2 and resets poppedOper */		 
            push(stack2, poppedOper);		 
            push(stack2, character);

            poppedOper = 0;	    
	 }

	 /* if the stack is empty, pushes the character onto stack2 */
	 else if(isempty_Stack(stack2))
	 {
           push(stack2, character);	
	 }	   
      }

      character = fgetc(stdin); /* grabs the next character from the buffer */
   }

   /* if all characters have been grabbed from stdin but stack2 isn't empty
      move characters from stack2 to stack1 until stack2 is empty */
   while(!isempty_Stack(stack2))    
   {
      long charToPush; /* holds the character popped from stack2 */
     
      /* moves characters from stack2 to stack1 */
      pop(stack2, &charToPush); 
      push(stack1, setupword(charToPush));
   }

   delete_Stack(&stack2); /* deallocates stack2 */
   
   return 1; /* return 1 if intopost was successful */
}


/*--------------------------------------------------------------------------
Function Name:         add
Purpose:               Adds two numbers together.
Description:           Takes in two parameters and adds them together.
Input:                 augend, addend - two numbers to be added to each other
Result:                Returns the result of the addition.
                       long - the result of the addition
--------------------------------------------------------------------------*/
static long add (long augend, long addend) {
    return augend + addend;
}


/*--------------------------------------------------------------------------
Function Name:         divide
Purpose:               Divides two numbers.
Description:           Takes in two parameters and divides them.
Input:                 divisor, dividend - two numbers to be divided
                                           dividend / divisor
Result:                Returns the result of the division.
                       long - the result of the division
--------------------------------------------------------------------------*/
static long divide (long divisor, long dividend) {
    return dividend / divisor;
}


/*--------------------------------------------------------------------------
Function Name:         exponent
Purpose:               Calculates the value of a number to some exponent.
Description:           Uses a for loop to multiply the base by itself 
                       exponent number of times.
Input:                 power - the exponent
                       base - the number to be raised to some power
Result:                Returns the result.
                       long - the value of number ^ exponent
--------------------------------------------------------------------------*/
static long exponent (long power, long base) {
   /* holds the value to be returned 
      set to 1 to be able to multiply it to base
      also base ^ 0 = 1 */
   long valueToReturn = 1; 

   /* for loop multiplying the base by itself power number of times */
   int i;
   for (i = 0; i < power; i++)
   {
      valueToReturn = valueToReturn * base;
   }

   return valueToReturn;
}


/*--------------------------------------------------------------------------
Function Name:         fact
Purpose:               Calculates the factorial of some number.
Description:           Uses a for loop to multiply all numbers up to some 
                       number together.
Input:                 xxx - the number to calculate the factorial of
                       ignored - any number; not used in calculations
Result:                Returns the result of the factorial.
                       long - the value of xxx factorial
--------------------------------------------------------------------------*/
static long fact (long xxx, long ignored) {
   /* the number to be returned 
      set to 1 to be able to multiply it to another number */
   long valueToReturn = 1; 

   /* 0! = 1; just in case 0 is considered a positive integer */
   if (xxx == 0)
   {
      return 1;
   }
   
   /* for xxx > 0 */
   else
   {
      /* for loop multiplying all numbers up to xxx together for factorial 
         too lazy to implement recursion */
      int i;
      for (i = 1; i <= xxx; i++)
      {
         valueToReturn = valueToReturn * i;
      }

      return valueToReturn;
   }
}


/*--------------------------------------------------------------------------
Function Name:         multiply
Purpose:               Multiplies two numbers together.
Description:           Takes in two parameters and multiplies them together.
Input:                 factorx, factory - two numbers to be multiplied 
                                         together
Result:                Returns the result of the multiplication.
                       long - the result of the multiplication
--------------------------------------------------------------------------*/
static long mult (long factorx, long factory) {
    return factorx * factory;
}


/*--------------------------------------------------------------------------
Function Name:         setupword
Purpose:               Sets up the operator so that the computer can 
                       distinguish it from the inputted numbers
Description:           Grabs the index the operator is at in the operators 
                       array. Returns the operator as a negative value
		       (sign bit is 1), the index of the character in the
		       array, and the ASCII code for the character OR'd 
		       together.
Input:                 character - the operator to convert to the format
Result:                Returns 0x80000[index][ASCII code of character]
                       example: 0x8000022B for "+" (2 is the index, 
		                2B is the ASCII code)
                       long - the result of the operator conversion
--------------------------------------------------------------------------*/
static long setupword (int character) {
   int index = 0; /* gets the index of the operator in the operators array */

   /* grabs the index of the operator in the array by going through the
      array and checking if the operator matches the element in the array
      at that index */
   while(operators[index])
   {
      if(character == operators[index])
      {
         break; /* breaks out of the while loop if the operator is found */
      }
      else
      {
         index++;
      }
   }

   return SIGN_BIT | (index << BYTE) | character;
}


/*--------------------------------------------------------------------------
Function Name:         sub
Purpose:               Subtracts two numbers.
Description:           Takes in two parameters and subtracts them.
Input:                 subtrahend, minuend - two numbers to be subtracted
                                             minuend - subtrahend
Result:                Returns the result of the subtraction.
                       long - the result of the subtraction
--------------------------------------------------------------------------*/
static long sub (long subtrahend, long minuend) {
    return minuend - subtrahend;
}

