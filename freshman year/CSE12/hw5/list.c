/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 2, 2015
                                                        cs12xre
                                Assignment Five

File Name:      list.c
Description:    This program contains the functions that define lists and
                nodes. These functions can be called to create a new list
		or node, insert nodes into the list, manipulate the list,
		and remove nodes and lists.
****************************************************************************/

/* ------------------------------------------------------------------------
Debugger Questions: 

1. front: (Node *) 0x804b038
2. Node 1 data: (void *) 0x804b028
3. Node 1 pre: (struct Node *) 0x804b078
4. Node 1 next: (struct Node *) 0x804b058
5. Node 2 data: (void *) 0x804b048
6. Node 2 pre: (struct Node *) 0x804b038
7. Node 2 next: (struct Node *) 0x804b078
8. Node 3 data: (void *) 0x804b068
9. Node 3 pre: (struct Node *) 0x804b058
10. Node 3 next: (struct Node *) 0x804b038
--------------------------------------------------------------------------*/

#include <malloc.h>
#include <stdio.h>
#include "mylib.h"
#include "list.h"

typedef struct Node {
        struct Node * pre;      /* how to access the prior Node */
        struct Node * next;     /* how to access the next Node */
        void * data;            /* the data to store */
} Node;

typedef struct List {
        Node * front;             /* the front of the list */
        long list_count;        /* which list is it */
        long occupancy;
        void *(*copy_func) (void *);
        void (*delete_func) (void *);
        long (*is_greater_than_func) (void *, void *);
        FILE *(*write_func) (void *, FILE *);
} List;

/* private Node function declarations */
static void delete_Node (Node **, void (*delete_func) (void *));
static int find_location (List * this_list, void * element);
static Node * insert_Node (Node *, void *, void *(*copy_func) (void *)); 
static Node * new_Node (void *, void *(*copy_func) (void *));
static void * remove_Node (Node *);
static void * view_Node (Node *); 
static FILE * write_Node (Node *, FILE *, FILE *(*write_func) 
		                                          (void *, FILE *));

/* catastrophic error messages */
static const char ADNEXT_NONEXIST[] = 
                "Advance next from non-existent list!!!\n";
static const char ADNEXT_EMPTY[] = 
                "Advance next from empty list!!!\n";
static const char ADPRE_NONEXIST[] = 
                "Advance pre from non-existent list!!!\n";
static const char ADPRE_EMPTY[] = 
                "Advance pre from empty list!!!\n";
static const char DELETE_NONEXIST[] =
                "Deleting from non-existent list!!!\n";
static const char DELETE_NONEXISTNODE[] =
                "Deleting a non-existent node!!!\n";
static const char ISEMPTY_NONEXIST[] =
                "Is empty check from non-existent list!!!\n";
static const char INSERT_NONEXIST[] =
                "Inserting to a non-existent list!!!\n";
static const char REMOVE_NONEXIST[] =
                "Removing from non-existent list!!!\n";
static const char REMOVE_EMPTY[] =
                "Remove from empty list!!!\n";
static const char VIEW_NONEXIST[] = 
                "Viewing a non-existent list!!!\n";
static const char VIEW_NONEXISTNODE[] = 
                "Viewing a non-existent node!!!\n";
static const char VIEW_EMPTY[] =
                "Viewing an empty list!!!\n";
static const char WRITE_NONEXISTFILE[] =
                "Writing to a non-existent file!!!\n";
static const char WRITE_NONEXISTLIST[] =
                "Writing from a non-existent list!!!\n";
static const char WRITE_MISSINGFUNC[] =
                "Don't know how to write out elements!!!\n";
static const char WRITE_NONEXISTNODE[] =
                "Writing from a non-existent node!!!\n";

/* debug messages */
static const char ADNEXT[] = "[List %ld - Advancing next]\n";
static const char ADPRE[] = "[List %ld - Advancing pre]\n";
static const char INSERT[] = "[List %ld - Inserting node]\n";
static const char REMOVE[] = "[List %ld - Removing node]\n";
static const char VIEW[] = "[List %ld - Viewing node]\n";
static const char LIST_ALLOCATE[] = "[List %ld has been allocated]\n";
static const char LIST_DEALLOCATE[] = "[List %ld has been deallocated]\n";

static int debug_on = FALSE;    /* allocation of debug flag */
static long list_counter = 0;   /* the number of lists allocated so far */


