/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 15, 2015
                                                        cs12xre
                                Assignment Seven

File Name:      Tree.c
Description:    This file contains the constructor, destructor and functions
                that define a Tree and the Nodes that go into the Tree. 
		The constructor is called to allocate and initialize a new 
		Tree or a new Node. The destructor deallocates memory.
		The Tree methods can be called to insert a new item 
	        into the Tree, look up an existing item in the Tree, 
		remove an item from the Tree, or write out the Tree. The 
		Node methods can be used to write out the Node.
****************************************************************************/


#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string.h>
#include "Tree.h"
using namespace std;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

static const char ALLOCATE[] = " - Allocating]\n";
static const char DEALLOCATE[] = " has been deallocated]\n";
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char INSERT[] = " - Inserting ";
static const char TREE[] = "[Tree ";


/*********************************************************************** 
* Struct Name: TNode
* Description: Implements the node of the tree data structure. Each node
*              contains two children, the left child which contains data
*              "less than" the data of the current node, and the right 
*              child which contains data "greater than" the data of the 
*              current node.
* Data Fields: Base * data - the data the Node contains
*              TNode * left - pointer to the left child of the node
*              TNode * right - pointer to the right child of the node
*              TNode * parent - pointer to the parent of the node
*              long occupancy - how many nodes are in the tree
*              long hasBeenDeleted - flag for if the node was removed
*              long balance - the balance of the node
*              long height - the height of the node
* Methods: Write - creates a string representation of the node
*          Write_AllTNodes - writes all the nodes in the tree
*          delete_AllTNodes - deallocates all nodes in the tree
************************************************************************/
struct TNode {
   Base * data;
   TNode * left, * right, * parent;
   static long occupancy;
   unsigned long hasBeenDeleted;

   // left child's height - right child's height
   long balance;

   // 1 + height of tallest child, or 0 for leaf
   long height;


   /*-----------------------------------------------------------------------
   Name:                  UCSDStudent
   Purpose:               Constructor for the TNode.
   Description:           Allocates and initializes the TNode's data 
                          fields.
   Input:                 element - the data to go into the node
   -----------------------------------------------------------------------*/
   TNode (Base * element) : data (element), left (0), right (0),
          parent (0), hasBeenDeleted (FALSE), balance (0), height(0) 
   {
      occupancy++;
   }


   /*-----------------------------------------------------------------------
   Name:                  ~UCSDStudent
   Purpose:               Destructor for the TNode.
   Description:           Frees up the memory associated with the node's
                          data and sets the data pointer to 0.
   Input:                 none
   -----------------------------------------------------------------------*/    
   ~TNode (void) 
   {
      delete data;
      data = 0;
   }

   ostream & Write (ostream & stream) const 
   {
      stream << "at height:  " << height << "  with balance:  " << balance 
	     << "  ";
      return data->Write (stream) << "\n";
   }

   ostream & Write_AllTNodes (ostream & stream) const 
   {
      if (left)
         left->Write_AllTNodes (stream);
      if (!hasBeenDeleted)
         Write (stream);
      if (right)
         right->Write_AllTNodes (stream);

      return stream;
   }

  
   /*-----------------------------------------------------------------------
   Function Name:         delete_AllTNodes
   Purpose:               Deletes all the nodes in the tree.
   Description:           Uses a loop and post-order traversal to travel
                          through a tree and deallocate all the nodes in it.
   Input:                 void - no input
   Result:                All nodes in the tree are deleted.
   -----------------------------------------------------------------------*/   
   void delete_AllTNodes (void) 
   {
      TNode * currNode = this; //the current node we're at
      TNode * preNode;

      //only initializes preNode if the tree has at least one node in it
      if(currNode)
      {
         //the parent node of the node we're at
      	 preNode = this->parent;
      }

      //loops while we haven't deleted all the nodes yet
      while(occupancy != 0)
      {
	  //if the current node has a left child, we want to go to that child    
          if(currNode && currNode->left)
	  {
	     preNode = currNode;	  
             currNode = currNode->left;
	  }

	  //if the current node has a right child, we want to go to that child   
          else if(currNode && currNode->right)
	  {
             preNode = currNode;
	     currNode = currNode->right;
	  }

	  //if we've deleted all a node's children already or the node is a 
	  //leaf, we can delete that node
          else
	  {
	     //local var to hold the node we want to delete	  
	     TNode * nodeToDelete = currNode; 

             if(preNode && (nodeToDelete == preNode->left))
	     {
	     
                preNode->left = 0;
	     }
	     else if(preNode && (nodeToDelete == preNode->right))
	     {
                //removes access to the node from the parent node	     
                preNode->right = 0;
	     }
 	     
             delete nodeToDelete; //deletes the node we're at

	     //goes up a node if there's a node higher in the tree
	     if(preNode) 
	     {
	        currNode = preNode;
	        preNode = currNode->parent;  
	     }

	     occupancy--;
	  }	  
       }

       //sets the pointers to 0 so we don't have access
       currNode = 0;
       preNode = 0;
   }
};

