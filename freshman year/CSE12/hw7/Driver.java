/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 15, 2015
                                                        cs12xre
                                Assignment Seven

File Name:      Driver.java
Description:    This file contains the driver as well as code for a 
                UCSDStudent object. The driver is used to run and test the 
		code of the tree and the UCSDStudent is the test object that
		will go into the tree.
****************************************************************************/

import java.io.*;

/*************************************************************************** 
 * Class Name: UCSDStudent
 * Description: A UCSDStudent object used to test the tree. Has name
 *              and student number fields.
 * Data Fields: String name - the name of the student
 *              long studentnum - the student number of the student
 * Public Methods: getName - returns the name of the student
 *                 equals - checks if this UCSDStudent is equal to 
 *                          another object
 *                 isGreaterThan - checks if this UCSDStudent's name is 
 *                                 greater in the alphabet to another name
 *                 toString - returns a string representation of the object
 ***************************************************************************/
class UCSDStudent extends Base {

   private String name; //the name of the student
   private long studentnum; //the student number


   /**
   * Constructor. 
   * Allocates and initializes the elements of the UCSDStudent
   *
   * @param    nm    the name of the UCSDStudent
   * @param    num   the student number of the UCSDStudent
   */   
   public UCSDStudent(String nm, long num)
   {
      this.name = new String(nm);
      studentnum = num;
   }


   /**
   * Returns the name.
   *
   * @return name of the UCSDStudent
   */   
   public String getName()
   {
      return this.name;
   }

 
   /**
   * Checks for equality. Two items are equal if their names are the same.
   *
   * @return boolean - T/F depending on if the names are equal or not
   */   
   public boolean equals(Object o)
   {	   
      if(o == null || !(o instanceof UCSDStudent))
      {
         return false; //not equal if o is null or o isn't a UCSDStudent		      
      }
      else
      {
         if(this.getName().equals(((UCSDStudent)o).getName()))
	 {
            return true; //true if the names match
	 }
	 else
	 {
            return false;
	 }
      }
   }


   /**
   * Checks the relation of two names.
   *
   * @return boolean - T/F depending on the relation between the two names                 
   */      
   public boolean isGreaterThan(Base base)
   {
      if(name.compareTo(base.getName()) > 0)
      {
	 //true if this UCSDStudent's name is greater in the alphabet than 
	 //the other name     
         return true;
      }
      else
      {
         //false if this UCSDStudent's name is less in the alphabet than 
	 //the other name	      
         return false;
      }
   }


   /**
   * Creates a string representation of the UCSDStudent
   *
   * @return  String representation of the UCSDStudent
   */   
   public String toString () {
      return "name:  " + name + "  studentnum:  " + studentnum;
   }
}

public class Driver {
   private static final short NULL = 0;

   public static void main (String [] args) {
        
      /* initialize debug states */
      Tree.debugOff();

      /* check command line options */
      for (int index = 0; index < args.length; ++index) {
         if (args[index].equals("-x"))
            Tree.debugOn();
      }


      /* The real start of the code */
      SymTab<UCSDStudent> symtab = 
                          new SymTab<UCSDStudent>("UCSDStudentTree");
      String buffer = null;
      char command;
      long number = 0;

      System.out.println ("Initial Symbol Table:\n" + symtab);

      while (true) {
         command = NULL; // reset command each time in loop
         System.out.print ("Please enter a command:  " + 
                           "((a)llocate, (i)nsert, (l)ookup, " +
                           "(r)emove, (w)rite):  ");

         try {
            command = MyLib.getchar ();
            MyLib.clrbuf (command); // get rid of return

            switch (command) {
               case 'a':
                  System.out.print
                              ("Please enter name of new Tree to " +
                               "allocate:  ");
                                
                  buffer = MyLib.getline ();// formatted input
                  symtab = new SymTab<UCSDStudent>(buffer);
                  break;

               case 'i':
                  System.out.print
                         ("Please enter UCSD student name to insert:  ");

                  buffer = MyLib.getline ();// formatted input

                  System.out.print
                          ("Please enter UCSD student number:  ");

                  number = MyLib.decin ();
                  MyLib.clrbuf (command); // get rid of return

                  // create student and place in symbol table
                  symtab.insert(new UCSDStudent (buffer, number));
                  break;

               case 'l': { 
		  UCSDStudent found;      // whether found or not

                  System.out.print
                       ("Please enter UCSD student name to lookup:  ");
                  buffer = MyLib.getline ();// formatted input

                  UCSDStudent stu = new UCSDStudent (buffer, 0);
                  found = symtab.lookup (stu);
                                
                  if (found != null) {
                     System.out.println("Student found!");
                     System.out.println(found);
                  }
                  else
                     System.out.println ("student " + buffer
                                         + " not there!");
                  }
                  break;
                        
               case 'r': { 
                  UCSDStudent removed; // data to be removed

                  System.out.print
                     ("Please enter UCSD student name to remove:  ");

                  buffer = MyLib.getline ();

                  UCSDStudent stu = new UCSDStudent (buffer, 0);

                  removed = symtab.remove(stu);

                  if (removed != null) {
                     System.out.println("Student removed!"); 
                     System.out.println(removed);
                  }
                  else
                     System.out.println ("student " + buffer
                                         + " not there!");
                  }
                  break;

               case 'w':
                  System.out.println("The Symbol Table " +
                                     "contains:\n" + symtab);
            }
         }
         catch (EOFException eof) {
            break;
         }
      }
      System.out.println("\nFinal Symbol Table:\n" + symtab);
   }
}
