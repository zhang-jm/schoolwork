/****************************************************************************

                                                Jamie Zhang
						cs12xre
                                                Christopher Cabreros
					        cs12xjm
                                                CSE 12, W15
                                                March 12

                              Assignment 9

File Name:      Driver.h
Description:    Contains the constructors and methods for the UCSDStudent 
                object which is inserted into the Tree to test it.
****************************************************************************/
#ifndef DRIVER_H
#define DRIVER_H

#include <string.h>
#include <iostream>
#include <cstdlib>
using namespace std;

//=========================================================================
// class UCSDStudent
//
// Description: Represents a UCSDStudent. Has a name, student id, and a 
//              print function. Also overrides some operators
//
// Data Fields:
//     name (char): name of student
//     studentnum (long): id number of student
//
// Public functions:
//     operator const char * : returns name of student
//     operator == : checks if names of students are the same
//     operator > : checks if name of 1 student is before another student
//                  alphabetically
//
//=========================================================================
class UCSDStudent {
        friend ostream & operator << (ostream &, const UCSDStudent &);
        char name[8];
        long studentnum;
public:
/*---------------------------------------------------------------------------
Function Name:  UCSDStudent default constructor
Purpose:        Initializes name and studentnum.
Description:    Sets all the values in the array to 0 and studentnum to 0.
Input:          None.
---------------------------------------------------------------------------*/  
   UCSDStudent() : studentnum(0) { memset(name, 0, sizeof(name)); }

/*---------------------------------------------------------------------------
Function Name:  UCSDStudent constructor w/ params
Purpose:        Initializes name and studentnum.
Description:    Sets the name array to the name passed in and studentnum to 
                the number passed in.
Input:          newName - the name of the student to be copied into the name
                          array
                num - the student number of the student
---------------------------------------------------------------------------*/   
   UCSDStudent(char * newName, long num) : studentnum(num) {
        memset(name, 0, sizeof(name));
	strcpy(name, newName);
   }

/*---------------------------------------------------------------------------
Function Name:  operator const char *
Purpose:        Overrides the cast to const char *
Description:    Returns the name of the student
Input:          None.
Output:         Name of student
Result:         None.
Side Effects:   None.
---------------------------------------------------------------------------*/
   operator const char * (void) const{
      return (char *)name;
   }

/*---------------------------------------------------------------------------
Function Name:  operator ==
Purpose:        Checks if two students' names are the same
Description:    Returns whether the two names are equal to each other.
                Delegates to strcmp
Input:          Another UCSDStudent
Output:         1 if the same, 0 if not
Result:         None.
Side Effects:   None.
---------------------------------------------------------------------------*/
   long operator == (const UCSDStudent & bbb) const {
      return ! strcmp (name, bbb);
   }

/*---------------------------------------------------------------------------
Function Name:  operator >
Purpose:        Checks for the greater of the two students' names
Description:    Returns whether student's name is greater than the input's
                Delegates to strcmp
Input:          Another UCSDStudent
Output:         1 if greater, 0 if not
Result:         None.
Side Effects:   None.
---------------------------------------------------------------------------*/
   long operator > (const UCSDStudent & bbb) const {
      return (strcmp (name, bbb) > 0) ? 1 : 0;
   }
};

#endif
