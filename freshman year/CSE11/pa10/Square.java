/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: December 11, 2014
* File: Square.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs,
*                  Java API docs, PA10
*
* A class that defines a square that can be manipulated.
*/

import java.awt.*;
import objectdraw.*;

/*
* Name: Square
* Purpose: Defines a square
*/
public class Square extends ARectangle
{
  private int side; //side length of square
 
  private static final String SQUARE = new String("Square");
  private static final String SIDES = new String("Sides: ");

  /*
  * Name: Square
  * Purpose: No-arg constructor, creates a default, empty square
  * Parameters: none
  */    
  public Square() 
  {
    this(0, 0, 0);
  }
 
  /*
  * Name: Square
  * Purpose: Creates a square with a upper left x and y value, side length
  * Parameters: x - upper left x value
  *             y - upper left y value
  *             side - side length
  */    
  public Square( int x, int y, int side ) 
  {
    //sets info
    super(SQUARE, x, y);
    this.setSide(side);
  }

  /*
  * Name: Square
  * Purpose: Creates a square with a upper left corner point, side length
  * Parameters: upperLeft - upper left corner point
  *             side - side length
  */      
  public Square( Point upperLeft, int side ) 
  { 
    //sets info	  
    super(SQUARE, upperLeft);
    this.setSide(side);
  }
 
  /*
  * Name: Square
  * Purpose: Copy constructor; creates a new square based on the 
  * upper left corner, side length
  * Parameters: r - the square to make a copy of
  */   
  public Square( Square r ) 
  {
    this(new Point(r.getUpperLeft()), r.getSide());
  }

  /*
  * Name: getSide
  * Purpose: Returns the side length of the square
  * Parameters: none
  * Return: int
  */     
  public int getSide()
  {
    return this.side;
  }

  /*
  * Name: getPoint
  * Purpose: Returns the upper left corner point of the square
  * Parameters: none
  * Return: Point
  */  
  public Point getUpperLeft()
  {
    return super.getUpperLeft();
  }

  /*
  * Name: setSide
  * Purpose: Sets the side length of the square
  * Parameters: side - the value that will be set to the side length
  * Return: void
  */     
  private void setSide(int side)
  {
    this.side = side;
  }
 
  /*
  * Name: toString
  * Purpose: Returns the name of the square, upper left corner point,
  * and the side length
  * Parameters: none
  * Return: String
  */    
  @Override
  public String toString() 
  { 
    //example return statement - 
    //"Square: Upper Left Corner: Point: (0, 0) Sides: 30"
    return super.toString() + SIDES + this.getSide();
  }
 
  /*
  * Name: equals
  * Purpose: Checks if a Square and another object are equal 
  * (same name, upper left corner point, side length)
  * Parameters: o - an object to check a square to
  * Return: boolean
  */    
  @Override
  public boolean equals( Object o ) 
  { 
    if(super.equals(o)) //checks name and upper left corner point
    {
      //checks if the side lengths are the same
      if(((Square)o).getSide() == this.getSide())
      {
        return true;
      }
      else
      {
        return false; //false if the side lengths aren't the same
      }
    }
    else
    {
      return false; //false if the name or upper left corner isn't the same
    }
  }
 
  /*
  * Name: draw
  * Purpose: Draws the square on the canvas
  * Parameters: canvas - canvas to draw the square on
  *             c - color to draw the square
  *             fill - if the square should be filled or not
  * Return: void
  */    
  public void draw( DrawingCanvas canvas, Color c, boolean fill ) 
  {
    if(c == null)
    {
      c = Color.BLACK; //sets color to black if c is null
    }

    if(fill) //if filled
    {
      //draws a filled square
      FilledRect square = new FilledRect(super.getUpperLeft().getX(), 
		          super.getUpperLeft().getY(), this.getSide(), 
			  this.getSide(), canvas);
      square.setColor(c);
    }
    else
    {
      //draws a not filled square
      FramedRect square = new FramedRect(super.getUpperLeft().getX(), 
		          super.getUpperLeft().getY(), this.getSide(), 
			  this.getSide(), canvas);     
      square.setColor(c);
    }     
  }
 
}
