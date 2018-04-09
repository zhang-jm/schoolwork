/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        March 2, 2015
                                                        cs12xre
                                Assignment Eight

File Name:      Tree.c
Description:    This file contains the constructor(s), destructor and 
                functions that define a Tree and the Nodes of the Tree. 
		The constructor is called to allocate and initialize a new 
		Tree or a new Node. The destructor deallocates memory.
		The Tree methods can be called to insert a new item 
	        into the Tree, look up an existing item in the Tree, 
		remove an item from the Tree, or write out the Tree. The 
		Node methods can be used to write out the Node.
****************************************************************************/

#include <cstdlib>
#include <string>
#include "Tree.h"
using namespace std;

// messages
static const char AND[] = " and ";
static const char COMPARE[] = " - Comparing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char INSERT[] = " - Inserting ";
static const char REPLACE[] = " - Replacing ";
static const char UPDATE[] = " - Updating ";

template <class Whatever>
int Tree<Whatever>::debug = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);

template <class Whatever>

/*-----------------------------------------------------------------------
Name:                  TNode
Description:           Nodes holding the data that will be inserted into 
                       the Tree.
Data Fields:           balance - balance of the node
                       data - data/object going into the tree
		       height - height of the node
		       left - pointer to the left child of the node
		       occupancy - number of nodes in the tree
		       right - pointer to the right child of the node
		       tree_count - what tree it is
-----------------------------------------------------------------------*/
struct TNode 
{
   long balance;
   Whatever data;
   long height;
   TNode<Whatever> * left;
   long & occupancy;
   TNode<Whatever> * right;
   unsigned long & tree_count;
        

   /*-----------------------------------------------------------------------
   Name:                  TNode
   Purpose:               Constructor for TNode.
   Description:           Initializes the TNode's data fields.
   Input:                 element - the data to go into the TNode
                          theTree - the tree the TNode belongs to
   -----------------------------------------------------------------------*/  
   TNode (const Whatever & element, Tree<Whatever> & theTree)
   : balance (0), data (element), height (0), left (0), 
   occupancy (theTree.occupancy), right (0), 
   tree_count (theTree.tree_count) 
   {	      
      occupancy++; //increments the number of TNodes in the tree
   }
     

   /*-----------------------------------------------------------------------
   Name:                  TNode
   Purpose:               Constructor for TNode.
   Description:           Initializes the TNode's data fields.
   Input:                 element - the data to go into the TNode
                          parentTNode - reference to the parent of the 
			                TNode about to be created
   -----------------------------------------------------------------------*/  
   TNode (const Whatever & element, TNode<Whatever> & parentTNode)
   : balance (0), data (element), height (0), left (0), 
   occupancy (parentTNode.occupancy), right (0), 
   tree_count (parentTNode.tree_count) 
   {
      occupancy++; //increments the number of TNodes in the tree
   }


   /*-----------------------------------------------------------------------
   Name:                  ~TNode
   Purpose:               Destructor for the UCSDStudent.
   Description:           Decrements the occupancy of the tree. No data
                          fields need to be deallocated.
   Input:                 none
   -----------------------------------------------------------------------*/  
   ~TNode (void) 
   {
      occupancy--; //decrements the number of TNodes in the tree
   }


   /*-----------------------------------------------------------------------
   Function Name:         delete_AllTNodes
   Purpose:               Deallocates all the TNodes in the tree below and
                          including the current TNode. Deallocates all nodes 
			  in the tree if called from root.
   Description:           Performs a post-order traversal through the tree
                          to deallocate all TNodes.
   Input:                 void - no input
   Result:                void - no return type
   -----------------------------------------------------------------------*/
   void delete_AllTNodes (void) 
   {
      //calls delete_AllTNodes on the left child if it exists	   
      if(left)
      {
         left->delete_AllTNodes();
      }

      //calls delete_AllTNodes on the right child if it exists      
      if(right)
      {
         right->delete_AllTNodes();
      }

      //deallocates the current node if it doesn't have any children
      delete this;
   }


   unsigned long Insert (const Whatever & element, 
              TNode<Whatever> *& PointerInParent);


