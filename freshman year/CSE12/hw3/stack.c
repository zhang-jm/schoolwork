/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Jan 19, 2015
                                                        cs12xre
                                Assignment Three

File Name:      stack.c
Description:    This program contains functions that allow a user to
                create a stack, add or remove items from it, check
		what items or how many items are in it, and delete it.
****************************************************************************/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "stack.h"

#define STACK_POINTER_INDEX (-1)        /* Index of next available space */
#define STACK_SIZE_INDEX (-2)           /* Index of size of the stack */
#define STACK_COUNT_INDEX (-3)          /* Index of which stack allocated */
#define STACK_OFFSET 3  /* offset from allocation to where user info begins */

/* catastrophic error messages */
static const char DELETE_NONEXIST[] = "Deleting a non-existent stack!!!\n";
static const char EMPTY_NONEXIST[] = "Emptying a non-existent stack!!!\n";
static const char INCOMING_NONEXIST[] = 
                        "Incoming parameter does not exist!!!\n";
static const char ISEMPTY_NONEXIST[] = 
                        "Isempty check from a non-existent stack!!!\n";
static const char ISFULL_NONEXIST[] = 
                        "Isfull check from a non-existent stack!!!\n";
static const char NUM_NONEXIST[] = 
                        "Num_elements check from a non-existent stack!!!\n";
static const char POP_NONEXIST[] = "Popping from a non-existent stack!!!\n";
static const char POP_EMPTY[] = "Popping from an empty stack!!!\n"; 
static const char PUSH_NONEXIST[] = "Pushing to a non-existent stack!!!\n";
static const char PUSH_FULL[] = "Pushing to a full stack!!!\n";
static const char TOP_NONEXIST[] = "Topping from a non-existent stack!!!\n";
static const char TOP_EMPTY[] = "Topping from an empty stack!!!\n";
static const char WRITE_NONEXIST_FILE[] = 
                    "Attempt to write using non-existent file pointer!!!\n";
static const char WRITE_NONEXIST_STACK[] = 
                    "Attempt to write to a non-existent stack!!!\n";

/* Debug messages */
static const char ALLOCATED[] = "[Stack %ld has been allocated]\n";
static const char DEALLOCATE[] = "[Stack %ld has been deallocated]\n";
static const char HEXPOP[] = "[Stack %ld - Popping 0x%lx]\n";
static const char HEXPUSH[] = "[Stack %ld - Pushing 0x%lx]\n";
static const char HEXTOP[] = "[Stack %ld - Topping 0x%lx]\n";
static const char POP[] = "[Stack %ld - Popping %ld]\n";
static const char PUSH[] = "[Stack %ld - Pushing %ld]\n";
static const char TOP[] = "[Stack %ld - Topping %ld]\n";

/* static variable allocation */
static int debug = FALSE; /* allocation of debug flag */
static int stack_counter = 0; /* number of stacks allocated so far */

/* Debug state methods */
void debug_off (void) {
   debug = FALSE;
}

void debug_on (void) {
   debug = TRUE;
}

/* start of true stack code */

/*--------------------------------------------------------------------------
Function Name:         delete_Stack
Purpose:               Deletes a stack that was created with new_Stack.
Description:           Checks if the stack exists. If it does, the function
                       calls free() to deallocate the memory associated
		       with the stack and sets the stack pointer to NULL.
Input:                 spp - a double pointer to the stack that is going to
                             be deleted
Result:                A warning message is displayed if the stack does not
                       exist. Otherwise, the stack is deleted.
                       void - no return value
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
   /* warning if stack doesn't exist */
   if(!spp || !*spp)
   {
      fprintf(stderr, DELETE_NONEXIST);
   }

   else
   {
      free(*spp - STACK_OFFSET); /* deallocates memory */
      
      *spp = 0; /* sets the stack pointer to null */

      /* debug */
      if(debug)
      {
         fprintf(stderr, DEALLOCATE, (long)stack_counter);
      }

      stack_counter--; /* decrements the number of stacks allocated */
   }
}