/*--------------------------------------------------------------------------
Function Name:         set_debug_on
Purpose:               Sets it so that debug messages are printed.
Description:           Sets the debug_on boolean value to true.
Input:                 void - no input
Result:                Debug messages will be printed.
                       void - no return value
--------------------------------------------------------------------------*/
void set_debug_on (void) 
{
    debug_on = TRUE;
}


/*--------------------------------------------------------------------------
Function Name:         set_debug_off
Purpose:               Sets it so that debug messages are not printed.
Description:           Sets the debug_on boolean value to false.
Input:                 void - no input
Result:                Debug messages will not be printed.
                       void - no return value
--------------------------------------------------------------------------*/
void set_debug_off (void) 
{
    debug_on = FALSE;
}


/*--------------------------------------------------------------------------
Function Name:         advance_next_List
Purpose:               Causes the front pointer of the list to move forward
                       by one node, shifting the elements of the list forward
		       by one.
Description:           Sets the front pointer of the list to point the element
                       after the current front element.
Input:                 this_list - a pointer to the list to be shifted
Result:                The front pointer of the list moves forward one node.
                       void - no return value
--------------------------------------------------------------------------*/
void advance_next_List (List * this_list) {
   /* error if this_list is null/doesn't exist */
   if(!this_list) 
   {
      fprintf(stderr, ADNEXT_NONEXIST);
   }

   /* error if this_list is empty */
   else if(this_list -> occupancy == 0)
   {
      fprintf(stderr, ADNEXT_EMPTY);
   }

   else
   {
      /* debug */
      if(debug_on)
      {
         fprintf(stderr, ADNEXT, this_list -> list_count);
      }

      /* sets the front of the list to the node after the current front */
      this_list -> front = this_list -> front -> next;
   }
}


/*--------------------------------------------------------------------------
Function Name:         advance_pre_List
Purpose:               Causes the front pointer of the list to move backward
                       by one node, shifting the elements of the list backward
		       by one.
Description:           Sets the front pointer of the list to point the element
                       before the current front element.
Input:                 this_list - a pointer to the list to be shifted
Result:                The front pointer of the list moves backward one node.
                       void - no return value
--------------------------------------------------------------------------*/
void advance_pre_List (List * this_list) {
   /* error if this_list is null/doesn't exist */
   if(!this_list) 
   {
      fprintf(stderr, ADPRE_NONEXIST);
   }

   /* error if this_list is empty */
   else if(this_list -> occupancy == 0)
   {
      fprintf(stderr, ADPRE_EMPTY);
   }

   else
   {
      /* debug */
      if(debug_on)
      {
         fprintf(stderr, ADPRE, this_list -> list_count);
      }     

      /* sets the front of the list to the node before the current front */
      this_list -> front = this_list -> front -> pre;
   }
}


/*--------------------------------------------------------------------------
Function Name:         delete_List
Purpose:               Deallocates all memory associated with the list, 
                       including the memory of the nodes. Sets the list 
		       pointer to null.
Description:           Loops to deallocate the memory of all the nodes in the
                       list as well as their data, then calls free to 
		       deallocate the memory of the list. Sets the pointer
		       to the list to null.
Input:                 lpp - a pointer to a pointer to the list to be deleted
Result:                The list and all its elements are deleted.
                       void - no return value
--------------------------------------------------------------------------*/
void delete_List (List ** lpp) 
{
   Node * next_Node; /* pointer to the node after the one to be deleted */

   /* error if the list doesn't exist */
   if(!lpp|| !*lpp)
   {
      fprintf(stderr, DELETE_NONEXIST);
   }

   else
   {
      /* loops to deallocate all nodes */
      while(!isempty_List(*lpp))
      {
	 /* only runs if there is more than one node left */
	 if((*lpp) -> occupancy > 1)
	 {
            /* grabs the node after the one to be deleted */
            next_Node = (*lpp) -> front -> next; 
	 }

	 /* deletes the front node and its data */
         delete_Node(&(*lpp) -> front, (*lpp) -> delete_func);

	 /* sets the front pointer to the next node */
	 (*lpp) -> front = next_Node;
	 (*lpp) -> occupancy--; /* decrements number of elements in list */
      }
	 
      /* debug */
      if(debug_on)
      {
         fprintf(stderr, LIST_DEALLOCATE, (*lpp) -> list_count);
      }

      free(*lpp); /* frees the memory */
      *lpp = NULL; /* sets the list pointer to null */

      list_counter--; /* decrements the number of lists */
   }
}