   /*-----------------------------------------------------------------------
   Function Name:         Lookup
   Purpose:               Checks to see if an element is in the tree or not. 
   Description:           If element we're searching for is in the current
                          node, we return true and return back the data. 
			  Else, if the element is greater than the data in
			  the current node, we want to call Lookup on the 
			  right child of the current node. If the element is
			  smaller than the data in the current node, we 
			  recursively call Lookup on the left child of the 
			  current node.
   Input:                 element - a reference to the element we want to 
                                    look for; if the element is in the tree, 
				    we use it to send back the data in the
				    tree matching the element we want to look
				    for
   Result:                long - 1 if we found what we're looking for
                                 0 if we didn't find it
   -----------------------------------------------------------------------*/
   unsigned long Lookup(Whatever & element) const
   {
      long found = 0;	  

      //if the node contains the element we're looking for, end the recursion
      if(element == data)
      {
         //returns the data in the tree back to the caller	      
         element = this->data; 

         return 1;
      }

      else if(element > data)
      {
	 //if there's no right child, the element isn't in the tree     
         if(!right)
	 {
            return 0;
	 }
	 //if there's a right child, recursively call Lookup
	 else
	 {
            found = right->Lookup(element);
	 }
      }

      else
      {
	 //if there's no left child, the element isn't in the tree    	      
	 if(!left)
	 {
            return 0;
	 }
	 //if there's a left child, recursively call Lookup	 
	 else
	 {
            found = left->Lookup(element);
	 }
      }
      return found;
   }


