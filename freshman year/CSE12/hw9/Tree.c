/****************************************************************************

                                                Jamie Zhang
						cs12xre
                                                Christopher Cabreros
					        cs12xjm
                                                CSE 12, W15
                                                March 12

                              Assignment 9

File Name:      Tree.c
Description:    Contains all the constructors and functions for Tree and
                TNode. The functions allow the insertion, removal, and
		lookup of a TNode into the tree by reading and writing to
		a disk file, as well as to write out the whole tree or a
		TNode.
****************************************************************************/

#include <stdlib.h>
#include <string.h>
#include "Tree.h"

// debug messages
static const char ALLOCATE[] = " - Allocating]\n";
static const char COST_READ[] = "[Cost Increment (Disk Access): Reading ";
static const char COST_WRITE[] = "[Cost Increment (Disk Access): Writing ";
static const char DEALLOCATE[] = " - Deallocating]\n";
static const char TREE[] = "[Tree ";

template <class Whatever>
int Tree<Whatever>::debug_on = 0;

template <class Whatever>
long Tree<Whatever>::cost = 0;

template <class Whatever>
long Tree<Whatever>::operation = 0;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#define THRESHOLD 2

template <class Whatever>
ostream & operator << (ostream &, const TNode<Whatever> &);


/*-----------------------------------------------------------------------
Name:                  TNode
Description:           Nodes holding the data that will be inserted into 
                       the Tree.
Data Fields:           data - data/object going into the tree
		       height - height of the node
		       balance - balance of the node
		       left - location in the datafile of the left child
		       right - location in the datafile of the right child
		       this_position - location in the datafile of the
		                       current node
-----------------------------------------------------------------------*/
template <class Whatever>
struct  TNode {
   // friends:

   // data fields:
   Whatever data;
   long height;
   long balance;
   offset left;
   offset right;
   offset this_position;	// current position

   // function fields:
   TNode () : height (0), balance (0), left (0), right (0), 
   this_position (0) {}

   // to declare the working TNode in Tree's Remove
   TNode (Whatever & element) : data (element), height (0), balance (0),
   left (0), right (0), this_position (0) {}

   TNode (Whatever &, fstream *, long &);	// to add new node to disk
   TNode (const offset &, fstream *);	// to read node from disk

   unsigned long Insert (Whatever &, fstream *, long &, offset &);
   unsigned long Lookup (Whatever &, fstream *);
   void Read (const offset &, fstream *);	// read node from disk
   unsigned long Remove (TNode<Whatever> &, fstream *, long &, offset &,
         long fromSHB = FALSE);
   void ReplaceAndRemoveMax (TNode<Whatever> &, fstream *, offset &);
   void SetHeightAndBalance (fstream *, offset &);
   void Write (fstream *) const;		// update node to disk

   ostream & Write_AllTNodes (ostream &, fstream *) const;
};