/*--------------------------------------------------------------------------
Function Name:         find_location
Purpose:               Locates where a node should be in a sorted list.
Description:           Loops to keep updating the front pointer of the
                       list until the node which the new node should be
		       inserted before is set as the front pointer.
Input:                 this_list - the list to check
                       element - the data within the node to be inserted
Result:                The node that the new node should be inserted 
                       directly before is set as the front pointer.
                       int - returns 1 if find_location worked, 
		             0 if it failed
--------------------------------------------------------------------------*/
static int find_location (List * this_list, void * element) 
{
   int numChecked = 0; /* holds number of nodes checked */
   
   /* does nothing if the list is empty */
   if(isempty_List(this_list))
   {
      return 0;
   }
  
   else
   {  
      /* keeps checking the elements in the list and updating the front 
       * pointer to the next node until a node with a larger value than the 
       * node to insert is found or there are no more elements to check */
      do 
      {
         if(this_list -> is_greater_than_func(element, 
			                   this_list -> front -> data)) 
         {
            advance_next_List(this_list); /* moves the front pointer */
	    numChecked++; /* updates the number of nodes checked */
         }
         else
         {
            break; /* breaks out of the loop if found */
         }	 

      }while(numChecked < this_list -> occupancy);
	
      return 1;
   }   
}


/*--------------------------------------------------------------------------
Function Name:         insert
Purpose:               Inserts a node with an element into the list at the
                       front, end, or in sorted form.
Description:           Saves where the original front pointer was pointing.
                       Uses insert_Node to create a new node and insert it
                       into the list. If where to insert is sorted, calls
		       find_location to update the front pointer. Inserts
		       the new node before the front node and updates the 
		       front pointer based on if the node was inserted
		       in the front, back, or sorted.
Input:                 this_list - the list to insert into
                       element - what to be inserted
		       where - where to insert; 1 = front, 0 = end, 2 = sorted
Result:                A node is inserted into the list holding the element.
                       long - returns 1 if insert worked, 
		              0 if it failed
--------------------------------------------------------------------------*/
long insert (List * this_list, void * element, long where) 
{
   Node * new_Node; /* the new node that is inserted */
   Node * front_Node; /* the original front of the list */
     
   /* error if this_list is NULL */
   if(!this_list) 
   {
      fprintf(stderr, INSERT_NONEXIST);	  
      return 0; 
   }

   /* debug */
   if(debug_on)
   {
      fprintf(stderr, INSERT, this_list -> list_count);
   }

   front_Node = this_list -> front; /* saves prior value of front */

   if(where == SORTED)
   {
      find_location(this_list, element);
   }

   /* creates a new node, inserts it, and updates pre and next pointers */
   new_Node = insert_Node(this_list -> front, element, 
		          this_list -> copy_func);

   /* if the insertion is at the front */
   if(where == FRONT) 
   {
      /* sets the front pointer to the new node */	   
      this_list -> front = new_Node; 
   }

   /* if the insertion is at the end */
   else if(where == END) 
   {
      if(front_Node != NULL)
      {	      
         /* keeps the original front pointer */
         this_list -> front = front_Node;
      }
      else
      {
         /* sets the new node as the front if it's the first node being
	  * inserted */
         this_list -> front = new_Node;
      }
   }

   /* if it's a sorted insert */
   else if(where == SORTED)
   {
      /* sets the front pointer to the new node if it's the first node being
       * inserted or it's smaller than the original front node */	   
      if(front_Node == NULL || 
              this_list -> is_greater_than_func(front_Node -> data, 
			                   new_Node -> data))
      {
         this_list -> front = new_Node;
      }     

      /* sets the front pointer to the original front node if the node
       * being inserted is at the middle or end of the sorted list */
      else if(front_Node != NULL)
      {
         this_list -> front = front_Node;
      }
   }

   this_list -> occupancy++; /* increments number of nodes in list */

   return 1;
}
 

