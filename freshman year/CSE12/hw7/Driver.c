/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 15, 2015
                                                        cs12xre
                                Assignment Seven

File Name:      Driver.c
Description:    This file contains the driver as well as code for a 
                UCSDStudent object. The driver is used to run and test the 
		code of the tree and the UCSDStudent is the test object that
		will go into the tree.
****************************************************************************/

#include <cstdlib>
#include <iostream>
#include <string.h>
#include "Base.h"
#include "SymTab.h"
using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

class UCSDStudent : public Base {
   char * name; //name of the student
   long studentnum; //student number of the student
public:
   /*-----------------------------------------------------------------------
   Name:                  UCSDStudent
   Purpose:               Constructor for the UCSDStudent.
   Description:           Allocates and initializes the UCSDStudent's data 
                          fields.
   Input:                 nm - name of the student
                          val - student number of the student
   -----------------------------------------------------------------------*/
   UCSDStudent(char * nm, long val = 0):
      name(strdup(nm)), studentnum(val) {}


   /*-----------------------------------------------------------------------
   Name:                  UCSDStudent
   Purpose:               Copy constructor for the UCSDStudent.
   Description:           Allocates and initializes the UCSDStudent's data 
                          fields based on an already existing UCSDStudent.
			  Makes a deep copy.
   Input:                 stu - an already existing UCSDStudent to copy
   -----------------------------------------------------------------------*/   
   UCSDStudent(const UCSDStudent & stu)
   {
      //initializes data fields by making a deep copy of stu's data	   
      name = strdup(stu.name);
      studentnum = stu.studentnum;
   }


   /*-----------------------------------------------------------------------
   Name:                  ~UCSDStudent
   Purpose:               Destructor for the UCSDStudent.
   Description:           Frees up the memory allocated for the UCSDStudent's 
                          name.
   Input:                 none
   -----------------------------------------------------------------------*/    
   ~UCSDStudent(void)
   {
      free(name); //deallocates memory
   }


   /*-----------------------------------------------------------------------
   Function Name:         operator char *
   Purpose:               Gets the name of the UCSDStudent.
   Description:           Returns the name of the UCSDStudent.
   Input:                 void - no input
   Result:                char * - the name of the UCSDStudent
   -----------------------------------------------------------------------*/   
   operator char * (void) const
   {
      return name;
   }


   /*-----------------------------------------------------------------------
   Function Name:         operator ==
   Purpose:               Checks to see if two UCSDStudent objects are equal.
   Description:           Uses strcmp to see if the names of two UCSDStudents
                          are the same.
   Input:                 bbb - what we're comparing with
   Result:                long - 1 if equal, 0 if not equal
   -----------------------------------------------------------------------*/     
   long operator == (const Base & bbb) const
   {
      return !strcmp(name, bbb);
   }


   /*-----------------------------------------------------------------------
   Function Name:         operator >
   Purpose:               Checks if the name of this UCSDStudent is greater in
                          the alphabet than the name of another UCSDStudent.
   Description:           Uses strcmp to check the relation between two
                          UCSDStudent's names.
   Input:                 bbb - what we're comparing with
   Result:                long - 1 if greater than, 0 if less than
   -----------------------------------------------------------------------*/     
   long operator > (const Base & bbb) const
   {
      return (strcmp(name, bbb) > 0) ? 1 : 0;
   }


   ostream & Write (ostream & stream) const {
      return stream << "name:  " << name << "  studentnum:  " << studentnum; 
   }
};

int main (int argc, char * const * argv) {
   char buffer[80];
   char command;
   long number;

   Tree::Set_Debug(0);

   if (argc != 1 && strcmp("-x", argv[1]) == 0) {
      Tree::Set_Debug(1);
   }
	
   SymTab * ST;
   ST = new SymTab ("UCSDStudentTree");
   ST->Write (cout << "Initial Symbol Table:\n");

   while (cin) {
      command = NULL;		// reset command each time in loop
      cout <<  "Please enter a command:  ((a)llocate, (i)nsert, " <<
			"(l)ookup, (r)emove, (w)rite):  ";
      cin >> command;

      switch (command) {

         case 'a':
	    cout << "Please enter name of new Tree to allocate:  ";
	    cin >> buffer; 

	    delete ST;
	    ST = new SymTab (buffer);

	    break;

         case 'i':
	    cout << "Please enter UCSD student name to insert:  ";
	    cin >> buffer;	// formatted input

	    cout << "Please enter UCSD student number:  ";
	    cin >> number;

	    // create student and place in symbol table
	    ST->Insert (new UCSDStudent (buffer, number));
	    break;

	 case 'l': {
	    const Base * found;	// whether found or not

	    cout << "Please enter UCSD student name to lookup:  ";
	    cin >> buffer;	// formatted input

	    UCSDStudent stu (buffer, 0);
	    found = ST->Lookup (&stu);
			
	    if (found)
	       found->Write (cout << "Student found!\n") << "\n";
	    else
	       cout << "student " << buffer << " not there!\n";
	    }
	    break;
		
	 case 'r': {
	    Base * removed;	// data to be removed

	    cout << "Please enter UCSD student name to remove:  ";
	    cin >> buffer;	// formatted input

	    UCSDStudent stu (buffer, 0);
	    removed = ST->Remove (&stu);

	    if (removed)
	       removed->Write (cout << "Student removed!\n") << "\n";
	    else
	       cout << "student " << buffer << " not there!\n";
	    }
	    break;


	 case 'w':
	    ST->Write (cout << "The Symbol Table contains:\n");
      }
   }

   ST->Write (cout << "\nFinal Symbol Table:\n");
   delete ST;
}
