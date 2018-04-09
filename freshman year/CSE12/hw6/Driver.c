/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 10, 2015
                                                        cs12xre
                                Assignment Six

File Name:      Driver.c
Description:    This file contains the code for a UCSDStudent object that 
                is inserted inside of the HashTable to test it. It also 
		contains the code for the Driver in order to run and test
		the HashTable.
****************************************************************************/

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <getopt.h>
#include <iostream>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

/***************************************************************************
% Class Name : UCSDStudent
% File :       Driver.c
% 
% Description :  An object used to test the HashTable.
***************************************************************************/
class UCSDStudent : public Base {
   char * name;
   long studentnum;
public:
   UCSDStudent (char * nm, long sn) : name (strdup (nm)), studentnum (sn) {}
   
   /**********************************************************************
   % Name: ~UCSDStudent
   % Description :  The destructor for the UCSDStudent object. Deallocates
   %                all memory having to do with the object.	
   % Paramenters: none
   **********************************************************************/
   ~UCSDStudent (void) 
   {
       free(name);
       name = 0;
   }

   /**********************************************************************
   % Name: operator char *
   % Description :  An accessor method that allows access to private data,
   %                specifically, the name of the UCSDStudent.	
   % Parameters: none
   % Return: char * - the name of the UCSDStudent
   **********************************************************************/
   operator char * ()
   {
      return name;
   }

   /**********************************************************************
   % Name: operator long
   % Description :  A method that gets the hashcode of the UCSDStudent.
   %                It does this by adding up the ASCII values of the name
   %                of the student.   
   % Parameters: none
   % Return: long - the ASCII value of the student's name
   **********************************************************************/   
   operator long () 
   {
      int index = 0; //the index of the character in the name to grab next
      int sumASCII = 0; //holds the sum of the characters
		
      //loops while the character isn't null
      while(name[index] != 0)
      {
         char ch = name[index]; //grabs the character
         sumASCII += ch; //adds the character to the sum
         index++;
      }

      return (long)sumASCII;
   }

   /**********************************************************************
   % Name: long operator ==
   % Description :  Compares this UCSDStudent to another object using
   %                strcmp to compare the name of the student to the name
   %                of the other object.
   % Parameters: base - the object to compare to
   % Return: long - 1 if the names match, 0 if they don't
   **********************************************************************/
   long operator == (Base & base)
   {
      return !strcmp(name, base);
   }

   ostream & Write (ostream & stream) {
      return stream << "name: " << name << " Studentnum: " << studentnum;
   }

};

/* You may have one or more functions defined here, too */

int main (int argc, char * const * argv) {
   SymTab ST (5);
   char buffer[128];
   char command;
   long number;
   char option;

   /* initialize debug variable */
   HashTable::Set_Debug(0);

   /* check command line options for debug display */
   while ((option = getopt (argc, argv, "x")) != EOF) {

      switch (option) {
         case 'x': HashTable::Set_Debug(1);
	  	   break;
      }
   } 

   ST.Write (cout << "Initial Symbol Table:\n");
   while (cin) {
      command = NULL;	// reset command each time in loop
      cout << "Please enter a command:  ((i)nsert, (l)ookup, (w)rite):  ";
      cin >> command;

      switch (command) {
         case 'i':
	    cout << "Please enter UCSD student name to insert:  ";
	    cin >> buffer;	// formatted input
            cout << "Please enter UCSD student number:  ";
	    cin >> number;

	    // create student and place in symbol table
	    if(!ST.Insert (new UCSDStudent (buffer, number))){
  	       ST.Write (cout << "\nFinal Symbol Table:\n");
	       exit (0);
	    }
	    break;

         case 'l': {
            Base * found;	// whether found or not

	    cout << "Please enter UCSD student name to lookup:  ";
	    cin >> buffer;	// formatted input

	    UCSDStudent stu (buffer, 0);
	    found = ST.Lookup (& stu);
			
	    if (found)
	       found->Write (cout << "Student found!!!\n") << "\n";
	    else
	       cout << "Student " << buffer << " not there!\n";
            }
  	    break;

         case 'w':
            ST.Write (cout << "The Symbol Table contains:\n");
      }
   }
   ST.Write (cout << "\nFinal Symbol Table:\n");
}