/*--------------------------------------------------------------------------
Function Name:         isempty_List
Purpose:               Checks if the list is empty or not.
Description:           Checks the occupancy of the list. If it's 0, the list
                       is empty. If it's not 0, the list is not empty.
Input:                 this_list - the list to check
Result:                Returns if the list is empty or not.
                       long - returns 0 if the list is not empty or the check
		              failed, 1 if the list is empty
--------------------------------------------------------------------------*/
long isempty_List (List * this_list) 
{
   /* error if the list doesn't exist */
   if(!this_list)
   {
      fprintf(stderr, ISEMPTY_NONEXIST);
      return 0;
   }
 
   /* true if the list occupancy is 0 */
   else if(this_list -> occupancy == 0)
   {
      return 1;
   }

   /* false if the list occupancy isn't 0 */
   else
   {
      return 0;
   }
}


/*--------------------------------------------------------------------------
Function Name:         new_List
Purpose:               Allocates and initializes a new list.
Description:           Allocates memory for a new List using malloc. 
                       Initializes all the fields within the list.
Input:                 copy_func - pointer to the copy function of the object
                                   the list will store
		       delete_func - pointer to the delete function of the 
		                     object the list will store
		       is_greater_than_func - pointer to the is greater than
		                              function of the object the list
					      will store
		       write_func - pointer to the write function of the 
		                    object that the list will store
Result:                A new list is created.
                       List * - a pointer to the new list.
--------------------------------------------------------------------------*/
List * new_List (
        void *(*copy_func) (void *),
        void (*delete_func) (void *),
        long (*is_greater_than_func) (void *, void *),
        FILE *(*write_func) (void *, FILE *)) 
{ 
   List * this_List; /* list pointer to new list */

   list_counter++; /* increments number of lists allocated */

   /*debug messages */
   if(debug_on) {
      fprintf(stderr, LIST_ALLOCATE, list_counter);
   }

   /* allocates memory */
   this_List = (List * ) malloc(sizeof(List));

   /*initializes list */
   this_List -> front = NULL; /* no front of list yet */
   this_List -> list_count = list_counter; /* which list it is */
   this_List -> occupancy = 0; /* empty list */
   /* methods */
   this_List -> copy_func = copy_func;
   this_List -> delete_func = delete_func;
   this_List -> is_greater_than_func = is_greater_than_func;
   this_List -> write_func = write_func;

   return this_List;
}


/*--------------------------------------------------------------------------
Function Name:         remove_List
Purpose:               Removes either the front or end node from the list.
Description:           If the node to remove is the end node, calls 
                       advance_pre_node to set the front pointer to point to 
		       the end node. If the node to remove is the front node, 
		       nothing is called. Then, remove_Node is called on the
		       node the front pointer is pointing to. The front
		       pointer is set to the node after the one that was
		       removed and the list's occupancy is decremented.
Input:                 this_list - the list to remove from
                       where - which node to remove; 1 for front, 0 for end
Result:                A node is removed from the list.
                       void * - returns a pointer to the data within the 
		                node that was removed.
--------------------------------------------------------------------------*/
void * remove_List (List * this_list, long where) 
{
   void * data; /* holds the data from the node removed */
   void * next_Node; /* the node after the one removed */

   /* error if this_list is null */
   if(!this_list)
   {
      fprintf(stderr, REMOVE_NONEXIST);
      return NULL;
   }

   /* error if this_list is empty */
   if(isempty_List(this_list))
   {
      fprintf(stderr, REMOVE_EMPTY);
      return NULL;
   }

   /* if the end node is to be removed */
   if(where == END)
   {
      /* moves the front pointer to point to the end node */
      advance_pre_List(this_list);
   }

   if(debug_on)
   {
      fprintf(stderr, REMOVE, this_list -> list_count);
   }

   /* checks if there's more than one node left */
   if(this_list -> front -> next != this_list -> front)
   {
      /* holds the pointer to the node after the one removed */
      next_Node = this_list -> front -> next;
   }
   else
   {
      /* sets the next_Node to null if the node to remove is the only node
       * in the list */
      next_Node = NULL;
   }

   /* calls remove_Node to unlink the node */
   data = remove_Node(this_list -> front);

   /* sets the front pointer to the next node */
   this_list -> front = next_Node;

   /* decrements number of elements */
   this_list -> occupancy--;

   return data;
}


