/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: December 11, 2014
* File: Rectangle.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs,
*                  Java API docs, PA10
*
* A class that defines a rectangle that can be manipulated.
*/

import java.awt.*;
import objectdraw.*;

/*
* Name: Rectangle
* Purpose: Defines a rectangle
*/
public class Rectangle extends ARectangle
{
  private int width; //width and height of the rectangle
  private int height;

  private static final String RECTANGLE = new String("Rectangle");
  private static final String WIDTH = new String("Width: ");
  private static final String HEIGHT = new String(" Height: ");

  /*
  * Name: Rectangle
  * Purpose: No-arg constructor, creates a default, empty rectangle
  * Parameters: none
  */     
  public Rectangle() 
  {
    this(0, 0, 0, 0);	  
  } 

  /*
  * Name: Rectangle
  * Purpose: Creates a rectangle with a upper left x and y value, width 
  * and height
  * Parameters: x - upper left x value
  *             y - upper left y value
  *             width - width of the Rectangle
  *             height - height of the Rectangle
  */     
  public Rectangle(int x, int y, int width, int height) 
  {
    //sets info for the rectangle
    super(RECTANGLE, x, y);

    this.setWidth(width);
    this.setHeight(height);
  }

  /*
  * Name: Rectangle
  * Purpose: Creates a rectangle with a upper left point, width 
  * and height
  * Parameters: upperLeft - upper left point of the Rectangle
  *             width - width of the Rectangle
  *             height - height of the Rectangle
  */  
  public Rectangle(Point upperLeft, int width, int height) 
  {
    //sets info
    super(RECTANGLE, upperLeft);
   
    this.setWidth(width);
    this.setHeight(height);    
  }
 
  /*
  * Name: Rectangle
  * Purpose: Copy constructor; creates a new rectangle based on the 
  * upper left corner, width and height
  * Parameters: r - the rectangle to make a copy of
  */     
  public Rectangle(Rectangle r) 
  { 
    this(new Point(r.getUpperLeft()), r.getWidth(), r.getHeight());
  }

  /*
  * Name: getWidth
  * Purpose: Returns the width of the rectangle
  * Parameters: none
  * Return: int
  */    
  public int getWidth()
  { 
    return this.width;
  }

  /*
  * Name: getHeight
  * Purpose: Returns the height of the rectangle
  * Parameters: none
  * Return: int
  */      
  public int getHeight()
  {
    return this.height;
  }

  /*
  * Name: getUpperLeft
  * Purpose: Returns the upper left corner of the rectangle
  * Parameters: none
  * Return: Point
  */      
  public Point getUpperLeft()
  {
    return super.getUpperLeft(); //calls superclass's upper left
  }

  /*
  * Name: setWidth
  * Purpose: Sets the width of the rectangle
  * Parameters: width - the value that will be set to the width
  * Return: void
  */     
  private void setWidth(int width)
  {
    this.width = width;
  }

  /*
  * Name: setHeight
  * Purpose: Sets the height of the rectangle
  * Parameters: height - the value that will be set to the height
  * Return: void
  */    
  private void setHeight(int height)
  {
    this.height = height;
  }
 
  /*
  * Name: toString
  * Purpose: Returns the name of the rectangle, upper left corner point,
  * and the width and height
  * Parameters: none
  * Return: String
  */    
  @Override
  public String toString() 
  { 
    //example return statement - 
    //"Rectangle: Upper Left Corner: Point: (0, 0) Width: 1 Height: 1"
    return super.toString() + WIDTH + this.getWidth() + 
	    HEIGHT + this.getHeight();
  }

  /*
  * Name: equals
  * Purpose: Checks if a Rectangle and another object are equal 
  * (same upper left corner point, width and height)
  * Parameters: o - an object to check a rectangle to
  * Return: boolean
  */   
  @Override
  public boolean equals(Object o) 
  { 
    if(super.equals(o)) //checks if upper left corner point and name are same
    {
      //if the widths and heights are the same
      if(((Rectangle)o).getWidth() == this.getWidth() &&
	 ((Rectangle)o).getHeight() == this.getHeight())
      {
        return true;
      }
      else
      {
        return false; //false if widths or heights aren't same
      }
    }
    else
    {
      return false; //false if upper left corner and name aren't same
    }
  }
 
  /*
  * Name: draw
  * Purpose: Draws the rectangle on the canvas
  * Parameters: canvas - canvas to draw the rectangle on
  *             c - color to draw the rectangle
  *             fill - if the rectangle should be filled or not
  * Return: void
  */    
  public void draw( DrawingCanvas canvas, Color c, boolean fill )
  {
    if(c == null)
    {
      c = Color.BLACK; //color is black if c is null
    }

    if(fill) //if it should be filled
    {
      //draws a filled rectangle
      FilledRect rect = new FilledRect(super.getUpperLeft().getX(), 
		          super.getUpperLeft().getY(), this.getWidth(), 
			  this.getHeight(), canvas);
      rect.setColor(c);
    }
    else
    {
      //draws a not filled rectangle
      FramedRect rect = new FramedRect(super.getUpperLeft().getX(), 
		          super.getUpperLeft().getY(), this.getWidth(), 
			  this.getHeight(), canvas);     
      rect.setColor(c);
    }   	  
  }
 
}