/*--------------------------------------------------------------------------
Function Name:         empty_Stack
Purpose:               Empties the stack of all its elements.
Description:           Does not actually empty the stack by removing all its
                       elements. If the stack exists, the value of the next 
		       available space is set to zero so that the old elements
		       can be overridden with new inputs.
Input:                 this_Stack - a pointer to the stack to be emptied
Result:                A warning message is displayed if the stack does not
                       exist. Otherwise, the stack is emptied.
                       void - no return value
--------------------------------------------------------------------------*/
void empty_Stack (Stack * this_Stack) {
   /* warning if the stack doesn't exist */
   if(!this_Stack)
   {
      fprintf(stderr, EMPTY_NONEXIST);
   }

   else
   {
      /* sets the index of the next available space to 0, "empties" stack */   
      this_Stack[STACK_POINTER_INDEX] = 0;
   }
}

        
/*--------------------------------------------------------------------------
Function Name:         isempty_Stack
Purpose:               Checks if the stack is empty, returning TRUE (1) if it 
                       is, and FALSE (0) if it is not.
Description:           Checks if the stack exists. If it does, checks what 
                       the value of the next available space is. If that value
		       is 0, then that means there is nothing in the stack
		       and the stack is empty. Otherwise, the stack is not 
		       empty.
Input:                 this_Stack - a pointer to the stack to be checked
Result:                A warning message is displayed if the stack does not
                       exist. Otherwise, returns 0 or 1 depending on if the
		       stack is empty or not.
                       long - true or false values
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_Stack) {
   /* prints error message if the stack to be checked does not exist */
   if(!this_Stack)
   {
      fprintf(stderr, ISEMPTY_NONEXIST);
      return 1;
   }
   
   /* checks if the stack is empty or not */
   else
   {
     /* empty if next available space is zero */
     if(this_Stack[STACK_POINTER_INDEX] == 0)
     {
       return 1;
     }
     else
     {
       return 0;
     }
   }
}


/*--------------------------------------------------------------------------
Function Name:         isfull_Stack
Purpose:               Checks if the stack is full, returning TRUE (1) if it 
                       is, and FALSE (0) if it is not.
Description:           Checks if the stack exists. If it does, checks what 
                       the value of the next available space is. If that value
		       is equal to the size of the stack (because the index of
		       the last element is one less than the size), that means
                       that the stack is full. Otherwise, the stack is not 
		       full.
Input:                 this_Stack - a pointer to the stack to be checked
Result:                A warning message is displayed if the stack does not
                       exist. Otherwise, returns 0 or 1 depending on if the
		       stack is full or not.
                       long - true or false values
--------------------------------------------------------------------------*/
long isfull_Stack (Stack * this_Stack) {
   /* prints error message if the stack to be checked does not exist */
   if(!this_Stack)
   {
      fprintf(stderr, ISFULL_NONEXIST);
      return 0;
   }
   
   /* checks if stack is full or not */
   else
   {
     /* full if the value of next available space equals stack size */
     if(this_Stack[STACK_POINTER_INDEX] == this_Stack[STACK_SIZE_INDEX])
     {
       return 1;
     }
     else
     {
       return 0;
     }
   }
}