   /*-----------------------------------------------------------------------
   Function Name:         ReplaceAndRemoveMax
   Purpose:               Removes an node in the tree and replaces it
                          with its predecessor node (the largest node in the
			  left subtree of the node to remove). 
   Description:           Recursively calls RARM until we find the predecessor
                          node to the node we want to remove. We then move
			  the data of the predecessor node into the node we
			  want to remove. If the predecessor node had left
			  children, we attach those children to the parent
			  of the predecessor node.
   Input:                 targetTNode - a reference to the node we want to 
                                        remove
                          PointerInParent - a reference to the TNode pointer 
			                    in the parent TNode used to get 
					    to the current TNode  
   Result:                void - no return type
   -----------------------------------------------------------------------*/  
   void ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
              TNode<Whatever> *& PointerInParent)
   {
      //debug	   
      if(Tree<Whatever>::debug)
      {
         cerr << TREE << tree_count << COMPARE << 
              (const char*)(PointerInParent->data) << "]\n";
      }

      //if we're at the rightmost node of the left subtree
      if(!(PointerInParent->right))
      {
	 //debug     
         if(Tree<Whatever>::debug)
         {           		 
            cerr << TREE << tree_count << REPLACE << 
		    (const char *)(PointerInParent->data) << "]\n";
         }

	 //pointer to the rightmost node so we can delete it later
         TNode<Whatever> * toBeDeleted = PointerInParent;

	 //grabs the data in the predecessor node and puts it into the 
	 //node we want to remove
         targetTNode.data = PointerInParent->data;

	 //checks if the predecessor node is the left child of the TNode 
	 //we want to remove and if it has a left child
	 if(targetTNode.left == PointerInParent && 
	    PointerInParent->left)
	 {
	    //sets the left child of the node we want to remove to the
	    //left child of the node we're replacing it with	 
            targetTNode.left = PointerInParent->left;
	 }
         //sets the left child of the node we want to remove to null if
	 //the predecessor doesn't have a left child
	 else if(targetTNode.left == PointerInParent
		 && !(PointerInParent->left))
	 {
	    targetTNode.left = 0;
	 }

	 //dellocates the predecessor node
	 delete toBeDeleted;
      }

      else
      {
	 //a pointer to the next node we're calling RARM on     
         TNode<Whatever> * nextPIP = PointerInParent->right;

	 //if the next node is the predecessor node and it has a left
	 //child, set the left child to the right pointer of the parent of
	 //the predecessor node
	 if(PointerInParent->right->left && !(PointerInParent->right->right))
	 {
            PointerInParent->right = PointerInParent->right->left;
	 }
         //set the right pointer of the predecessor node to null if the 
	 //predecessor node doesn't have a left child
	 else if (!(PointerInParent->right->right))
	 {
	    PointerInParent->right = 0;
	 }

	 //recursively call RARM
         ReplaceAndRemoveMax(targetTNode, nextPIP);

	 //call SHAB to update PIP (since its children were possibly 
	 //changed above)
         SetHeightAndBalance(PointerInParent);
      }

   }


   /*-----------------------------------------------------------------------
   Function Name:         Remove
   Purpose:               Removes an node from the tree.
   Description:           Recursively calls Remove until we find the node that
                          we want to remove from the tree. If the node has 0
			  children, we remove the node and set its parent's 
			  pointer to null. If the node has 1 child, we 
			  remove the node and set its parent's pointer to the
			  child. If the node has 2 children, we call RARM to 
			  take care of the removal.
   Input:                 elementTNode - a reference to a node with the data
                                         that identifies the node we want to 
					 remove
                          PointerInParent - a reference to the TNode pointer 
			                    in the parent TNode used to get 
					    to the current TNode  
			  fromSHAB - true/false value tracking whether or not
			             the method was called from SHAB
   Result:                long - 1 if the remove was successful, 0 if the
                                 remove failed
   -----------------------------------------------------------------------*/  
   unsigned long Remove (TNode<Whatever> & elementTNode, 
              TNode<Whatever> *& PointerInParent,
              long fromSHAB = FALSE)
   {
      long removed = 0; //holds the return value from recursion

      //debug
      if(Tree<Whatever>::debug)
      {
         cerr << TREE << tree_count << COMPARE << 
		 (const char*)elementTNode.data 
	      << AND << (const char*)(PointerInParent->data) << "]\n";
      }

      //if the element we want to find is in the node pointed to by PIP
      if(elementTNode.data == PointerInParent->data)
      {
	 //if the node only has 1 child     
         if(!(PointerInParent->left 
	   && PointerInParent->right))
	 {
	    //TNode pointer to the node we want to remove	 
            TNode<Whatever> * toBeRemoved = PointerInParent;
	       
	    //sets PIP to be the child of the node to be removed (since it
	    //only has one child)
            if(PointerInParent->left)
            {
               PointerInParent = PointerInParent->left;

	       //updates the height and balance of that child node         
   	       if(!fromSHAB)
	       {
                  SetHeightAndBalance(PointerInParent);
	       }
            }
            else if(PointerInParent->right)
            {
               PointerInParent = PointerInParent->right;

	       //updates the height and balance of that child node
	       if(!fromSHAB)
	       {
                  SetHeightAndBalance(PointerInParent);
	       }	       
            }

            //grabs the data from the node we want to remove to be outputted
	    //later
	    elementTNode.data = toBeRemoved->data;

	    //delete the node we want to remove
	    delete toBeRemoved;

	    return 1;
	 }

	 else
	 {
	    //call RARM if the node has 2 children	 
            ReplaceAndRemoveMax(*PointerInParent, PointerInParent->left);

	    //updates PIP if it wasn't called from SHAB
	    if(!fromSHAB)
	    {
	       SetHeightAndBalance(PointerInParent);
	    }

	    return 1; 
	 }

      }

      //if the element we want to remove has a greater value than the 
      //current element, we want to check right
      else if(elementTNode.data > PointerInParent->data)
      {
	 //debug     
         if(Tree<Whatever>::debug)
         {
            cerr << TREE << tree_count << COMPARE << 
		 (const char*)elementTNode.data << AND << 
		 (const char*)(PointerInParent->right->data) << "]\n";
         }

	 //if there's no right child, the element isn't there and we can't
	 //remove it
         if(!(PointerInParent->right))
	 {
            return 0;
	 }

	 //checks if the right child is what we want to remove
	 else if(PointerInParent->right->data == elementTNode.data)
	 {
	    //if the right child doesn't have 2 children	 
            if(!(PointerInParent->right->left 
	      && PointerInParent->right->right))
	    {
	       //TNode pointer to the node we want to remove
               TNode<Whatever> * toBeRemoved = PointerInParent->right;
	       
	       //sets PIP's right child to be the left child of the node to 
	       //be removed if it has a child	       
	       if(PointerInParent->right->left)
	       {
	          PointerInParent->right = PointerInParent->right->left;
	       }
	       //sets PIP's right child to be the right child of the node to 
	       //be removed if it has a child	       
	       else if(PointerInParent->right->right)
	       {
	          PointerInParent->right = PointerInParent->right->right;  
	       }
	       //sets PIP's right child to be null if the node to be removed
	       //doesn't have children
	       else
	       {
	          PointerInParent->right = 0; 
	       }

	       //grabs the data of the node to be removed to be outputted
	       elementTNode.data = toBeRemoved->data;
	       delete toBeRemoved; //deallocate the node
	       
	       //call SHAB to update PIP (since its children were edited 
	       //above)
	       SetHeightAndBalance(PointerInParent);
	       return 1;
	    }	

            else
	    {
	       //calls RARM if the node to remove has 2 children	    
               ReplaceAndRemoveMax(*PointerInParent->right, 
			       PointerInParent->right->left); 

	       SetHeightAndBalance(PointerInParent->right);

	       return 1;
	    }	    
	 }
	 else
	 {
	    //recursively calls Remove on the right child of the current node 
            removed = PointerInParent->right->Remove(elementTNode, 
			    PointerInParent->right, 0);
	    SetHeightAndBalance(PointerInParent);
	 }
      }

      //if the element we want to remove has a lesser value than the 
      //current element, we want to check left      
      else
      {
	 //debug  	      
         if(Tree<Whatever>::debug)
         {
            cerr << TREE << tree_count << COMPARE << 
		 (const char*)elementTNode.data << AND << 
		 (const char*)(PointerInParent->left->data) << "]\n";
         }  

	 //if there's no left child, the element isn't there and we can't
	 //remove it	 
         if(!(PointerInParent->left))
	 {
            return 0;
	 }
	 
	 //checks if the left child is what we want to remove	 
	 else if(PointerInParent->left->data == elementTNode.data)
	 {
	    //if the left child doesn't have 2 children		 
            if(!(PointerInParent->left->left 
	      && PointerInParent->left->right))
	    {
	       //TNode pointer to the node we want to remove		    
               TNode<Whatever> * toBeRemoved = PointerInParent->left;

	       //sets PIP's left child to be the left child of the node to 
	       //be removed if it has a child	  	       
	       if(PointerInParent->left->left)
	       {
	          PointerInParent->left = PointerInParent->left->left;
	       }
	       //sets PIP's left child to be the right child of the node to 
	       //be removed if it has a child	   	       
	       else if(PointerInParent->left->right)
	       {
	          PointerInParent->left = PointerInParent->left->right;  
	       }
	       //sets PIP's left child to be null if the node to be removed
	       //doesn't have children	       
	       else
	       {
	          PointerInParent->left = 0; 
	       }

	       //grabs the data of the node to be removed to be outputted     
	       elementTNode.data = toBeRemoved->data;
	       delete toBeRemoved;

	       //call SHAB to update PIP (since its children were edited 
	       //above)	       
	       SetHeightAndBalance(PointerInParent);
	       return 1;
	    }	

            else
	    {
	       //calls RARM if the node to remove has 2 children	    
               ReplaceAndRemoveMax(*PointerInParent->left, 
			       PointerInParent->left->left); 
	       
               SetHeightAndBalance(PointerInParent->left);

	       return 1;
	    }	    
	 }
	 else
	 {
            //recursively calls Remove on the right child of the current node 
            removed = PointerInParent->left->Remove(elementTNode, 
			    PointerInParent->left, 0);
	    SetHeightAndBalance(PointerInParent);
	 }
      }
	  
      return removed;
   }


   /*-----------------------------------------------------------------------
   Function Name:         SetHeightAndBalance
   Purpose:               Updates the height and balance of a node in a tree.
   Description:           Gets the heights of the left and right children if
                          the node has any. Finds the height of the node
			  by taking 1 + the height of the tallest child.
			  Finds the balance of the tree by taking the
			  height of the leftchild - height of the right child.
			  If the balance gets over the threshold, call RARM
			  to remove the node and reinsert it into the tree.
   Input:                 PointerInParent -  reference to the TNode pointer 
                                             in the parent TNode used to get
					     to the current TNode
   Result:                void - no return
   -----------------------------------------------------------------------*/  
   void SetHeightAndBalance (TNode<Whatever> *& PointerInParent) 
   {
      long rightHeight = -1; //holds the right child's height
      long leftHeight = -1; //holds the left child's height

      //debug
      if(Tree<Whatever>::debug)
      {
         cerr << TREE << tree_count << UPDATE << 
		 (const char *)(PointerInParent->data)
		 << "]\n";
      }

      if(PointerInParent)
      {
         //grabs the right child's height if the right child exists
         if(PointerInParent->right)
         {
            rightHeight = PointerInParent->right->height;
         }     

         //grabs the left child's height if the left child exists
         if(PointerInParent->left)
         {
            leftHeight = PointerInParent->left->height;
         }

	 //sets the height = 0 if the node is a leaf node
         if(leftHeight == -1 && rightHeight == -1)
         {
            PointerInParent->height = 0;
         }

	 //else the height is set to 1 + height of tallest child
         else if(rightHeight >= leftHeight)
         {
            PointerInParent->height = rightHeight + 1;
         }
         else if(leftHeight > rightHeight)
         {
            PointerInParent->height = leftHeight + 1;
         }

	 //sets the balance
         PointerInParent->balance = leftHeight - rightHeight;

	 //removes the node if the balance is over the threshold and 
	 //reinserts it into the tree at a different spot
         if(PointerInParent->balance > THRESHOLD || 
            PointerInParent->balance < -THRESHOLD )
         {
            TNode<Whatever> temp (PointerInParent->data, *PointerInParent);

	    PointerInParent->Remove(temp, PointerInParent, 1);
	    PointerInParent->Insert(temp.data, PointerInParent);
         }
      }
   }


   ostream & Write_AllTNodes (ostream & stream) const {
      if (left)
         left->Write_AllTNodes (stream);
      stream << *this;
      if (right)
         right->Write_AllTNodes (stream);

      return stream;
   }
};

