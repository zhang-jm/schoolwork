/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: November 20, 2014
* File: ReverseRecurse.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs, 
*                  Java API docs
* 
* This program makes an array from user input and reverses it using recursion.
*/

import java.util.Scanner;

/*
* Name: ReverseRecurse
* Purpose: Makes an array from user input and reverses it using recursion.
*/
public class ReverseRecurse 
{
  //minimum length of array in reverse method 2
  private static final int MIN_ARRAY_LENGTH = 3;
  //used to shrink array length
  private static final int SHRINKING_ARRAY_LENGTH = 2;

  int size; 
  int count = 0;
  int[] array;
  int[] array2;

  /*
  * Name: initArray
  * Purpose: Creates an array from user input
  * Parameters: none
  * Return: int[]
  */
  public int[] initArray() 
  {
    System.out.print(PA7Strings.MAX_NUM); //prints instructions

    //new scanner
    Scanner in = new Scanner(System.in);

    //if the input is an int
    if (in.hasNextInt())
    {
      //sets size to the inputted value
      size = in.nextInt();

      //if size is smaller or equal to zero
      while (size <= 0)
      {
	//the input is too small, lets user try again
	System.out.println(PA7Strings.TOO_SMALL);
	size = in.nextInt();
      }
    
      //prints second set of instructions
      System.out.printf(PA7Strings.ENTER_INTS, size);

      //creates a new array with length of originally inputted size
      array2 = new int[size];

      //while integers are being inputted & number of inputs is smaller than
      //original size
      while (count < array2.length && in.hasNextInt())
      {
	//adds those integers into an array
	array2[count] = in.nextInt();
	count++;
      }

      //creates a new array of length number of inputs
      array = new int[count];
      //copies inputs over to new array
      System.arraycopy(array2, 0, array, 0, count);

      return array; //returns new array  
    }
 
    else
    {
      return null;
    }
  }
   
  /*
  * Name: printArray
  * Purpose: Prints the array
  * Parameters: array - an array that needs to be printed
  * Return: void
  */
  public void printArray( int[] array ) 
  { 
    //if the array is null or no length
    if(array == null || array.length == 0)
    {
      System.out.println(PA7Strings.EMPTY); //prints "empty array"
    }
    else
    {
      //for each index in the array
      for (int i = 0; i < array.length; i++)
      {
	//print the value
        System.out.print(array[i] + " ");
      }
      //prints a new line
      System.out.println("");
    }
  } 

  /*
  * Name: reverse
  * Purpose: Reverses an array
  * Parameters: originalArray - an array that needs to be reversed
  *             low - the left endpoint
  *             high - the right endpoint
  * Return: void
  */
  public void reverse( int[] originalArray, int low, int high ) 
  {
    //if the array is not null
    if(originalArray != null)
    {	 
      //if the left endpoint is still to the left of the right endpoint	    
      if(low < high)
      {
	//switches the values in the two endpoints
        int i = originalArray[low];
        int j = originalArray[high];
        originalArray[low] = j;
        originalArray[high] = i;

	//calls the method to run with the next indexes
        reverse(originalArray, low + 1, high - 1);
      }
      //if the middle
      else if(low == high)
      {
        //do nothing
      }
      //if there are no more endpoints that need switching
      else if(low > high)
      {
        //do nothing
      }
    }
  }
  
  /*
  * Name: reverse
  * Purpose: Reverses an array
  * Parameters: originalArray - an array that needs to be reversed
  * Return: int[]
  */  
  public int[] reverse( int[] originalArray ) 
  {
    //if the array is not null
    if (originalArray != null)
    {
      //gets the length of the original array
      int length = originalArray.length;
      //creates a new array of the length of the original array
      int[] newArray = new int[originalArray.length];

      //if the length of the original array is big enough
      if(originalArray.length >= MIN_ARRAY_LENGTH)
      {
	//creates a new array smaller than the original array
        int[] copyArray = new int[originalArray.length - 
		                  SHRINKING_ARRAY_LENGTH];
      
	//switches the values in the endpoints
        int i = originalArray[0];
        int j = originalArray[originalArray.length - 1];
        newArray[0] = j;
        newArray[originalArray.length - 1] = i;

	//makes a copy of the original array without the two endpoints
        System.arraycopy(originalArray, 1, copyArray, 0,
	  	         originalArray.length - SHRINKING_ARRAY_LENGTH);
      
	//does recursion to reverse endpoints of the smaller array
        System.arraycopy(reverse(copyArray), 0, newArray, 
  		         1, copyArray.length);
      }
      //if the original array has two numbers
      else if (originalArray.length == SHRINKING_ARRAY_LENGTH)
      {
	//switches those two numbers
        int i = originalArray[0];
        int j = originalArray[originalArray.length - 1];
        newArray[0] = j;
        newArray[originalArray.length - 1] = i;
      }
      //if the original array length only has one number
      else if (originalArray.length == 1)
      {
        newArray[0] = originalArray[0]; //returns that one number
      }
      return newArray;
    } 
    else
    {
      return null; //returns null if the original array was null
    }
  }
} //end class ReverseRecurse
