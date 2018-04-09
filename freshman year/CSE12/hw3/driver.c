/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Jan 19, 2015
                                                        cs12xre
                                Assignment Three

File Name:      driver.c
Description:    This program is used to run and test the functions in 
                stack.c. Takes in user input to determine what function
		to call/how to manipulate the stack(s). Only one stack 
		can be created at a time.
****************************************************************************/

/****************************************************************************
 Answers to Debug Questions

 1. (void *) 0x8048ab5 
 2. (Stack *) 0xbfffeb88
 3. heap
 4. (Stack **) 0xbfffebac; heap
 5. (Stack *) 0x804b014; heap
 6. (Stack *) 0x8048ab5
 7. Yes, it's the same address!

****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "mylib.h"
#include "stack.h"


/*--------------------------------------------------------------------------
Function Name:         main
Purpose:               Used to call the functions in stack.c
Description:           Prompts the user for commands to create, manipulate, 
                       and delete a stack. Takes the user input and uses it
		       to run the functions.
Input:                 argc - number of command-line arguments
                       argv - argument vector holding the arguments
Result:                Stacks are manipulated.
                       int - always returns 0
--------------------------------------------------------------------------*/
int main (int argc, char * const * argv) {

   Stack * main_Stack = 0;       /* the test stack */
   unsigned long amount;         /* numbers of items possible go on stack */
   long command;                 /* stack command entered by user */
   long item = 0;                /* item to go on stack */
   char option;                  /* the command line option */
   long status;                  /* return status of stack functions */
        
   /* initialize debug states */
   debug_off ();

   /* check command line options for debug display */
   while ((option = getopt (argc, argv, "x")) != EOF) {
                
      switch (option) {
         case 'x': debug_on (); break;
      }
   }

   while (1) {
      command = 0;            /* initialize command, need for loops */
      writeline ("\nPlease enter a command:", stdout);
      writeline ("\n\t(a)llocate, (d)eallocate, ", stdout);
      writeline ("p(u)sh, (p)op, (t)op, (i)sempty, (e)mpty, ",stdout);
      writeline ("\n\tis(f)ull, (n)um_elements, (w)rite to stdout, ", stdout);
      writeline ("(W)rite to stderr.\n", stdout);
      writeline ("Please enter choice:  ", stdout);
      command = getchar ();
      if (command == EOF)     /* are we done? */
         break;

      clrbuf (command);       /* get rid of extra characters */

      switch (command) {      /* process commands */

	 case 'a':                 /*allocate */
	    newline(stdout);

	    /* prompts user to enter size of stack */
            writeline("Please enter the number of objects", stdout); 
	    writeline(" to be able to store: ", stdout);
	    
	    amount = (long)decin(); /* grabs the size from stdin */
	    clrbuf(0);
	   
	    /* removes the old main_Stack so that only one stack exists at 
	       a time */
	    if(main_Stack)
	    {
	       delete_Stack(&main_Stack);
	    }

	    main_Stack = new_Stack(amount); /* creates new stack */

            break;	    

	 case 'e':               /* empty_Stack */
	    empty_Stack(main_Stack); /* empties the stack */

	    writeline("Stack is empty.\n", stdout); /* notifies user */

	    break;

	 case 'd':               /* delete_Stack */
	    delete_Stack(&main_Stack); /* deletes the stack */

	    writeline("Stack has been deleted.\n", stdout); /* notification */

	    break;

         case 'f':               /* isfull */
            if (isfull_Stack (main_Stack))
	       writeline ("Stack is full.\n", stdout);
	    else
	       writeline ("Stack is not full.\n", stdout);
	    break;

	 case 'i':               /* isempty */
	    if (isempty_Stack (main_Stack))
	       writeline ("Stack is empty.\n", stdout);
	    else
	       writeline ("Stack is not empty.\n", stdout);
	    break;

	 case 'n':               /* num_elements */
	    writeline ("Number of elements on the stack is:  ", stdout);
	    decout (num_elements (main_Stack));
	    newline (stdout);
	    break;

	 case 'p':               /* pop */
	    status = pop (main_Stack, &item);
	    if (! status)
	       fprintf (stderr,"\nWARNING:  pop FAILED\n");
	    else {
	       writeline ("Number popped from the stack is:  ", stdout);
	       decout (item);
	       newline (stdout);
	    }
	    break;

	 case 't':               /* top */
	    status = top (main_Stack, &item);
	    if (! status)
	       fprintf (stderr,"\nWARNING:  top FAILED\n");
	    else {
	       writeline ("Number at top of the stack is:  ", stdout);
	       decout (item);
	       newline (stdout);
	    }
	    break;

	  case 'u':               /* push */
	     writeline ("\nPlease enter a number to push to stack:  ", 
		         stdout);
	     item = decin ();
	     clrbuf (0);     /* get rid of extra characters */
	     status = push (main_Stack, item);
	     if (! status)
		fprintf(stderr,"\nWARNING:  push FAILED\n");
	     break;

	  case 'w':               /* write */
	     writeline ("\nThe Stack contains:\n", stdout);
	     write_Stack (main_Stack, stdout);
	     break;

	  case 'W':               /* write */
	     writeline ("\nThe Stack contains:\n", stdout);
	     write_Stack (main_Stack, stderr);
	     break;
       }

       if (item == EOF) /* check if done */
          break;
   }

   if (main_Stack)
      delete_Stack (&main_Stack);     /* deallocate stack */
   
   newline (stdout);
   
   return 0;
}