// initialization of static data fields of Tree
long TNode :: occupancy = 0;
bool Tree :: debug_on = 0;

void Tree :: Set_Debug (bool option) {
   debug_on = option;
}

Tree :: Tree (const char * name) : root (0), tree_name (strdup (name))
/***************************************************************************
% Routine Name : Tree :: Tree  (public)
% File :         Tree.c
% 
% Description :  Initializes root pointer to NULL, and occupancy to 0.
***************************************************************************/
{
   //debug
   if(debug_on)
   {
      cerr << TREE << tree_name << ALLOCATE;
   } 

   TNode :: occupancy = 0;
}

Tree :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
   //debug
   if(debug_on)
   {
      cerr << TREE << tree_name << DEALLOCATE;
   } 
    
   root->delete_AllTNodes();
   free((void*)tree_name);
}	/* end: ~Tree */


/*-----------------------------------------------------------------------
Name:                  Insert
Purpose:               Inserts a node containing some data into the tree at 
                       the location it belongs.
Description:           Uses a while loop to continue searching down the tree, 
                       going to the left or to the right depending on the 
		       value of the data compared to the rest of the tree, 
		       until a spot it can be inserted into is found. Then, 
		       the height and balance of the affected nodes are 
		       edited.
Input:                 element - pointer to the data going into the node
Result:                A node is inserted into the tree.
                       long - 1 if the insert was successful, 
		              0 if it wasn't
-----------------------------------------------------------------------*/
unsigned long Tree :: Insert (Base * element) 
{
   //the node we're currently looking at; set to root at the beginning
   TNode * working = root;
   //flag checking if the item was successfully inserted
   long hasBeenInserted = 0;

   //checks if the root node exists and inserts the first node/data as the
   //root if it doesn't
   if(!working)
   {
      working = new TNode(element);
      root = working; //sets the root pointer to the new node

      hasBeenInserted = 1;
   }

   //checks if the item has been inserted yet
   while(!hasBeenInserted)
   {
      //debug     
      if(debug_on)
      {
         cerr << TREE << tree_name << COMPARE << (char*)(*element) << 
	          AND << (char*)(*(working->data)) << "]\n";
      }	 

      //checks if it's a duplicate insertion
      if(*element == *(working->data))
      {
	 delete working->data; //deallocates memory of the original data    

         //updates the data field of the node with the new data
         working->data = element;
	 hasBeenInserted = 1;

	 //the element is not deleted anymore if it was before
	 working->hasBeenDeleted = 0;
      }

	 //checks if we should go to the right
	 else if(*element > *(working->data))
	 {
            //if the right child of the current node is null, insert there 
	    if(!(working->right))
	    {
	       //creates a new node and updates or sets all necessary fields	    
               working->right = new TNode(element);
	       working->right->parent = working;

	       hasBeenInserted = 1;
	    }

	    //updates the pointer to the node we're working with
	    working = working->right;
	 }

	 //checks if we should go to the left
	 else if(!(*element > *(working->data)))
	 {
            //if the left child of the current node is null, insert there 	
	    if(!(working->left))
	    {
	       //creates a new node and updates or sets all necessary fields
	       working->left = new TNode(element);
	       working->left->parent = working;

	       hasBeenInserted = 1;
	    }

	    //updates the pointer to the node we're working with
	    working = working->left;
	 }
	 
	 //breaks out of the while loop in an attempt to prevent an infinite
	 //loop
	 else
	 {
            break;
	 }
      }

      //checks if we've reached the top of the tree; only runs if we've 
      //inserted something
      while(working && hasBeenInserted)
      {
         //holds the height of the right child of the working node	      
         long rightHeight = -1;
         //holds the height of the left child of the working node
	 long leftHeight = -1;

	 //if there is a right child, get its height
	 if(working->right)
	 {
            rightHeight = working->right->height;
	 }
	 
	 //if there is a left child, get its height
	 if(working->left)
	 {
           leftHeight = working->left->height;
	 }

	 //if both the right child and the left child were null, the working
	 //node is a leaf and the height is 0
	 if(rightHeight == -1 && leftHeight == -1)
	 {
            working->height = 0;
	 }

	 //updates the height of the node to be the height of the tallest 
	 //child + 1
	 else if(rightHeight >= leftHeight)
	 {
            working->height = rightHeight + 1;
	 }
	 else if(leftHeight > rightHeight)
	 {
            working->height = leftHeight + 1;
	 }

	 //updates the balance of the node to be the left child's height -
	 //the right child's height
	 working->balance = leftHeight - rightHeight;

	 //updates the pointer to go up the tree
	 working = working->parent;	 
      }

      //debug message
      if(debug_on)
      {
         cerr << TREE << tree_name << INSERT << (char*)(*element) << "]\n";
      }      

      return hasBeenInserted;
}


