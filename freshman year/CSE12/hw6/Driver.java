/****************************************************************************

                                                        Jamie Zhang
                                                        CSE12, WI15
                                                        Feb 10, 2015
                                                        cs12xre
                                Assignment Six

File Name:      Driver.java
Description:    This file contains the code for a UCSDStudent object that 
                is inserted inside of the HashTable to test it. It also 
		contains the code for the Driver in order to run and test
		the HashTable.
****************************************************************************/

import java.io.EOFException;

/*
 * Class Name : UCSDStudent
 * Description :  An object used to test the HashTable.
 */
class UCSDStudent extends Base {
   private String name;
   private long Studentnum;

   /*
    * Name : UCSDStudent
    * Description :  A constructor initializes the name and student number of
    *                the object.
    * Parameters: name - the name to be inserted into the name variable
    *             num - the number to be inserted into Studentnum
    */   
   public UCSDStudent(String name, long num)
   {
      this.setName(name); //initializes name
      this.setStudentNum(num); //initializes Studentnum
   }

   /*
    * Name : UCSDStudent
    * Description :  A copy constructor initializing the name and student 
    *                number of the new object to the same values as the
    *                original object.
    * Parameters: student - the UCSDStudent to make a copy of
    */ 
   public UCSDStudent(UCSDStudent student)
   {
      this.setName(student.getName()); //initializes name
      this.setStudentNum(student.getStudentNum()); //initializes Studentnum
   }

   @Override
   public String toString () 
   {
      return "name: " + name + " Studentnum: " + Studentnum;
   }

   /*
    * Name : hashCode
    * Description :  A method that gets the hashcode of the UCSDStudent.
    *                It does this by adding up the ASCII values of the name
    *                of the student.
    * Parameters: none
    * Return: int - the sum of the ASCII values of the name
    */    
   @Override
   public int hashCode() 
   {
      int index = 0; //the index of the character in the name to grab next
      int sumASCII = 0; //holds the sum of the characters
		
      //loops while the whole name hasn't been checked yet      
      while(index < this.name.length())
      {
         char ch = name.charAt(index); //grabs the character
         sumASCII += ch; //adds the character to the sum
         index++;
      }

      return sumASCII;
   }

   /*
    * Name : equals
    * Description :  Compares this student to another object by checking
    *                if their names are the same.
    * Parameters: o - the object to compare this student with
    * Return: boolean - true if the names match, false if they don't
    */      
    @Override
    public boolean equals(Object o)
    {
       if(o == null)
       {
          return false; //false if o is null
       }

       //checks if o is a UCSDStudent
       else if(o instanceof UCSDStudent)
       {
          if(this.getName().equals(((UCSDStudent)o).getName()))
          { 
  	     return true; //true if the names match
	  }
	  else
	  {
	     return false; //false if they don't match
	  }
        }

        else
	{
           return false; //false if o is not a UCSDStudent
	}
    }

    /*
    * Name : getName
    * Description :  Returns the name of the student
    * Parameters: none
    * Return: String - the name of the student
    */ 
    @Override
    public String getName()
    {
       return this.name;
    }

    /*
    * Name : getStudentNum
    * Description :  Returns the student number of the student
    * Parameters: none
    * Return: long - the student number
    */     
    public long getStudentNum()
    {
       return this.Studentnum;
    }

    /*
    * Name : setName
    * Description :  Sets the name of the student
    * Parameters: name - the name to set the name of the student to
    * Return: void - no return type
    */ 
    private void setName(String name)
    {
       this.name = new String(name);
    }

    /*
    * Name : setStudentNum
    * Description :  Sets the student number of the student
    * Parameters: num - the number to set the student number to
    * Return: void - no return type
    */     
    private void setStudentNum(long num)
    {
       this.Studentnum = num;
    }
}

public class Driver {

   public static void main (String [] args) {

      /* initialize debug states */
      HashTable.debugOff();

      /* check command line options */
      for (int index = 0; index < args.length; ++index) {
         if (args[index].equals("-x"))
             HashTable.debugOn();
      }

      /* The real start of the code */
      SymTab symtab = new SymTab (5);
      String buffer = null;
      char command;
      long number = 0;

      System.out.print ("Initial Symbol Table:\n" + symtab);

      while (true) {
         command = 0;    // reset command each time in loop
         System.out.print ("Please enter a command:  "
                            + "((i)nsert, (l)ookup, (w)rite):  ");

	 try {
            command = MyLib.getchar ();
            MyLib.clrbuf (command); // get rid of return

            switch (command) {
               case 'i':
                  System.out.print (
                         "Please enter UCSD Student name to insert:  ");
                  buffer = MyLib.getline (); // formatted input

                  System.out.print (
                          "Please enter UCSD Student number:  ");

                  number = MyLib.decin ();
                  MyLib.clrbuf (command); // get rid of return

                  // create Student and place in symbol table
                  if(!symtab.insert (new UCSDStudent (buffer, number), 1)){
                     System.out.println("Couldn't insert " + 
                                         "student!!!"); 
                   }
                   break;
	       case 'l': {
		   Base found;     // whether found or not

                   System.out.print (
                             "Please enter UCSD Student name to lookup:  ");

                   buffer = MyLib.getline ();// formatted input
 
		   UCSDStudent stu = new UCSDStudent (buffer, 0);
                   found = symtab.lookup (stu);
                                
                   if (found != null) {
                      System.out.println ("Student found!!!");
                      System.out.println (found);
                   }
                   else
                      System.out.println ("Student " + buffer
                                           + " not there!");
                   }
                   break;

                case 'w':
                   System.out.print (
                              "The Symbol Table contains:\n" + symtab);
             }
          }
          catch (EOFException eof) {
             break;
          }
       }
       System.out.print ("\nFinal Symbol Table:\n" + symtab);
   }
}
