/****************************************************************************

                                                Jamie Zhang
						cs12xre
                                                Christopher Cabreros
					        cs12xjm
                                                CSE 12, W15
                                                March 12

                              Assignment 9

File Name:      Driver.c
Description:    Driver used to test the Tree. Allows users to insert into, 
                remove from, lookup an item in, and write out the tree,
		either from a file or from the keyboard.
****************************************************************************/


#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>
#include <getopt.h>
#include "Driver.h"
#include "SymTab.h"

using namespace std;

#ifdef NULL
#undef NULL
#define NULL 0
#endif

ostream & operator << (ostream & stream, const UCSDStudent & stu) {
   return stream << "name:  " << stu.name
      << " with studentnum:  " << stu.studentnum;
}

int main (int argc, char * const * argv) {
   char buffer[BUFSIZ];
   char command;
   long number;
   char option;

   istream *is = &cin;
   ostream *os = &cout;
   SymTab<UCSDStudent>::Set_Debug_Off ();

   while ((option = getopt (argc, argv, "x")) != EOF) {

      switch (option) {
         case 'x': SymTab<UCSDStudent>::Set_Debug_On ();
                   break;
      }       
   }

   SymTab<UCSDStudent> ST("Driver.datafile");
   ST.Write (cout << "Initial Symbol Table:\n" );

   while (cin) {
      command = NULL;         // reset command each time in loop
      *os << "Please enter a command ((f)ile, (i)nsert, "
         << "(l)ookup, (r)emove, (w)rite):  ";
      *is >> command;

      switch (command) {

         case 'i': {
                      *os << "Please enter UCSD student name to insert:  ";
                      *is >> buffer;  // formatted input

                      *os << "Please enter UCSD student number:  ";
                      *is >> number;

                      UCSDStudent stu (buffer, number);

                      // create student and place in symbol table
                      ST.Insert (stu);
                      break;
                   }
         case 'l': { 
                      unsigned long found;    // whether found or not

                      *os << "Please enter UCSD student name to lookup:  ";
                      *is >> buffer;  // formatted input

                      UCSDStudent stu (buffer, 0);
                      found = ST.Lookup (stu);

                      if (found)
                         cout << "Student found!!!\n" << stu << "\n";
                      else
                         cout << "student " << buffer << " not there!\n";
                      break;
                   }
         case 'r': { 
                      unsigned long removed;

                      *os << "Please enter UCSD student name to remove:  ";
                      *is >> buffer;  // formatted input

                      UCSDStudent stu (buffer, 0);
                      removed = ST.Remove(stu);

                      if (removed)
                         cout << "Student removed!!!\n" << stu << "\n";
                      else
                         cout << "student " << buffer << " not there!\n";
                      break;
                   }
         case 'w': {
                      ST.Write (cout << "The Symbol Table contains:\n");
		      break;
                   }
         case 'f': {
                      //read from keyboard
                      if (is == &cin){
                         *os << "Please enter file name for commands:  ";
                         *is >> buffer; //input

                         //read in the file
                         is = new ifstream(buffer);
                         os = new ofstream("/dev/null");
                      }

                      //read from file
                      else{
			 *is >> buffer; //input
                         delete is;
                         is = new ifstream(buffer);
                      }

                   }
      }
      if (is != &cin && !(*is)) {
         //delete is and set pointer back to cin
         delete is;
         is = &cin;
         delete os;
         os = &cout;
      }
   }

   ST.Write (cout << "\nFinal Symbol Table:\n");

   if (ST.GetOperation() != 0) {
      cout << "\nCost of operations: ";
      cout << ST.GetCost();
      cout << " tree accesses";

      cout << "\nNumber of operations: ";
      cout << ST.GetOperation();

      cout << "\nAverage cost: ";
      cout << (((float)(ST.GetCost()))/(ST.GetOperation()));
      cout << " tree accesses/operation\n";
   }

   else
      cout << "\nNo cost information available.\n";

}
