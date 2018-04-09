/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        March 2, 2015
                                                        cs12xre
                                Assignment Eight

File Name:      Driver.h
Description:    This file contains the constructor and overloaded operators
                for the UCSDStudent object that will be inserted into the
		Tree.
****************************************************************************/

#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

/*-----------------------------------------------------------------------
Name:                  UCSDStudent
Description:           An generic object to be inserted into the tree.
Data Fields:           name - the name of the student
                       studentnum - the student number
-----------------------------------------------------------------------*/
class UCSDStudent {
   friend ostream & operator << (ostream &, const UCSDStudent &);
   char name[20];
   long studentnum;
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
   name(), studentnum(val) 
   {
      strcpy(name, nm); //copies the value of nm into name
   } 

   /*-----------------------------------------------------------------------
   Function Name:         operator const char *
   Purpose:               Gets the name of the UCSDStudent.
   Description:           Returns the name of the UCSDStudent.
   Input:                 void - no input
   Result:                const char * - the name of the UCSDStudent
   -----------------------------------------------------------------------*/
   operator char * (void) const
   {
      return (char*)name;
   }	 


   /*-----------------------------------------------------------------------
   Function Name:         operator ==
   Purpose:               Checks to see if two UCSDStudents are 
                          have the same name.
   Description:           Uses strcmp to see if the names of two UCSDStudents
                          are the same.
   Input:                 stu - the UCSDStudent we're comparing to
   Result:                long - 1 if equal, 0 if not equal
   -----------------------------------------------------------------------*/ 
   long operator == (const UCSDStudent & stu) const
   {
      return !strcmp(name, stu);
   }  


   /*-----------------------------------------------------------------------
   Function Name:         operator >
   Purpose:               Checks if the name of this UCSDStudent is greater in
                          the alphabet than the name of another UCSDStudent.
   Description:           Uses strcmp to check the relation between two
                          UCSDStudent's names.
   Input:                 stu - the UCSDStudent we're comparing to
   Result:                long - 1 if greater than, 0 if less than/equal
   -----------------------------------------------------------------------*/    
   long operator > (const UCSDStudent & stu) const
   {
      return (strcmp(name, stu) > 0) ? 1 : 0;
   }
};

#endif
