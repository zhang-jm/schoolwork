/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 3, 2015
                                                        cs12xre
                                Assignment Five

File Name:      stack.c
Description:    This program contains the functions that define stacks. To 
                implement these functions, all work is delegated to the
		functions of the list.
****************************************************************************/

#include <stdio.h>
#include "list.h"
#include "stack.h"

/*--------------------------------------------------------------------------
Function Name:         delete_Stack
Purpose:               Deletes the stack.
Description:           Calls delete_List (List's delete function) to 
                       deallocate memory, etc.
Input:                 spp - a pointer to a pointer to the stack to be
                             deleted
Result:                A stack is deleted.
                       void - no return value
--------------------------------------------------------------------------*/
void delete_Stack (Stack ** spp) {
        delete_List (spp);
}


/*--------------------------------------------------------------------------
Function Name:         isempty_Stack
Purpose:               Checks if the stack is empty.
Description:           Calls isempty_List (the List's isempty check) to
                       check if the list is empty or not.
Input:                 this_stack - the stack to check
Result:                long - 1 if the stack is empty, 0 if the stack is
                              not empty
--------------------------------------------------------------------------*/
long isempty_Stack (Stack * this_stack) {
        return isempty_List (this_stack);
}


/*--------------------------------------------------------------------------
Function Name:         new_Stack
Purpose:               Makes a new stack.
Description:           Calls new_List to allocate memory, initialize,
                       etc.
Input:                 copy_func - pointer to the copy function of the object
                                   the stack will store
		       delete_func - pointer to the delete function of the 
		                     object the stack will store
		       is_greater_than_func - pointer to the is greater than
		                              function of the object the stack
					      will store
		       write_func - pointer to the write function of the 
		                    object that the stack will store
Result:                A new stack is created.
                       Stack * - a pointer to the new Stack
--------------------------------------------------------------------------*/
Stack * new_Stack (void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) {

        return new_List (copy_func, delete_func, is_greater_than_func, 
			 write_func);
}


/*--------------------------------------------------------------------------
Function Name:         pop
Purpose:               Removes an item from the stack and gets it.
Description:           Calls remove_List (List's remove) to remove an item
                       from the stack.
Input:                 this_stack - the stack to remove the item from
Result:                void * - pointer to the object that was removed
--------------------------------------------------------------------------*/
void * pop (Stack * this_stack) {
        return remove_List (this_stack, END);
}


/*--------------------------------------------------------------------------
Function Name:         push
Purpose:               Adds an item to the stack.
Description:           Calls insert (List's insert) to add an item to the
                       stack.
Input:                 this_Stack - the stack to add an item to
                       element - the item to add to the stack
Result:                long - 1 if the push succeeded, 
                              0 if the push failed
--------------------------------------------------------------------------*/
long push (Stack * this_stack, void * element) {
        return insert (this_stack, element, END);
}


/*--------------------------------------------------------------------------
Function Name:         top
Purpose:               Grabs the item at the top of the stack but does not 
                       remove it.
Description:           Calls view (List's view function) to get a pointer to
                       the item at the top of the list.
Input:                 this_stack - the stack to get the item from
Result:                void * - pointer to the object at the top of the
                                stack
--------------------------------------------------------------------------*/
void * top (Stack * this_stack) {
        return view (this_stack, END);
}

/*--------------------------------------------------------------------------
Function Name:         write_Stack
Purpose:               Writes out the items on the stack.
Description:           Calls write_List (List's write function) to write out
                       the items on the stack.
Input:                 this_stack - the stack to write out
                       stream - where to write to (stdout, stderr)
Result:                The stack is written out.
                       FILE * - where the stack was written to (stdout, 
		                stderr)
--------------------------------------------------------------------------*/
FILE * write_Stack (Stack * this_stack, FILE * stream) {
        return write_List (this_stack, stream);
}