/*--------------------------------------------------------------------------
Function Name:         new_Stack
Purpose:               Allocates and initializes a new Stack object.
Description:           Calls malloc to allocate enough memory to hold a 
                       user-specified number of longs. It then initializes
		       the stack infrastructure by setting the pointer
		       returned by malloc to point to the location in memory
		       of the first storage space in the stack.
Input:                 stacksize - the size of the stack (number of storage
                                   spaces)
Result:                A new stack is created.
                       Stack * - a pointer to the location in memory of the
		                 first storage space in the stack
--------------------------------------------------------------------------*/
Stack * new_Stack (unsigned long stacksize) {
   /* pointer to the location in memory where user input starts */
   Stack * this_Stack; 

   void * memory; /* holds the memory location from malloc */

   stack_counter++; /* increments count of number of stacks */	

   /* allocates memory */
   memory = malloc((stacksize + STACK_OFFSET) * sizeof(long));

   /* initializes the new stack */
   this_Stack = (Stack *)memory + STACK_OFFSET;
   this_Stack[STACK_POINTER_INDEX] = 0; /* next empty index in stack */
   this_Stack[STACK_SIZE_INDEX] = stacksize; /* sets size of stack */
   this_Stack[STACK_COUNT_INDEX] = stack_counter; /* sets stack number */

   /* debug message */
   if(debug)
   {
     fprintf(stderr, ALLOCATED, (long)stack_counter);
   }

   return this_Stack;
}


/*--------------------------------------------------------------------------
Function Name:         num_elements
Purpose:               Returns the number of elements in the stack.
Description:           Checks if the stack exists. If it does, it gets the
                       index of the next available spot, which is also the
		       number of elements (because indexes are one less than
		       the number of items).
Input:                 this_Stack - a pointer to the stack to be checked
Result:                A warning message is displayed if the stack does not
                       exist. Otherwise, the number of items in the stack
		       is returned.
                       long - number of items in the stack
--------------------------------------------------------------------------*/
long num_elements (Stack * this_Stack) {
    /* warning if stack does not exist */
    if(!this_Stack)
    {
       fprintf(stderr, NUM_NONEXIST);
       return 0;
    }

    else
    {
       /*gets index of next available spot which is also the num of items */
       long num_elements = this_Stack[STACK_POINTER_INDEX]; 

       return num_elements;
    }
}


/*--------------------------------------------------------------------------
Function Name:         pop
Purpose:               Removes the item from the top of the stack and sends
                       it back to the user via the parameter item.
Description:           Checks if the stack exists and if the stack is empty or 
                       not. If there is nothing hindering the pop, it gets the
                       index of the top item in the stack and grabs its value.
		       The index of the next available space is decremented
		       to remove the item from the stack.
Input:                 this_Stack - a pointer to the stack from which to pop
                       item - a pointer to a long to hold the value of the
		              top-most item
Result:                A warning message is displayed if the stack does not
                       exist or if it is empty. Otherwise, the value of the
		       top-most item is grabbed.
                       long - true or false; returns FALSE (0) if the pop
		       failed, and TRUE (1) if the pop succeeded
--------------------------------------------------------------------------*/
long pop (Stack * this_Stack, long * item) {
   /* warning if stack doesn't exist */
   if(!this_Stack)
   {
      fprintf(stderr, POP_NONEXIST);
      return 0;
   }

   /* warning if stack is empty */
   else if(isempty_Stack(this_Stack))
   {
      fprintf(stderr, POP_EMPTY);
      return 0;
   }

   else
   {
      /* gets the index of the top item in the stack */
      long numToPopIndex = this_Stack[STACK_POINTER_INDEX] - 1;
     
      /* grabs the value of the top item in the stack */
      *item = this_Stack[numToPopIndex];

      /* decrements index of next available space to remove item */
      this_Stack[STACK_POINTER_INDEX]--;

      /* debug messages */
      if(debug)
      {
	 if(*item < 0) /* uses hex debug message if item is negative */
	 {
	    fprintf(stderr, HEXPOP, (long)stack_counter, *item);
	 }
	 else
	 {	 
            fprintf(stderr, POP, (long)stack_counter, *item);
	 }
      }

      return 1;
   }
}


