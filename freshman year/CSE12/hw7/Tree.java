/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 15, 2015
                                                        cs12xre
                                Assignment Seven

File Name:      Tree.java
Description:    This file contains the constructor and methods that define 
                a Tree and the Nodes that go into the Tree. The constructor 
		is called to allocate and initialize a new Tree or a new Node. 
		The Tree methods can be called to insert a new item 
	        into the Tree, look up an existing item in the Tree, 
		remove an item from the Tree, or write out the Tree. The 
		Node methods can be used to write out the Node.
****************************************************************************/

/*************************************************************************** 
 * Class Name: Tree
 * Description: A tree containing nodes that holds data. The first/topmost
 *              node is the root node and the nodes branch out from there
 *              based on the value of the data they hold.
 * Data Fields: TNode root - a pointer to the root node of the tree
 *              long occupancy - holds the number of items in the tree
 *              String treeName - the name of the tree
 * Public Methods: debugOff - turns debug messages off
 *                 debugOn - turns debug messages on
 *                 getName - returns the tree's name
 *                 insert - inserts a node and some data into the tree
 *                 lookup - looks to see if some data is in the tree
 *                 remove - removes data from the tree if it's there and 
 *                          hasn't already been removed
 *                 toString - creates a string representation of the tree
 ***************************************************************************/
public class Tree<Whatever extends Base> extends Base 
{
   //data fields
   private TNode root;
   private long occupancy; 
   private String treeName;

   //debug flag
   private static boolean debug;

   //debug messages
   private static final String ALLOCATE = " - Allocating]\n";
   private static final String AND = " and ";
   private static final String COMPARE = " - Comparing ";
   private static final String INSERT = " - Inserting ";
   private static final String TREE = "[Tree ";


   /**
   * Constructor. 
   * Allocates and initializes the elements of the tree.
   *
   * @param    name    the name of the tree 
   */ 
   public Tree (String name) 
   {
      //initializes the value of the name field
      this.treeName = name;

      //debug
      if(debug)
      {
         System.err.print(TREE + treeName + ALLOCATE);
      }
   }


   /**
   * Sets the debug boolean value to false.
   * This means that no debug messages will be printed.
   */   
   public static void debugOff () 
   {
      debug = false;
   }


   /**
   * Sets the debug boolean value to true.
   * This means that debug messages will be printed.
   */    
   public static void debugOn () 
   {
      debug = true;
   }


   /**
   * Returns the tree's name
   *
   * @return name of the tree
   */
   public String getName() 
   {
      return treeName;
   }