/*--------------------------------------------------------------------------
Function Name:         view
Purpose:               Views either the front or end node from the list.
Description:           If the node to view is the end node, calls 
                       advance_pre_node to set the front pointer to point to 
		       the end node. If the node to view is the front node, 
		       nothing is called. Then, view_Node is called on the
		       node the front pointer is pointing to. If the node
		       that was viewed was the end node, the front pointer
		       is returned to point to the original front.
Input:                 this_list - the list to view from
                       where - which node to view; 1 for front, 0 for end
Result:                A node is viewed.
                       void * - returns a pointer to the data within the 
		                node that was viewed.
--------------------------------------------------------------------------*/
void * view (List * this_list, long where) 
{
   void * node_Data; /* holds the data of the node to be viewed */

   /* error if this_list is null */
   if(!this_list)
   {
      fprintf(stderr, VIEW_NONEXIST);
      return NULL;
   }

   /* error if this list is empty */
   if(isempty_List(this_list))
   {
      fprintf(stderr, VIEW_EMPTY);
      return NULL;
   }

   /* if the node to view is the end node */
   if(where == END)
   {
      /* moves the front pointer to point to the end node */  
      advance_pre_List(this_list);
   }
 
   /* debug */
   if(debug_on)
   {
      fprintf(stderr, VIEW, this_list -> list_count);
   }

   /* calls view_Node to return a pointer to the data of the node to view */
   node_Data = view_Node(this_list -> front);

   /* if the node to view was the end node */
   if(where == END)
   {
      /* moves the front pointer to point at the original front node */
      advance_next_List(this_list);
   }

   return node_Data;
}


FILE * write_List (List * this_list, FILE * stream) {
        long count;             /* to know how many elements to print */
        Node * working;                 /* working node */

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_list) {
                fprintf (stderr, WRITE_NONEXISTLIST);
                return NULL;
        }

        if (stream == stderr)
                fprintf (stream, "List %ld has %ld items in it.\n",
                        this_list->list_count, this_list->occupancy);

        if (!this_list->write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        if (this_list->occupancy >= 1)
                working = this_list->front;

        for (count = 1; count <= this_list->occupancy; count++) {
                if (stream == stderr)
                        fprintf (stream, "\nelement %ld:  ", count);
                write_Node (working, stream, this_list->write_func);
                working = working->next;
        }
                
        return stream;
}


/*--------------------------------------------------------------------------
Function Name:         write_reverse_List
Purpose:               Writes the list from front to back.
Description:           Sets the first node to be written as the end node of
                       the list. Calls write_Node to write the node. Then,
		       sets the node before the node that was just written 
		       to be the node to be written next.
Input:                 this_list - the list to view from
                       stream - where to print (stdout, stderr, etc)
Result:                The list is printed in reverse.
                       FILE * - a pointer to the place the list should be
		                printed (stdout, stderr)
--------------------------------------------------------------------------*/
FILE * write_reverse_List (List * this_list, FILE * stream) 
{
   long count; /* how many elements to print */
   Node * working; /* working node */

   /* error if stream is null */
   if (!stream) 
   {
      fprintf (stderr, WRITE_NONEXISTFILE);
      return NULL;
   }

   /* error if this_list is null */
   if (!this_list) 
   {
      fprintf (stderr, WRITE_NONEXISTLIST);
      return NULL;
   }

   /* prints list number and number of items in it */
   if (stream == stderr)
   {
      fprintf (stream, "List %ld has %ld items in it.\n",
               this_list->list_count, this_list->occupancy);
   }

   /* error if no write function */
   if (!this_list->write_func) 
   {
      fprintf (stream, WRITE_MISSINGFUNC);
      return stream;
   }

   /* sets the working node to the last node of the list */
   if (this_list->occupancy >= 1)
   {
      working = this_list-> front-> pre; 
   }
      
   /* prints nodes in reverse order */
   for (count = 1; count <= this_list -> occupancy; count++) 
   {
      /* prints element number */
      if (stream == stderr)
      {
         fprintf (stream, "\nelement %ld:  ", count);
      }
                
      write_Node (working, stream, this_list->write_func); /* writes node */
      working = working->pre; /* goes back a node */
   }
                
   return stream;
}


static void delete_Node (Node ** npp, void (*delete_func) (void *)) {

        /* does the node exist??? */
        if (!npp || !*npp) {
                fprintf (stderr, DELETE_NONEXISTNODE);
                return;
        }

        /* call function to delete element */
        if (delete_func && (*npp)->data)
                (*delete_func) (&((*npp)->data));

        /* delete element */
        free (*npp);

        /* assign node to NULL */
        *npp = NULL;
}


