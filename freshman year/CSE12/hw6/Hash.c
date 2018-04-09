/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 10, 2015
                                                        cs12xre
                                Assignment Six

File Name:      Hash.c
Description:    This file contains the constructors and methods that 
                define a HashTable. The constructor is called to 
		allocate and initialize a new HashTable. The methods 
		can be called to insert a new item into the table and look
		up an existing item in the table. The destructor is called
		to deallocate memory of the HashTable.
****************************************************************************/

#include <cstdlib>
#include <string>
#include "Hash.h"

using namespace std;

//debug messages
static const char AND[] = " and ";
static const char DEBUG_ALLOCATE[] = " - Allocated]\n";
static const char DEBUG_DEALLOCATE[] = " has been deallocated]\n";
static const char DEBUG_LOCATE[] = " - Locate]\n";
static const char DEBUG_LOOKUP[] = " - Lookup]\n";
static const char BUMP[] = "[Bumping To Next Location...]\n";
static const char COMPARE[] = " - Comparing ";
static const char FOUND_SPOT[] = " - Found Empty Spot]\n";
static const char HASH[] = "[Hash Table ";
static const char HASH_VAL[] = "[Hash value is: ";
static const char INSERT[] = " - Inserting ";
static const char PROCESSING[] = "[Processing ";
static const char TRYING[] = "[Trying index ";

bool HashTable :: debug = 0; //debug status
int HashTable :: counter = 0; //number of HashTables

void HashTable :: Set_Debug (int option)

/***************************************************************************
% Routine Name : HashTable :: Set_Debug (public)
% File :         hash.c
% 
% Description :  This function sets debug mode on or off
%
% Parameter descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% option             true should set debug mode on, false should set debug
%                    mode off.
***************************************************************************/

{
   if(option)
   {
      debug = 1;    // turns debug on if option is true
   }	   

   else
   {
      debug = 0; //turns debug off if option is false
   }
}


HashTable :: HashTable (int sz) : size (sz),
	table_count(++counter), occupancy (0), table (new Base *[sz]), 
	probeCount (new int[sz])
/***************************************************************************
% Routine Name : HashTable :: HashTable (public)
% File :         hash.c
% 
% Description :  This function allocates and initializes the memory
%                associated with a hash table.
%
% Parameter descriptions :
% 
% name               description
% ------------------ ------------------------------------------------------
% sz               The number of elements for the table...MUST BE PRIME!!!
***************************************************************************/

{
   //debug message	
   if(debug)
   {
      cerr << HASH << table_count << DEBUG_ALLOCATE;
   }

   //initializes the values within the two arrays; rest of variables
   //initalized with default values
   for(int i = 0; i < sz; i++)
   {
      table[i] = 0;	   
      probeCount[i] = 0;
   }
}	/* end: HashTable */


HashTable :: ~HashTable (void)
/***************************************************************************
% Routine Name : HashTable :: ~HashTable  (public)
% File :         hash.c
% 
% Description :  deallocates memory associated with the Hash Table.  It
%                will also delete all the memory of the elements within
%                the table.
***************************************************************************/

{    
   //debug message	
   if(debug)
   {
      cerr << HASH << table_count << DEBUG_DEALLOCATE;
   }   

   //loops through the contents of the hash table and deallocates all the
   //elements within the table
   for(int i = 0; i < size; i++)
   { 
      if(table[i] != 0)
      {	      
         delete table[i];
         table[i] = 0;
      }
   }
     
   delete[] table; //deletes the array holding the elements itself
   delete[] probeCount; //deletes the array holding the probe count

   //sets the pointers to 0
   table = 0; 
   probeCount = 0;
   
   //decrements number of tables
   counter--;

}	/* end: ~HashTable */


bool HashTable :: Insert (Base * element, int initialCount)
/***************************************************************************
% Routine Name : HashTable :: Insert (public)
% File :         hash.c
% 
% Description : This function will insert the element in the hash table.
%		If the element cannot be inserted, false will be returned.
%		If the element can be inserted, the element is inserted
%		and true is returned.  Duplicate insertions will
%		cause the existing element to be deleted, and the duplicate
%		element to take its place.
%
% Parameters descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert.
% initialCount       Where to start in probe seq
% <return>           true or false indicating success or failure of insertion
***************************************************************************/