   /**
   * Inserts a node containing some data into the tree at the location it
   * belongs. Uses a while loop to continue searching down the tree, going
   * to the left or to the right depending on the value of the data compared
   * to the rest of the tree, until a spot it can be inserted into is found.
   * Then, the height and balance of the affected nodes are edited.
   *
   * @param   element   the data to be inserted into the tree
   * @return  boolean - T/F depending if the insert was successful or not
   */   
   public boolean insert (Whatever element) 
   {
      //the node we're currently looking at; set to root at the beginning
      TNode working = root;
      //flag checking if the item was successfully inserted
      boolean hasBeenInserted = false;

      //checks if the root node exists and inserts the first node/data as the
      //root if it doesn't
      if(working == null)
      {
         working = new TNode(element);
	 root = working; //sets the root pointer to the new node
	 occupancy++;

	 hasBeenInserted = true;
      }

      //checks if the item has been inserted yet
      while(!hasBeenInserted)
      {
	 //debug     
	 if(debug)
	 {
            System.err.println(TREE + treeName + COMPARE + element.getName() + 
			       AND + working.data.getName() + "]");
	 }	 

	 //checks if it's a duplicate insertion
         if(element.getName().equals(working.data.getName()))
	 {
	    //updates the data field of the node with the new data
            working.data = element;
	    hasBeenInserted = true;

	    //the element is not deleted anymore if it was before
	    working.hasBeenDeleted = false;
	 }

	 //checks if we should go to the right
	 else if(element.isGreaterThan(working.data))
	 {
            //if the right child of the current node is null, insert there 
	    if(working.right == null)
	    {
	       //creates a new node and updates or sets all necessary fields	    
               working.right = new TNode(element);
	       working.right.parent = working;
	       occupancy++;

	       hasBeenInserted = true;
	    }

	    //updates the pointer to the node we're working with
	    working = working.right;
	 }

	 //checks if we should go to the left
	 else if(!element.isGreaterThan(working.data))
	 {
            //if the left child of the current node is null, insert there 	
	    if(working.left == null)
	    {
	       //creates a new node and updates or sets all necessary fields
	       working.left = new TNode(element);
	       working.left.parent = working;
	       occupancy++;

	       hasBeenInserted = true;
	    }

	    //updates the pointer to the node we're working with
	    working = working.left;
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
      while(working != null && hasBeenInserted)
      {
         //holds the height of the right child of the working node	      
         long rightHeight = -1;
         //holds the height of the left child of the working node
	 long leftHeight = -1;

	 //if there is a right child, get its height
	 if(working.right != null)
	 {
            rightHeight = working.right.height;
	 }
	 
	 //if there is a left child, get its height
	 if(working.left != null)
	 {
           leftHeight = working.left.height;
	 }

	 //if both the right child and the left child were null, the working
	 //node is a leaf and the height is 0
	 if(rightHeight == -1 && leftHeight == -1)
	 {
            working.height = 0;
	 }

	 //updates the height of the node to be the height of the tallest 
	 //child + 1
	 else if(rightHeight >= leftHeight)
	 {
            working.height = rightHeight + 1;
	 }
	 else if(leftHeight > rightHeight)
	 {
            working.height = leftHeight + 1;
	 }

	 //updates the balance of the node to be the left child's height -
	 //the right child's height
	 working.balance = leftHeight - rightHeight;

	 //updates the pointer to go up the tree
	 working = working.parent;	 
      }

      //debug message
      if(debug)
      {
         System.err.println(TREE + treeName + INSERT + element.getName() + 
			    "]");
      }      

      return hasBeenInserted;
   }


   /**
   * Looks to see if a specific data is in the tree. Goes down the tree
   * depending on the value of the element we're looking for. If we find
   * a node containing data with the same name as the element we're looking
   * for, it's a successful search. Else, the element isn't there.
   *
   * @param   element   the data we're looking for
   * @return  Whatever - returns the data if we found it, null if not found
   */     
   public Whatever lookup (Whatever element) 
   {
      //the node we're currently looking at; set to root at beginning	   
      TNode working = root; 

      //loops until we reach the bottom of the tree
      while(working != null)
      {
         //debug  
         if(debug)
	 {
            System.err.println(TREE + treeName + COMPARE + element.getName() + 
			       AND + working.data.getName() + "]");
	 }

	 //if the name of the data in the current node is equal to the name
	 //of the data, we've found it
         if(working.data.getName().equals(element.getName()))
	 {
	    //only returns the data if the node hasn't been removed
            if(!working.hasBeenDeleted)	
	    {	 
               return working.data;
	    }
	    else
	    {
               return null;
	    }
	 }

	 //keep going if we haven't found it yet
         else if(element.isGreaterThan(working.data))	 
	 {
	    //go to the right if element is greater alphabetically	 
            working = working.right;
	 }
	 else
	 {
            //go to the left if element is not greater alphabetically	
	    working = working.left;
	 }
      }
      
      return null;
   }


   /**
   * Removes a specific data if it's in the tree. Goes down the tree
   * depending on the value of the element we're looking for. If we find
   * a node containing data with the same name as the element we're looking
   * for, we remove it. Else, the element isn't there.
   *
   * @param   element   the data we're trying to remove
   * @return  Whatever - returns the data if we successfully remove it, 
   *                     null if not found/couldn't remove
   */     
   public Whatever remove (Whatever element) 
   {
      //the node we're currently looking at; set to root at the beginning	   
      TNode working = root;

      //loops until we reach the bottom of the tree      
      while(working != null)
      {
	 //debug     
	 if(debug)
	 {
            System.err.println(TREE + treeName + COMPARE + element.getName() + 
			       AND + working.data.getName() + "]");
	 }

	 //if the name of the data in the current node is equal to the name
	 //of the data, we've found the node we want to remove	 
	 if(working.data.getName().equals(element.getName()))
	 {
	    //only "removes" the node if it hasn't been removed yet 	 
            if(!working.hasBeenDeleted)	
	    {
	       //updates the needed fields	    
	       working.hasBeenDeleted = true; 
               occupancy--;
                 	       
               return working.data;
	    }
	    else
	    {
               return null;
	    }
	 }

	 //keep going if we haven't found it yet	 
         else if(element.isGreaterThan(working.data))	 
	 {
            //go to the left if element is greater alphabetically		 
            working = working.right;
	 }
	 else
	 {
            //go to the left if element is not greater alphabetically		 
            working = working.left; //go left
	 }
      }
      
      return null;     
   }

   /**
   * Creates a string representation of this tree. This method first
   * adds the general information of this tree, then calls the
   * recursive TNode function to add all nodes to the return string 
   *
   * @return  String representation of this tree 
   */
   public String toString () 
   {
      String string = "Tree " + treeName + ":\noccupancy is ";
      string += occupancy + " elements.";

      if(root != null)
         string += root.writeAllTNodes();

      return string;
   }


   /*********************************************************************** 
   * Class Name: TNode
   * Description: Implements the node of the tree data structure. Each node
   *              contains two children, the left child which contains data
   *              "less than" the data of the current node, and the right 
   *              child which contains data "greater than" the data of the 
   *              current node.
   * Data Fields: Whatever data - the data the Node contains
   *              TNode left - pointer to the left child of the node
   *              TNode right - pointer to the right child of the node
   *              TNode parent - pointer to the parent of the node
   *              boolean hasBeenDeleted - flag for if the node was removed
   *              long balance - the balance of the node
   *              long height - the height of the node
   * Public Methods: toString - creates a string representation of the node
   *                 writeAllNodes - writes all the nodes in the tree
   ************************************************************************/   
   private class TNode 
   {
      public Whatever data;
      public TNode left, right, parent;
      public boolean hasBeenDeleted;

      //left child's height - right child's height
      public long balance;
      //1 + height of tallest child, or 0 for leaf
      public long height;

  
      /**
      * Constructor. 
      * Allocates and initializes the elements of the TNode
      *
      * @param    element   the data going into the tree
      */       
      public TNode (Whatever element) 
      {
         //initializes the TNode's data
         this.data = element;
      }

      /**
      * Creates a string representation of this node. Information
      * to be printed includes this node's height, its balance,
      * and the data its storing.
      *
      * @return  String representation of this node 
      */
      @Override
      public String toString () 
      {
         return "at height:  " + height + "  with balance:  " + balance + 
		 "  " + data;
      }

      /**
      * Writes all TNodes to the String representation field. 
      * This recursive method performs an in-order
      * traversal of the entire tree to print all nodes in
      * sorted order, as determined by the keys stored in each
      * node. To print itself, the current node will append to
      * tree's String field.
      */
      public String writeAllTNodes () 
      {
         String string = "";
         if (left != null)
            string += left.writeAllTNodes ();
         if (!hasBeenDeleted) 
            string += "\n" + this;          
         if (right != null)
            string += right.writeAllTNodes ();

         return string;
      }
   }
}