/*--------------------------------------------------------------------------
Function Name:         push
Purpose:               Adds an item to the top of the stack.
Description:           Checks if the stack exists and if the stack is full or 
                       not. If there is nothing hindering the push, it gets 
		       the index of the next available space in the stack and
		       sets that space to the value of the item being pushed.
		       The index of the next available space is incremented.
Input:                 this_Stack - a pointer to the stack on which to push
Result:                A warning message is displayed if the stack does not
                       exist or if it is full. Otherwise, an item is added
		       to the stack.
                       long - true or false; returns FALSE (0) if the push
		       failed, and TRUE (1) if the push succeeded
--------------------------------------------------------------------------*/
long push (Stack * this_Stack, long item) {
   /* warning if stack is does not exist */
   if(!this_Stack)
   {
      fprintf(stderr, PUSH_NONEXIST);
      return 0;
   }
   /* warning if stack is full */
   else if(isfull_Stack(this_Stack))
   {
     fprintf(stderr, PUSH_FULL);
     return 0;   
   }
   /* adds item to the stack */
   else
   {
      /* gets index of next empty spot */	   
      long sp = this_Stack[STACK_POINTER_INDEX]; 

      this_Stack[sp] = item; /* adds item */

      /* increments index of next empty spot */
      this_Stack[STACK_POINTER_INDEX]++;

      /* debug messages */
      if(debug)
      {
	 if(item < 0) /* uses hex debug message if item is negative */
	 {
	    fprintf(stderr, HEXPUSH, (long)stack_counter, item);
	 }
	 else
	 {	 
            fprintf(stderr, PUSH, (long)stack_counter, item);
	 }
      }

      return 1;
   }
}


/*--------------------------------------------------------------------------
Function Name:         top
Purpose:               Gets the value of the item at the top of the stack and 
                       sends it back to the user via the parameter item. Does
		       not remove the item from the stack.
Description:           Checks if the stack exists and if the stack is empty or 
                       not. If there is nothing hindering the top, it gets the
                       index of the top item in the stack and grabs its value.
Input:                 this_Stack - a pointer to the stack from which to pop
                       item - a pointer to a long to hold the value of the
		              top-most item
Result:                A warning message is displayed if the stack does not
                       exist or if it is empty. Otherwise, the value of the
		       top-most item is grabbed.
                       long - true or false; returns FALSE (0) if the top
		       failed, and TRUE (1) if the top succeeded
--------------------------------------------------------------------------*/
long top (Stack * this_Stack, long * item) {
   /* warning if stack doesn't exist */
   if(!this_Stack)
   {
      fprintf(stderr, TOP_NONEXIST);
      return 0;
   }

   /* warning if stack is empty */
   else if(isempty_Stack(this_Stack))
   {
      fprintf(stderr, TOP_EMPTY);
      return 0;
   }

   else
   {
      /* gets the index of the top item in the stack */
      long numToTopIndex = this_Stack[STACK_POINTER_INDEX] - 1;
     
      /* grabs the value of the top item in the stack */
      *item = this_Stack[numToTopIndex];

      /* debug messages */
      if(debug)
      {
	 if(*item < 0) /* uses hex debug message if item is negative */
	 {
	    fprintf(stderr, HEXTOP, (long)stack_counter, *item);
	 }
	 else
	 {	 
            fprintf(stderr, TOP, (long)stack_counter, *item);
	 }
      }

      return 1;
   }
}


FILE * write_Stack (Stack * this_Stack, FILE * stream) {

   long index = 0;         /* index into the stack */

   if (this_Stack == NULL) {
      fprintf (stderr, WRITE_NONEXIST_STACK);
      return stream;
   }

   if (stream == NULL) {
      fprintf (stderr, WRITE_NONEXIST_FILE);
      return stream;
   }
                
   if (stream == stderr)
      fprintf (stream, "Stack has %ld items in it.\n",
               num_elements (this_Stack));

   for (index = STACK_COUNT_INDEX + STACK_OFFSET;
        index < num_elements (this_Stack); index++) {

      if (stream == stderr)
         fprintf (stream, "Value on stack is |0x%lx|\n", this_Stack[index]);
 
       else {
          if (this_Stack[index] < 0)
             fprintf (stream, "%c ", (char) this_Stack[index]);
    
          else
             fprintf (stream, "%ld ", this_Stack[index]);
       }
   }

   return stream;
}