/*--------------------------------------------------------------------------
Function Name:         insert_Node
Purpose:               Inserts a node into a list.
Description:           Inserts a node into a list by attaching it to the
                       nodes that should be before and after it. When calling
		       this method, a pointer to the node that the new node
		       should be inserted before is passed in. If that pointer
		       is null, the new node is the first node and its pre and
		       next pointers are set to itself. Otherwise, the new
		       node as well as the nodes before and after the new 
		       node update their pre and next pointers to include the
		       new node in the list.
Input:                 this_Node - the node the new node to insert should be
                                   before
                       element - a pointer to the data that should go in the
		                 new node
		       copy_func - a pointer to the copy function of the data
Result:                A node is inserted into a list.
                       Node * - returns a pointer to the new node
--------------------------------------------------------------------------*/
static Node * insert_Node (Node * this_Node, void * element, 
        void * (*copy_func) (void *)) 
{

   Node * inserted_Node; /* pointer to a new node to be created */

   /* calls new_Node to create a new node */
   inserted_Node = new_Node(element, copy_func);

   /* checks if the node is the first node being inserted */
   if(this_Node == NULL) 
   {
      /*sets pre and next pointers to itself */
      inserted_Node -> next = inserted_Node;	   
      inserted_Node -> pre = inserted_Node;	   	   
   }

   else 
   {
      inserted_Node -> next = this_Node; /* sets next to point to next node */
      /* sets pre to point to previous node, attaches node */
      inserted_Node -> pre = this_Node -> pre; 

      /* updates pointers of the previous and next nodes of the new node */
      this_Node -> pre = inserted_Node;
      inserted_Node -> pre -> next = inserted_Node;
   } 

   return inserted_Node;  
}


static Node* new_Node (void * element, void * (*copy_func) (void *)) {

   /* allocate memory */
   Node *this_Node = (Node *) malloc (sizeof (Node));

   /* initialize memory */
   this_Node->next = this_Node->pre = NULL;
   this_Node->data = (copy_func) ? (*copy_func) (element) : element;

   return this_Node;
}


/*--------------------------------------------------------------------------
Function Name:         remove_Node
Purpose:               Removes the node from a list and deallocates
                       its memory.
Description:           Unlinks the node from a list by setting the pointers
                       of the nodes before and after the node to be deleted
		       to point to each other and not the node to be deleted.
		       Saves a pointer to the data in the node and calls
		       delete_Node to deallocate the memory having to do with
		       the node.
Input:                 this_Node - the node to remove
Result:                A node is removed.
                       void * - returns a pointer to the data within the 
		                node that was removed.
--------------------------------------------------------------------------*/
static void * remove_Node (Node * this_Node) 
{
   void * data; /* void pointer to hold the data inside the node */
	
   /* only does these if the node exists */
   if(this_Node)
   {
      /* unlinks this node from the list */
      this_Node -> next -> pre = this_Node -> pre;
      this_Node -> pre -> next = this_Node -> next;

      /* gets a pointer to the data in the node */
      data = this_Node -> data;
   }

   /* frees memory of the node (but not the data in the node) */
   delete_Node(&this_Node, 0);

   return data;
}


/*--------------------------------------------------------------------------
Function Name:         view_Node
Purpose:               Gets the data from within a node.
Description:           Returns a pointer to the data within a node.
Input:                 this_Node - the node to get the data from
Result:                void * - returns a pointer to the data within the 
		                node that was passed in
--------------------------------------------------------------------------*/
static void * view_Node (Node * this_Node) 
{
   /* error message if this_Node is null */
   if(!this_Node)
   {
      fprintf(stderr, VIEW_NONEXISTNODE);
      return NULL;
   }

   /* returns a pointer to the data in this_Node */
   else
   {   
      return this_Node -> data;
   }
}


static FILE* write_Node (Node * this_Node, FILE * stream,
        FILE * (*write_func) (void *, FILE *)) {

        if (!stream) {
                fprintf (stderr, WRITE_NONEXISTFILE);
                return NULL;
        }

        if (!this_Node) {
                fprintf (stream, WRITE_NONEXISTNODE);
                return stream;
        }

        if (!write_func) {
                fprintf (stream, WRITE_MISSINGFUNC);
                return stream;
        }

        return (*write_func) (this_Node->data, stream);
}
