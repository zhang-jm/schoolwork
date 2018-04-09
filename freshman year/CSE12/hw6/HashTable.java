/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 10, 2015
                                                        cs12xre
                                Assignment Six

File Name:      HashTable.java
Description:    This file contains the constructors and methods that 
                define a HashTable object. The constructor is called to 
		allocate and initialize a new HashTable object.	The methods 
		can be called to insert a new item into the table and look
		up an existing item in the table.
****************************************************************************/

/*
 * Class Name: HashTable
 * Description: Contains the functionality for a HashTable object that 
 * can store items.
 */
public class HashTable extends Base {

   private static int counter = 0;	// number of HashTables so far
   private static boolean debug;        // allocation of debug state

   // set in locate, last location checked in hash table 
   private int index = 0; 

   // set in insert/lookup, count of location in probe sequence
   private int count = 0;
        
   private int probeCount[];   // where we are in the probe sequence 
   private long occupancy;     // how many elements are in the Hash Table
   private int size;           // size of Hash Table
   private Base table[];       // the Hash Table itself ==> array of Base
   private int tableCount;     // which hash table it is

   // messages
   private static final String AND = " and ";
   private static final String DEBUG_ALLOCATE = " - Allocated]\n";
   private static final String DEBUG_LOCATE = " - Locate]\n";
   private static final String DEBUG_LOOKUP = " - Lookup]\n";
   private static final String BUMP = "[Bumping To Next Location...]\n";
   private static final String COMPARE = " - Comparing ";
   private static final String FOUND_SPOT = " - Found Empty Spot]\n";
   private static final String HASH = "[Hash Table ";
   private static final String HASH_VAL = "[Hash Value Is: ";
   private static final String INSERT = " - Inserting ";
   private static final String PROCESSING = "[Processing ";
   private static final String TRYING = "[Trying Index ";
        
   /**
    * This method turns debug messages on. 
    * If called, this sets the boolean value of the static variable debug to
    * true. This allows debug messages to be printed. 
    */
    public static void debugOn () 
    {
       debug = true;
    }

   /**
    * This method turns debug messages off. 
    * If called, this sets the boolean value of the static variable debug to
    * false. This means that no debug messages will be printed. 
    */
    public static void debugOff () 
    {
       debug = false;
    }

  /**
   * This constructor allocates and initializes the memory
   * associated with a hash table.
   *
   * @param  sz   The number of elements for the table... MUST BE PRIME!
   */
   public HashTable (int sz) 
   {
      //allocates new arrays; automatically initialized	   
      probeCount = new int[sz];
      table = new Base[sz];

      //initializes the value of HashTable variables
      occupancy = 0;
      size = sz;
      tableCount = ++counter; //increments number of HashTables

      //debug message
      if(debug)
      {
         System.err.print(HASH + tableCount + DEBUG_ALLOCATE);
      }
   }


   /**
    * This method will insert the element in the hash table.
    * If the element cannot be inserted, false will be returned.
    * If the element can be inserted, the element is inserted
    * and true is returned.  Duplicate insertions will
    * cause the existing element to be deleted, and the duplicate
    * element to take its place.
    *
    * @param   element       The element to insert.
    * @param   initialCount  Where to start in probe seq (recursive calls) 
    * @return  T/F value indicating success or failure of insertion
    */
    public boolean insert (Base element, int initialCount) 
    {
       //debug messages	    
       if(debug)
       {
          System.out.println(HASH + this.tableCount + INSERT + 
				 element.getName() + "]");
       }

       count = initialCount; //sets the value of count to use inside locate      
       locate(element); //calls locate to find the index of where to insert

       //if the spot Locate returns is not empty and the elements don't have 
       //the same name
       if(table[index] != null && !element.equals(table[index]))
       {
	  //give an error if the table is full     
          if(occupancy == size)
	  {
	     return false;
	  }
          
	  //a reference to the element inside the table
	  Base toInsert = table[index];  
	  //current probe count of the element in the table
	  int pCount = probeCount[index];

          //puts the new element inside the table
	  table[index] = element;
          probeCount[index] = count;
 
          //debug message
 	  if(debug)
	  {
             System.err.print(BUMP);     
	  }

	  //recursively calls insert passing in the element that was kicked 
	  //out of its spot
          insert(toInsert, pCount + 1);
       }

       //if the spot Locate returns is empty or the two elements have the 
       //same name       
       else if(table[index] == null || element.equals(table[index]))
       {    
          //increases occupancy only if the spot is empty	 	       
	  if(!element.equals(table[index]))
	  {
	     occupancy++;
	  } 

          //puts the new element into the table
	  table[index] = element;
          probeCount[index] = count;

	  return true;
	}

       return true;
   }


