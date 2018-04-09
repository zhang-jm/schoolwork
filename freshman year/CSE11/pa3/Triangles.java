/* 
 * Name: Jamie Zhang
 * Login: cs11fmd 
 * Date: October 23, 2014
 * File: Triangles.java
 * Sources of Help: none 
 * 
 * This program draws four triangles in alternating orientations, using 
 * asterisks and spaces. Users are prompted to input the size of the triangles
 * they wish to have displayed. The program then takes the input and draws
 * triangles of that size or returns an error message if the input is too 
 * small.
 */

import java.util.Scanner;

/* 
 * Name: Triangles
 * Purpose: Takes user input and uses it to draw four triangles in alternating
 * orientations with size specified by the user. Returns an error message if
 * the inputted size is too small.
 */ 
public class Triangles
{
  
/* 
 * Name: main
 * Purpose: Executes the code to draw triangles. 
 * Parameters: none
 * Return: void
 */ 
  public static void main(String args[])
  {
    int counter1 = 0;
    int counter2 = 0;
    int counter3 = 0;
    int counter4 = 0;
    int counter5 = 0;
    int counter6 = 0;
    int counter7 = 0;
    int counter8 = 0;
    int row = 0;
    
    System.out.print("Enter the size of the triangles to display: ");

    Scanner input = new Scanner(System.in);
    int inputtedNumber = input.nextInt(); //records triangle size
    int inputtedNumber2 = inputtedNumber; //creates new variable equal to size
    
    //inputted triangle size cannot be smaller than 2
    if (inputtedNumber < 2)
    {
      System.out.println("Triangle size must be > 1; Try again.");
    }
    
    //if inputted size is 2 or larger
    else
    {
      //sets these counters equal to the size of the triangles	    
      counter2 = inputtedNumber;
      counter3 = inputtedNumber;   
      counter6 = inputtedNumber;   
      counter7 = inputtedNumber;
      
      //while 
      while (row < inputtedNumber)
      {
	//increments the value of row
	row++;
	//decrements inputtedNumber2 (original value equal to size of triangle)
	inputtedNumber2--; 
        
	//draws first triangle
	while (counter1 < row)
	{
	  System.out.print("*");
	  counter1++;
	}
        
	//draws a triangle (out of spaces) between triangles 1 & 2
	while (counter2 > 0 )
        {
          System.out.print(" ");
	  counter2--;
	}
	
	//draws second triangle 
	while (counter3 > 0)
	{
	  System.out.print("*");
	  counter3--;
	}
	
	//draws a triangle (out of spaces) between triangles 2 & 3
	while (counter4 < row)
	{
	  System.out.print(" ");
	  counter4++;
	}
	
	//draws a second triangle (out of spaces) between triangles 2 & 3
	while (counter5 + 1 < row)
	{
	  System.out.print(" ");
	  counter5++;
	}
	
	//draws third triangle
	while (counter6 > 0)
	{
	  System.out.print("*");
	  counter6--;
	}
	
	//draws a triangle (out of spaces) between triangles 3 & 4
	while (counter7 > 0)
	{
	  System.out.print(" ");
	  counter7--;
	}

	//draws the fourth/last triangle
	while (counter8 < row)
	{
	  System.out.print("*");
	  counter8++;
	}
        
	//moves to print on the next line if not the last row
	//if(row < inputtedNumber)
	//{
          System.out.print("\n");
	//}

	//resets counters
	counter1 = 0;
	counter2 = inputtedNumber2;
	counter3 = inputtedNumber2;
	counter4 = 0;
	counter5 = 0;
	counter6 = inputtedNumber2;
	counter7 = inputtedNumber2;
	counter8 = 0;

      }
    }
  }
} //end class Triangles