{
   //debug messages	
   if(debug)
   {
      cerr << HASH << table_count << INSERT << (char*)(*element)<< "]\n";
   }

   count = initialCount; //sets the value of count to use inside Locate	      
   Locate(element); //calls Locate to find the index of where to insert

   //if the spot Locate returns is not empty and the elements don't have the
   //same name
   if(table[index] != 0 && ((*element == *table[index]) == 0))
   {
      //give an error if the table is full	   
      if(occupancy == size)
      {
         return 0;
      }

      //gets a pointer to the element inside the table
      Base * toInsert = table[index];
      //current probe count of the element in the table
      int pCount = probeCount[index]; 

      //puts the new element inside the table
      table[index] = element;
      probeCount[index] = count;
 
      //debug message
      if(debug)
      {
         cerr << BUMP;     
      }

      //recursively calls Insert passing in the element that was kicked out
      //of its spot
      Insert(toInsert, pCount + 1);
   }

   //if the spot Locate returns is empty or the two elements have the same
   //name
   else if(table[index] == 0 || (*element == *table[index]))
   {    
      //increases occupancy only if the spot is empty	   
      if(table[index] == 0)
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


bool HashTable :: Locate (Base * element)
/**********************************************************************
% Routine Name : HashTable :: Locate (private)
% File :         HashTable.c
% 
% Description : This function will locate the location in the
%               table for the insert or lookup.
%
% Parameter descriptions :
%  
% name               description
% ------------------ -------------------------------------------------
% element            The element needing a location.
% <return>           true if item found, or false if not found
**********************************************************************/

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
      cerr << HASH << table_count << DEBUG_LOCATE;
      cerr << PROCESSING << (char*)(*element) << "]" << "\n";
      cerr << HASH_VAL << (long)(*element) << "]" << "\n";
   }

   indexToCheck = (long)(*element) % size; //the first index to be checked
   increment = ((long)(*element) % (size - 1)) + 1; //size of the increment

   //loops to go through the probe sequence if the object was bumped and
   //we don't need to check all the earlier indexes in the sequence
   while(toCatchUp > 1)
   {
      indexToCheck = (indexToCheck + increment) % size;
      toCatchUp--;
   }   

   //loops while we haven't checked all the spots in the array yet
   while(numSpotsChecked < size)
   {
      //debug message	   
      if(debug)
      {
         cerr << TRYING << indexToCheck << "]" << "\n";
      }   

      numSpotsChecked++; //increments the number of times we've searched

      //if the spot in the table at the index is empty
      if(table[indexToCheck] == 0)
      {
	 //debug message     
         if(debug)
         {
            cerr << HASH << table_count << FOUND_SPOT;
	 }

         index = indexToCheck; //grabs that index for use by Insert/Lookup
	 return 0;
      }

      //debug message
      if(debug)
      {
         cerr << HASH << table_count << COMPARE <<
                            (char*)(*element) << AND << 
	                    (char*)(*table[indexToCheck]) << "]" << "\n";
      }

      //if the object in the array has searched less than this element
      if(probeCount[indexToCheck] < count)
      {
         index = indexToCheck;  //grabs that index for use by Insert/Lookup
	 return 0;
      }
			  
      //if the objects in the array have the same name
      else if(*table[indexToCheck] == *element)
      {
         index = indexToCheck; //grabs that index for use by Insert/Lookup
	 return 1; //returns true; an object with the same name was found
      }

      //adds on to the indexToCount to search next spot
      else
      {
         indexToCheck = (indexToCheck + increment) % size;
	 count++; //increments count for use by Insert
      }
   }

   return 0;
}

Base * HashTable :: Lookup (Base * element)
/***************************************************************************
% Routine Name : HashTable :: Lookup (public)
% File :         hash.c
% 
% Description : This function will lookup the element in the hash table.  If
%               found a pointer to the element is returned.  If the element
%               is not found, NULL will be returned to the user.
%
% Parameter descriptions :
%  
% name               description
% ------------------ ------------------------------------------------------
% element            The element to insert or to lookup.
% <return>           A pointer to the element if found, else NULL
***************************************************************************/

{
   //debug message	
   if(debug)
   {
      cerr << HASH << table_count << DEBUG_LOCATE;
   }

   count = 1; //resets count
   bool found = Locate(element); //calls locate to search for the element

   //if locate returns true, return a pointer to the element
   if(found)
   {
      return table[index];
   }
   //if locate returns false, return NULL
   else
   {
      return 0;
   }
}


ostream & HashTable :: Write (ostream & stream)
/***************************************************************************
% Routine Name : HashTable :: Write (public)
% File :         hash.c
% 
% Description : This funtion will output the contents of the hash table
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
	stream << "Hash Table " << table_count << ":\n"
	       << "size is " << size << " elements, "
	       << "occupancy is " << occupancy << " elements.\n";

	/* go through all table elements */
	for (int index = 0; index < size; index++)
		if (table[index])
			table[index]->Write(stream << "at index "
			<< index << ": ") << " with probeCount: "
			<< probeCount[index] << "\n";
	return stream;
}	/* end: Write */