   /**
    * This method will locate the location in the
    * table for the insert or lookup.
    *
    * @param   element  The element needing a location.
    * @return  true if item found, or false if not found
    */
    private boolean locate (Base element) 
    {
       //holds the number of times spots have been searched		    
       int numSpotsChecked = 0;
       int indexToCheck; //holds the next index to be checked
       int increment; //holds the increment value for the particular element
       //the probe count so we don't have to check indexes already checked
       int toCatchUp = count;

       //debug messages
       if(debug)
       {
          System.err.print(HASH + this.tableCount + DEBUG_LOCATE);
          System.err.println(PROCESSING + element.getName() + "]");
          System.err.println(HASH_VAL + element.hashCode() + "]");
       }

       //the first index to be checked
       indexToCheck = element.hashCode() % this.size;
       //size of the increment 
       increment = (element.hashCode() % (this.size - 1)) + 1;

       //loops to go through the probe sequence if the object was bumped and
       //we don't need to check all the earlier indexes in the sequence
       while(toCatchUp > 1)
       {
          indexToCheck = (indexToCheck + increment) % this.size;
	  toCatchUp--;
       }

       //loops while we haven't checked all the spots in the array yet       
       while(numSpotsChecked < this.size)
       {
          //debug message	       
          if(debug)
	  {
	     System.err.println(TRYING + indexToCheck + "]");
	  }   

          numSpotsChecked++; //increments the number of times we've searched

	  //if the spot in the table at the index is empty
   	  if(table[indexToCheck] == null)
          {
	     //debug message     		  
             if(debug)
             {
	        System.err.print(HASH + this.tableCount + FOUND_SPOT);
             }

	      //grabs that index for use by insert/lookup
	      index = indexToCheck;
	      return false;
	  }
        
	  //debug message
	  if(debug)
	  {
             System.err.println(HASH + this.tableCount + COMPARE +
	                        element.getName() + AND + 
	                        table[indexToCheck].getName() + "]");
          }
      
	  //if the object in the array has searched less than this element
	  if(probeCount[indexToCheck] < count)
	  {
	     //grabs that index for use by insert/lookup		  
	     index = indexToCheck;
	     return false;
	  }
	     
	  //if the objects in the array have the same name		      
	  else if(table[indexToCheck].equals(element))
	  {
	     //grabs that index for use by insert/lookup		  
             index = indexToCheck;
	     return true;
	  }
	  
	  //adds on to the indexToCount to search next spot
          else
	  {
             indexToCheck = (indexToCheck + increment) % this.size;
	     count++; //increments count for use by insert
	  }
       }

       return false;
    }


   /**
    * This method will lookup the element in the hash table.  If
    * found a pointer to the element is returned.  If the element
    * is not found, NULL will be returned to the user.
    *
    * @param   element  The element to look up.
    * @return  A pointer to the element if found, else NULL
    */
    public Base lookup (Base element) 
    {	       
       //debug message
       if(debug)
       {
          System.err.print(HASH + tableCount + DEBUG_LOOKUP);
       }

       this.count = 1; //resets count
       
       //calls locate to search for the element
       boolean found = locate(element);

       //if locate returns true, return the element
       if(found)
       {
          return table[index];
       }
       //if locate returns false, return null
       else
       {
          return null;
       }
    }


   /**
    * Creates a string representation of the hash table. The method 
    * traverses the entire table, adding elements one by one ordered
    * according to their index in the table. 
    *
    * @return  String representation of hash table
    */
    public String toString () {
       String string = "Hash Table " + tableCount + ":\n";
       string += "size is " + size + " elements, "; 
       string += "occupancy is " + occupancy + " elements.\n";

       /* go through all table elements */
       for (int index = 0; index < size; index++) {

          if (table[index] != null) {
             string += "at index " + index + ": ";
             string += "" + table[index];
             string += " with probeCount: "; 
             string += probeCount[index] + "\n";
          }
       }

       return string;
    }
}