/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Set_Debug_On
Purpose:               Turns debug messages on.
Description:           Sets value of the debug variable to true.
Input:                 void - no input
Result:                void - no output
-----------------------------------------------------------------------*/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_On(){
   debug_on = 1;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Set_Debug_Off
Purpose:               Turns debug messages off.
Description:           Sets value of the debug variable to false.
Input:                 void - no input
Result:                void - no output
-----------------------------------------------------------------------*/
template <class Whatever>
void Tree<Whatever> :: Set_Debug_Off(){
   debug_on = 0;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Insert
Purpose:               Inserts a TNode into the tree.
Description:           If the tree is empty, we insert the root node and
                       write it to the tree. Otherwise, we read in the root
		       node and delegate the inserting to its Insert.
Input:                 element - the data we want to insert into the tree
Result:                long - 1 if the insert succeeded, 0 if the insert
			      failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Insert (Whatever & element) {
   long inserted = 0; //flag for if the item was successfully inserted or not

   //check if tree is empty
   if(occupancy == 0) {
      //insert root node
      TNode<Whatever> root (element, fio, occupancy);   

      inserted = 1;
   }

   else {
      //gets the root node
      TNode<Whatever> readRootNode (root, fio);

      //delegate to the root node's insert
      inserted = readRootNode.Insert(element, fio, occupancy, root);
   }

   //increments operation because a tree operation occurred
   IncrementOperation();

   return inserted;
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: ReplaceAndRemoveMax
Purpose:               Removes a node and replaces it with its predecessor
                       node.
Description:           Keep recursively calling ReplaceAndRemoveMax on the
                       right child of the current node until we find a node
		       without a right child. Replace the data in the node
		       we want to remove with the data in the node we just
		       found (the predecessor node). Remove the predecessor
		       node from the tree.
Input:                 targetTNode - the a reference to the TNode we want to
                                     remove from the tree
	               fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
		       PositionInParent - a reference to the TNode position 
		                    in the parent TNode used to get to the 
				    current TNode's offset in the datafile
Result:                void - no output
-----------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: ReplaceAndRemoveMax (TNode<Whatever> & targetTNode, 
      fstream * fio, offset & PositionInParent) {
   
      //keep going right until you find a leaf
      if (right){
         TNode<Whatever> rightChild (right, fio);
         rightChild.ReplaceAndRemoveMax(targetTNode, fio, right);
	 
         //set the height and balance after the change in positions
         SetHeightAndBalance(fio, PositionInParent);
      }

      //otherwise set the data of the targetTNode to current data,
      //set pointer to null, and delete the node
      else{
         //replace the data into the target TNode
         targetTNode.data = data;
	 
         //assign children correctly, if possible
         PositionInParent = left;

      }
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: Remove
Purpose:               Removes a node from the tree.
Description:           If the node we want to remove is the current node,
                       remove it and replace it with its child if it has 1
		       child, set it to null if it has 0 children, or call
		       RARM if it has 2 children. Otherwise, if the node
		       we want to remove is greater or less than the 
		       current node, go left or right depending and call
		       Remove recursively on the current node's child if 
		       it exists. If the child doesn't exist, the item isn't
		       in the tree and we can't remove it.
Input:                 elementTNode - the a reference to the TNode we want to
                                     remove from the tree
	               fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
	               occupancy - the occupancy of the tree
		       PositionInParent - a reference to the TNode position 
		                    in the parent TNode used to get to the 
				    current TNode's offset in the datafile
Result:                long - 1 if the remove succeeded, 0 if the remove
			      failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long TNode<Whatever> :: Remove (TNode<Whatever> & elementTNode,
      fstream * fio, long & occupancy, offset & PositionInParent,
      long fromSHB) {
   
   unsigned long result = 0; //result of the many function calls

   //case left
   if (data > elementTNode.data){
      //check if left is null
      if (!left){
         result = 0;
      }
      else {
         //make a left child and recursively call remove
         TNode<Whatever> leftChild (left, fio);
	 result = leftChild.Remove(elementTNode, fio, occupancy, left, 
               fromSHB);
        
	 //update the tree, if not from setHeightAndBalance
         if (!fromSHB){
            SetHeightAndBalance(fio, PositionInParent);
	 }
      }
   }

   //case center
   else if (data == elementTNode.data){
      //assign data to element TNode
      elementTNode.data = data;
      //update occupancy
      occupancy--;
      //tell driver of successful operation
      result = 1;

      //case 1 left child
      if (left && !right){
         //set positionInParent to the left
         PositionInParent = left;
      }
     
      //case 1 right child
      else if (!left && right){
         //set positionInParent to the right
         PositionInParent = right;
      }

      //case 2 children
      else if (left && right){
         //read the left TNode
         TNode<Whatever> leftChild(left, fio);

         //call replaceAndRemoveMax on the left node
         leftChild.ReplaceAndRemoveMax(*this, fio, left);
         
	 Write(fio);
      }

      //case no children
      else{
         //set positionInParent
         PositionInParent = 0;
      }
   }

   //case right
   else{
      //check if right is null
      if (!right){
         result = 0;
      }
      else{
         //make a right child and recursively call remove
         TNode<Whatever> rightChild (right, fio);
	 result = rightChild.Remove(elementTNode, fio, occupancy, right,
	    fromSHB);
        
	 //update the tree, if not from setHeightAndBalance
         if (!fromSHB){
            SetHeightAndBalance(fio, PositionInParent);
         }
      }
   }

   //tell driver result of the remove
   return result;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: Remove
Purpose:               Removes a node from the tree.
Description:           If the tree is empty, do nothing. Otherwise, delegate
                       to the root node's remove method.
Input:                 element - the item we want to remove from the tree.
Result:                long - 1 if the remove was successful, 0 if the
                              remove failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Remove (Whatever & element) {
   long removed = 0; //flag for if the item was successfully removed or not

   //check if tree is empty
   if (occupancy == 0){
      //do nothing
   }

   //otherwise delegate to remove of the tree
   else{
      TNode<Whatever> tempNode(element);

      //get the root node
      TNode<Whatever> readRootNode(root, fio);

      //delegate to the root node's remove
      removed = readRootNode.Remove(tempNode, fio, occupancy, root, 0);

      //change the element input parameter
      element = tempNode.data;

      //increment operations because a tree operation occured
      IncrementOperation();

      //check if the last node was removed
      if (occupancy == 0){
          ResetRoot();
      }
   }
   return removed;
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: SetHeightAndBalance
Purpose:               Updates the height and balance of the current node.
Description:           Gets the height and balances of the current node's 
                       children if they exist. Finds the height by finding
		       1 plus the tallest child's height. Finds the balance
		       by subtracting left child's height - right child's 
		       height. If the balance is over 2 or less than -2, 
		       remove and then reinsert the current node into the 
		       tree. 
Input:                 fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
		       PositionInParent - a reference to the TNode position 
		                    in the parent TNode used to get to the 
				    current TNode's offset in the datafile
Result:                void - no output
-----------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: SetHeightAndBalance (fstream * fio,
      offset & PositionInParent) {
   long leftHeight = -1; //variable holding the height of the left child
   long rightHeight = -1; //variable holding the height of the right child

   //checks if we have a left child
   if(left){
      //reads in the left child and grabs its height
      TNode<Whatever> leftChild (left, fio);
      leftHeight = leftChild.height;
   }

   //checks if we have a right child
   if(right){
      //reads in the right child and grabs its height	   
      TNode<Whatever> rightChild (right, fio);
      rightHeight = rightChild.height;
   }

   //sets the node's height to the height of the tallest child + 1
   if(leftHeight >= rightHeight){
      height = leftHeight + 1;
   }
   else{
      height = rightHeight + 1;
   }

   //sets the balance to height of the left child - height of the right child
   balance = leftHeight - rightHeight;

   //if balance is over threshold 
   if (abs(balance) > THRESHOLD){
      //declare a local variable for occupancy. because these operations
      //should not affect tree occupancy
      long occupancy = 0; //temp variable

      TNode<Whatever> tempNode(data);

      //remove this current node from the tree
      Remove(*this, fio, occupancy, PositionInParent, 1);

      TNode<Whatever> tempNode2(PositionInParent, fio);

      //reinsert this back into the tree
      tempNode2.Insert(tempNode.data, fio, occupancy, PositionInParent);
   }
   else{
      //updates the file with the new heights and balances
      Write(fio);
   }
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: GetCost
Purpose:               Returns cost.
Description:           Returns the number of reads and writes we've done.
Input:                 None.
Result:                long - value of cost
-----------------------------------------------------------------------*/
template <class Whatever>
long Tree <Whatever> :: GetCost () {
   return cost;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: GetOperation
Purpose:               Returns operation.
Description:           Returns the number of operations we've done.
Input:                 None.
Result:                long - value of operation
-----------------------------------------------------------------------*/
template <class Whatever>
long Tree <Whatever> :: GetOperation () {
   return operation;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: IncrementCost
Purpose:               Increments cost.
Description:           Increments cost. Should be called whenever a read
                       or write occurs.
Input:                 None.
Result:                void - no return
-----------------------------------------------------------------------*/
template <class Whatever>
void Tree <Whatever> :: IncrementCost () {
   cost++;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: IncrementOperation
Purpose:               Increments operation.
Description:           Increments operation. Should be called whenever Tree's
                       Insert, Remove, or Lookup is called.
Input:                 None.
Result:                void - no return
-----------------------------------------------------------------------*/
template <class Whatever>
void Tree <Whatever> :: IncrementOperation () {
   operation++;
}


/*-----------------------------------------------------------------------
Function Name:         Tree<Whatever> :: ResetRoot
Purpose:               Resets root to point to the end of the file.
Description:           Moves the pointer to the end of the file and sets
                       root to be the position at the end of the file.
Input:                 None.
Result:                void - no return
-----------------------------------------------------------------------*/
template <class Whatever>
void Tree <Whatever> :: ResetRoot () {
   //sets root to be the end of the file	
   fio->seekg(0, ios::end);
   root = fio->tellg();
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: Insert
Purpose:               Inserts a node into the tree.
Description:           If the data is the same as the data in the current
                       node, it's a duplicate insert and we want to replace
		       the old data with the new data. Otherwise, if the 
		       data is greater or less than the current node's data,
		       we check the current node's right or left child 
		       respectively and insert the data if the child is null.
		       Otherwise, we call Insert recursively.
Input:                 element - the data we want to insert into the tree
	               fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
		       occupancy - the occupancy of the tree
		       PositionInParent - a reference to the TNode position 
		                    in the parent TNode used to get to the 
				    current TNode's offset in the datafile
Result:                long - 1 if the insert succeeded, 0 if it failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long TNode<Whatever> :: Insert (Whatever & element, fstream * fio,
      long & occupancy, offset & PositionInParent) {
   long inserted = 0; //flag for if insert succeeded or not
   
   //checks for duplicate insertion
   if(data == element) {
      //replaces old data with new data	   
      data = element;

      Write(fio);

      inserted = 1;
   } 

   //element is alphabetically less than current data, go left
   else if(data > element) {
      //insert a new node if the current node doesn't have a left child	   
      if(!left){
	 //write a new node holding element     
         TNode<Whatever> newNode (element, fio, occupancy);

	 //updates left offset to point to the new node's position
	 left = newNode.this_position;

	 //updates height and balance since we inserted a new node	 
	 SetHeightAndBalance(fio, PositionInParent);
	 
         inserted = 1;
      }

      //recursively call Insert if we have a left child
      else {
         TNode<Whatever> leftChild (left, fio);

	 inserted = leftChild.Insert(element, fio, occupancy, left);

	 //updates height and balance since we inserted a new node
	 SetHeightAndBalance(fio, PositionInParent);
      }
   }

   //element is alphabetically greater than current data, go right 
   else {
      //insert a new node if the current node doesn't have a right child	 
      if(!right)
      {
	 //write a new node holding element     
         TNode<Whatever> newNode (element, fio, occupancy);

	 //updates right offset to point to the new node's position
	 right = newNode.this_position;

	 //updates height and balance since we inserted a new node
	 SetHeightAndBalance(fio, PositionInParent);
	 
         inserted = 1;
      }

      //recursively call Insert if we have a right child
      else {
         TNode<Whatever> rightChild (right, fio);

	 inserted = rightChild.Insert(element, fio, occupancy, right);

	 //updates height and balance since we inserted a new node
	 SetHeightAndBalance(fio, PositionInParent);
      }
   }

   return inserted;
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: Lookup
Purpose:               Looks to see if an item is in the tree.
Description:           If the data is the same as the data in the current
                       node, we found what we're looking for! Otherwise,
		       check if the data is greater or less than the data
		       in the current node, check the right or left child
		       respectively. If there is no child, the item isn't
		       in the tree. Otherwise, recursively call Lookup
		       with the child.
Input:                 element - the data we want to look for in the tree
	               fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
Result:                long - 1 if the lookup succeeded, 0 if it failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long TNode<Whatever> :: Lookup (Whatever & element, fstream * fio) {
   long found = 0; //flag for if the lookup succeeded or not

   //end the recursion if we found what we're looking for
   if(data == element){
      element = data; //return the data to the user

      found = 1;
   }

   //element is alphabetically less than current data, go left
   else if (data > element) {
      //if there's no left child, the element isn't there	   
      if(!left) {
         return found;
      }

      //recursively call Lookup to check the left child & its children
      else {
         TNode<Whatever> leftChild (left, fio);
	 
	 found = leftChild.Lookup(element, fio);
      }
   }

   //element is alphabetically greater than current data, go right
   else {
      //if there's no right child, the element isn't there	   
      if(!right) {
         return found;
      }

      //recursively call Lookup to check the right child & its children
      else {
         TNode<Whatever> rightChild (right, fio);
	 
	 found = rightChild.Lookup(element, fio);
      }
   }

   return found;
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: Lookup
Purpose:               Looks to see if an item is in the tree.
Description:           If the tree is empty, do nothing. Otherwise, delegate
                       to the root node's Lookup.
Input:                 element - the data we want to look for in the tree
Result:                long - 1 if the lookup succeeded, 0 if it failed
-----------------------------------------------------------------------*/
template <class Whatever>
unsigned long Tree<Whatever> :: Lookup (Whatever & element) const {
   long found = 0; //flag for if lookup succeeded or not

   //element isn't there if there are no nodes in the tree
   if(occupancy == 0) {
      //do nothing
   }

   else {
      //reads in the root node
      TNode<Whatever> readRootNode (root, fio);

      found = readRootNode.Lookup(element, fio);
   }

   //increments operation because a tree operation occurred
   IncrementOperation();

   return found;
}


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: Read
Purpose:               Reads in a TNode from a specified position in a file.
Description:           Sets the read pointer to the position we want to 
                       start reading from and reads in the data from the
		       file. Increments cost because a read occured.
Input:                 position - where in the file we want to start reading
                       fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
Result:                void - no return
-----------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: Read (const offset & position, fstream * fio) {
   //reads the TNode at the specified position
   fio->seekg(position, ios::beg);
   fio->read((char *)(this), sizeof(TNode<Whatever>)); 

   //increments cost because a read occured
   Tree<Whatever>::IncrementCost();

   //debug
   if(Tree<Whatever>::debug_on){
      cerr <<  COST_READ << (const char*)this->data << "]\n";
   }
}


/*-----------------------------------------------------------------------
Function Name:         TNode's Read Constructor
Purpose:               Creates a new TNode by reading it in from a specified 
                       position in a file.
Description:           Delegates all work to the Read() function.
Input:                 position - where in the file we want to start reading
                       fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
-----------------------------------------------------------------------*/
template <class Whatever>
TNode<Whatever> :: TNode (const offset & position, fstream * fio) {
   //delegates work to Read() to read the TNode from the file
   Read(position, fio);
}


/*-----------------------------------------------------------------------
Function Name:         TNode's Write Constructor
Purpose:               Creates a new TNode, initializes its values and writes
                       it to the file.
Description:           Initializes all data fields and delegates all work to
                       the Write() function to actually write to the file.
Input:                 element - the data going into the node
                       fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
		       occupancy - the occupancy of the tree
-----------------------------------------------------------------------*/
template <class Whatever>
TNode<Whatever> :: TNode (Whatever & element, fstream * fio, long & occupancy): 
   data (element), height (0), balance (0), left (0), right (0) {
      //increments the Tree's occupancy field
      occupancy++;

      //sets this_position to be the current end of the file
      fio->seekp(0, ios::end);
      this_position = fio->tellp();

      //delegates work to Write() to write the TNode to the file
      Write(fio);
   }


/*-----------------------------------------------------------------------
Function Name:         TNode<Whatever> :: Write
Purpose:               Writes a TNode to its position in a file.
Description:           Sets the write pointer to be where we want to write
                       the TNode to and then write the data to the file. 
		       Increment cost because a write occurred.
Input:                 fio - the filestream corresponding to the datafile 
		             where the Tree is stored on disk
Result:                void - no return
-----------------------------------------------------------------------*/
template <class Whatever>
void TNode<Whatever> :: Write (fstream * fio) const {
   //writes the TNode to the end of the file
   fio->seekp(this_position, ios::beg);
   fio->write((const char *)(this), sizeof(TNode<Whatever>));

   //increments cost because a write occured
   Tree<Whatever>::IncrementCost();

   //debug
   if(Tree<Whatever>::debug_on)
   {
      cerr <<COST_WRITE << (const char*)data << "]\n";
   }   
}


/*-----------------------------------------------------------------------
Function Name:         Tree's Constructor
Purpose:               Creates a new Tree and a new fstream object.
Description:           Allocates memory for a new fstream object and 
                       initializes all the data fields that need to be 
		       initialized in the tree. If the file we're reading
		       from is empty, we want to reserve space to write out 
		       root and occupancy. Otherwise, read in root and 
		       occupancy from the file.
Input:                 datafile - the data file where all the info will be
                                  stored
-----------------------------------------------------------------------*/
template <class Whatever>
Tree<Whatever> :: Tree (const char * datafile) :
   fio (new fstream (datafile, ios :: out | ios :: in)){

   static long counter;	//static variable to track number of trees
   tree_count = ++counter; //initialize tree_count
   occupancy = 0; //initialize occupancy
   root = 0; //initialize root

   //debug	   
   if(debug_on)
   {
      cerr << TREE << tree_count << ALLOCATE;
   }

   //gets the position of the beginning of the file
   fio->seekg(0, ios::beg);
   offset beginning = fio->tellg();

   //gets the position of the end of the file
   fio->seekg(0, ios::end);
   offset ending = fio->tellg();

   //file is empty, reserve space for root and occupancy
   if(beginning == ending){
      //move the write pointer to the beginning and write out 
      //root and occupancy
      fio->seekp(0, ios::beg);
      fio->write((const char *)(&root), sizeof(root));
      fio->write((const char *)(&occupancy), sizeof(occupancy));
      root = fio->tellp();
   }

   //file not empty, read in values
   else{
      //move the read pointer to the beginning and read in values of
      //root and occupancy	   
      fio->seekg(0, ios::beg);
      fio->read((char *)(&root), sizeof(root));
      fio->read((char *)(&occupancy), sizeof(occupancy));
   }
}

template <class Whatever>
Tree<Whatever> :: ~Tree (void)
/***************************************************************************
     % Routine Name : Tree :: ~Tree  (public)
     % File :         Tree.c
     % 
     % Description :  deallocates memory associated with the Tree.  It
     %                will also delete all the memory of the elements within
     %                the table.
***************************************************************************/

{
   if(debug_on)
   {
      cerr << TREE << tree_count << DEALLOCATE;
   }

   //writes to the file the values of root and occupancy of the tree right 
   //before deallocating	
   fio->seekp(0, ios::beg);
   fio->write((const char *)(&root), sizeof(root));
   fio->write((const char *)(&occupancy), sizeof(occupancy));

   delete fio;
}	/* end: ~Tree */


template <class Whatever>
ostream & operator << (ostream & stream, const TNode<Whatever> & nnn) {
   stream << "at height:  :" << nnn.height << " with balance:  "
      << nnn.balance << "  ";
   return stream << nnn.data << "\n";
}


template <class Whatever>
ostream & Tree<Whatever> :: Write (ostream & stream) const
/***************************************************************************
  % Routine Name : Tree :: Write (public)
  % File :         Tree.c
  % 
  % Description : This funtion will output the contents of the Tree table
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
   long old_cost = cost;

   stream << "Tree " << tree_count << ":\n"
      << "occupancy is " << occupancy << " elements.\n";

   fio->seekg (0, ios :: end);
   offset end = fio->tellg ();

   // check for new file
   if (root != end) {
      TNode<Whatever> readRootNode (root, fio);
      readRootNode.Write_AllTNodes (stream, fio);
   }

   // ignore cost when displaying nodes to users
   cost = old_cost;

   return stream;
}


template <class Whatever>
ostream & TNode<Whatever> ::
Write_AllTNodes (ostream & stream, fstream * fio) const {
   if (left) {
      TNode<Whatever> readLeftNode (left, fio);
      readLeftNode.Write_AllTNodes (stream, fio);
   }
   stream << *this;
   if (right) {
      TNode<Whatever> readRightNode (right, fio);
      readRightNode.Write_AllTNodes (stream, fio);
   }

   return stream;
}