/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Set_Debug_On
Purpose:               Turns debug messages on.
Description:           Sets value of the debug variable to true.
Input:                 void - no input
Result:                void - no output
-----------------------------------------------------------------------*/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_On()
{
   debug = 1;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Set_Debug_Off
Purpose:               Turns debug messages off.
Description:           Sets value of the debug variable to false.
Input:                 void - no input
Result:                void - no output
-----------------------------------------------------------------------*/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off()
{
   debug = 0;
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: Insert
Purpose:               Inserts a TNode into the tree.
Description:           If the element we want to insert is the same as
                       the current node, it's a duplicate insert, so replace
		       the old data with the new data. If the element 
		       we want to insert is greater than the current node, 
		       call Insert again on the right child of the current
		       node if it exists or insert at the right child if it 
		       doesn't. If the element we want to insert is less than
		       the current node, call Insert again on the left child 
		       of the current node if it exists or insert at the
		       left child if it doesn't.
Input:                 element - reference to the data we want to insert
                       PointerInParent - a reference to the TNode pointer 
			                    in the parent TNode used to get 
					    to the current TNode  
Result:                long - 1 if insertion was successful, 0 if the
                              insertion failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (const Whatever & element, 
      TNode<Whatever> *& PointerInParent) 
{
   //debug	
   if(Tree<Whatever>::debug)
   {
      cerr << TREE << tree_count << COMPARE << (const char*)element 
	      << AND << (const char*)(PointerInParent->data) << "]\n";
   }

   //duplicate insertion
   if(element == PointerInParent->data)
   {
      //debug	   
      if(Tree<Whatever>::debug)
      {
         cerr << TREE << tree_count << INSERT << (const char*)element 
    	      << "]\n";
      }

      //replace old data with new data
      PointerInParent->data = element;
   }

   //element is greater than current node's data; go right
   else if(element > PointerInParent->data)
   {
      //insert as right child if current node doesn't have a right child   
      if(!(PointerInParent->right))
      {
	 //debug     
	 if(Tree<Whatever>::debug)
         {
            cerr << TREE << tree_count << INSERT << (const char*)element 
		 << "]\n";
	 }

         PointerInParent->right = new TNode<Whatever>(element, 
			 *PointerInParent);
         //update height and balance of current node
	 SetHeightAndBalance(PointerInParent);
	 return 1;
      }
      //recursively calls Insert if there is a right child
      else
      {
         PointerInParent->right->Insert(element, PointerInParent->right);
	 SetHeightAndBalance(PointerInParent);
      }
   }

   //element is less than current node's data; go right   
   else
   {
      //insert as left child if current node doesn't have a left child  
      if(!(PointerInParent->left))
      {
	 //debug  	      
	 if(Tree<Whatever>::debug)
         {
            cerr << TREE << tree_count << INSERT << (const char*)element 
		 << "]\n";
	 }

         PointerInParent->left = new TNode<Whatever>(element, 
			 *PointerInParent);
	 //update height and balance of current node
	 SetHeightAndBalance(PointerInParent);
	 return 1;
      }
      //recursively calls Insert if there is a left child      
      else
      {
         PointerInParent->left->Insert(element, PointerInParent->left);
	 SetHeightAndBalance(PointerInParent);
      }
   }

   return 0;
}


template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
        stream << "at height:  :" << nnn.height << " with balance:  "
                << nnn.balance << "  ";
        return stream << nnn.data << "\n";
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Insert
Purpose:               Inserts a node into the tree.
Description:           Insert at the root if no items are in the tree. 
                       Else, delegate to root's Insert.
Input:                 element - the data to go into the tree
Result:                long - 1 if the insert was successful, 0 if the
                              insert failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (const Whatever & element) 
{
   long inserted = 0; //holds if the insert was successful or not

   //if there are no items in the tree, insert the data as the root node
   //of the tree
   if(!root)
   {
      //debug	   
      if(debug)
      {
         cerr << TREE << tree_count << INSERT << (const char*)element 
              << "]\n";
      }

      root = new TNode<Whatever>(element, *this);

      inserted = 1;
   }

   //delegate to root's insert
   else
   {
      inserted = root->Insert(element, root);
   }
 
   return inserted;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Lookup
Purpose:               Checks if a node is in the tree.
Description:           If there are items in the tree, delegate to root's
                       Lookup.
Input:                 element - the item we want to look for in the tree
Result:                long - 1 if the item was found, 0 if it wasn't
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const 
{
   long found = 0; //holds if the lookup was successful or not

   //delegates to root's Lookup if root exists
   if(occupancy != 0)
   {
      found = root->Lookup(element);
   }
  
   return found;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Remove
Purpose:               Removes a node from the tree.
Description:           If there are items in the tree, delegate to root's
                       Remove.
Input:                 element - the item we want to remove from the tree
Result:                long - 1 if the item was removed, 0 if it wasn't
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) 
{
   //holds whether the remove was successful or not	
   long removed = 0; 

   //creates a new node holding the element we want to look for in the tree
   //and remove
   TNode<Whatever> * temp = new TNode<Whatever>(element, *this);

   //delegates to root's remove if occupancy isn't 0
   if(occupancy != 0)
   {
      removed = root->Remove(*temp, root, 0);
   }

   //grabs the data put into temp by root's Remove to be outputted
   element = temp->data;
   delete temp; //deallocates temp to decrement occupancy

   //sets the root pointer to null if occupancy is 0
   if(occupancy == 0)
   {
      root = 0;
   }

   return removed;
}
                                

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
% Routine Name : Tree<Whatever> :: ~Tree  (public)
% File :         Tree.c
% 
% Description :  deallocates memory associated with the Tree.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{
   if(debug)
   {
      cerr << TREE << tree_count << DEALLOCATE;
   }

   if(occupancy != 0)
   {
      root->delete_AllTNodes();
   }
}


template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
% Routine Name : Tree<Whatever> :: Write (public)
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
        stream << "Tree " << tree_count << ":\n"
                << "occupancy is " << occupancy << " elements.\n";

        return (root) ? root->Write_AllTNodes (stream) : stream;
}