/*-----------------------------------------------------------------------
Name:                  Lookup
Purpose:               Looks to see if a specific data is in the tree.
Description:           Goes down the tree depending on the value of the 
                       element we're looking for. If we find a node containing
		       data with the same name as the element we're looking
		       for, it's a successful search. Else, the element isn't
		       there.
Input:                 element - pointer to the data we want to look for
Result:                Base * - returns the data if we find it, 
                                0/NULL if we didn't
-----------------------------------------------------------------------*/
const Base * Tree :: Lookup (const Base * element) const {
   //the node we're currently looking at; set to root at beginning	   
   TNode * working = root; 

   //loops until we reach the bottom of the tree
   while(working)
   {
      //debug  
      if(debug_on)
      {
         cerr << TREE << tree_name << COMPARE << (char*)(*element) <<
		  AND << (char*)(*(working->data)) << "]\n";
      }

      //if the name of the data in the current node is equal to the name
      //of the data, we've found it
      if(*(working->data) == *element)
      {
         //only returns the data if the node hasn't been removed
         if(!(working->hasBeenDeleted))	
	 {	 
            return working->data;
	 }
	 else
	 {
            return 0;
	 }
      }

      //keep going if we haven't found it yet
      else if(*element > *(working->data))	 
      {
         //go to the right if element is greater alphabetically	      
         working = working->right; 
      }
      else
      {
         //go to the left if element is not greater alphabetically	      
         working = working->left;
      }

   }

   return 0;
}


/*-----------------------------------------------------------------------
Name:                  Remove
Purpose:               Removes a specific data if it's in the tree.
Description:           Goes down the tree depending on the value of the 
                       element we're looking for. If we find a node containing
		       data with the same name as the element we're looking 
		       for, we remove it. Else, the element isn't there.
Input:                 element - pointer to the data we want to remove
Result:                A node is "removed."
                       Base * - returns the data if we found and removed it, 
                                0/NULL if we didn't
-----------------------------------------------------------------------*/
Base * Tree :: Remove (const Base * element) {
   //the node we're currently looking at; set to root at the beginning	   
   TNode * working = root;

   //loops until we reach the bottom of the tree      
   while(working)
   {
      //debug     
      if(debug_on)
      {
         cerr << TREE << tree_name << COMPARE << (char*)(*element) <<
		  AND << (char*)(*(working->data)) << "]\n";
      }

      //if the name of the data in the current node is equal to the name
      //of the data, we've found the node we want to remove	 
      if(*(working->data) == *element)
      {
         //only "removes" the node if it hasn't been removed yet 	 
         if(!(working->hasBeenDeleted))	
         {
            //updates the needed fields	    
            working->hasBeenDeleted = 1; 
	    TNode :: occupancy--;
                 	       
            return working->data;
         }
         else
         {
            return 0;
	 }
      }

      //keep going if we haven't found it yet	 
      else if(*element > *(working->data))	 
      {
         //go to the right if element is greater alphabetically	 	      
         working = working->right;
      }
      else
      {
         //go to the left if element is not greater alphabetically	      
         working = working->left;
      }
   }     
   
   return 0;     
}

ostream & Tree :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree :: Write (public)
% File :         Tree.c
% 
% Description : This function will output the contents of the Tree table
%               to the stream specificed by the caller.  The stream could be
%               cerr, cout, or any other valid stream.
%
% Parameters descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% stream             A reference to the output stream.
% <return>           A reference to the output stream.
***************************************************************************/

{
        stream << "Tree " << tree_name << ":\n"
		<< "occupancy is " << TNode :: occupancy << " elements.\n";

	return (root) ? root->Write_AllTNodes (stream) : stream;
}       /* end: Write */

