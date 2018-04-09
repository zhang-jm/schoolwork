/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: December 11, 2014
* File: CSE11_Line.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs,
*                  Java API docs, PA10
*
* A class that defines a line that can be manipulated.
*/

import java.awt.*;
import objectdraw.*;

/*
* Name: CSE11_Line
* Purpose: Defines a line
*/
public class CSE11_Line extends Shape
{
  //endpoints of the line
  private Point start;
  private Point end;

  private static final String CSE11LINE = new String("CSE11_Line");
 
  /*
  * Name: CSE11_Line
  * Purpose: No-arg constructor, creates a default, empty line
  * Parameters: none
  */    
  public CSE11_Line() 
  {
    //calls other constructor to set circle info	  
    this(new Point(), new Point());
  }

  /*
  * Name: CSE11_Line
  * Purpose: Creates a line with two endpoints
  * Parameters: start - start point of the line
  *             end - end point of the line
  */  
  public CSE11_Line( Point start, Point end ) 
  {
    //sets info of the circle	  
    super(CSE11LINE);

    this.setStart(start);
    this.setEnd(end);    
  }
 
  /*
  * Name: CSE11_Line
  * Purpose: Copy constructor; creates a new line based on the 
  * end points of an original line
  * Parameters: c - the line to make a copy of
  */       
  public CSE11_Line( CSE11_Line line ) 
  {
    this(new Point(line.getStart()), new Point(line.getEnd()));
  }

  /*
  * Name: getStart
  * Purpose: Returns the start point of the line
  * Parameters: none
  * Return: Point
  */ 
  public Point getStart()
  {
    return this.start;
  }

  /*
  * Name: getEnd
  * Purpose: Returns the end point of the line
  * Parameters: none
  * Return: Point
  */   
  public Point getEnd()
  {
    return this.end;
  }
 
  /*
  * Name: setStart
  * Purpose: Sets the start point of the line
  * Parameters: start - the point that will be set to the start point
  * Return: void
  */  
  private void setStart(Point start)
  {
    this.start = new Point(start);
  }

  /*
  * Name: setEnd
  * Purpose: Sets the end point of the line
  * Parameters: end - the point that will be set to the end point
  * Return: void
  */    
  private void setEnd(Point end)
  {
    this.end = new Point(end);
  }

  /*
  * Name: move
  * Purpose: Moves the line
  * Parameters: xDelta - the amount to move the line in the x direction
  *             yDelta - the amount to move the line in the y direction
  * Return: void
  */     
  public void move( int xDelta, int yDelta )
  {
    this.getStart().move(xDelta, yDelta);
    this.getEnd().move(xDelta, yDelta);
  }

  /*
  * Name: toString
  * Purpose: Returns the name of the line and the location of its 
  * two end points in a string
  * Parameters: none
  * Return: String
  */   
  @Override
  public String toString() 
  {
    //example return statement - 
    //"CSE11_Line: Point: (0, 0) to Point: (1, 1)"
    return super.getName() + ": " + start.toString() + " to " +
	   end.toString();
  }

  /*
  * Name: equals
  * Purpose: Checks if a line and another object are equal 
  * (same end points)
  * Parameters: o - an object to check a line to
  * Return: boolean
  */   
  @Override
  public boolean equals( Object o ) 
  {
    if(o == null)
    {
      return false; //false if o is null
    }
    else
    {
      if(o instanceof CSE11_Line) //checks if o is CSE11_Line
      {
	//checks if the endpoints of the two objects are the same
        if(((CSE11_Line)o).getStart().equals(this.getStart()) && 
	   ((CSE11_Line)o).getEnd().equals(this.getEnd()))
	{
          return true;
	}
        else
	{
          return false; //false if endpoints not the same
	}
      }
      else
      {
        return false; //false if o is not CSE11_Line
      }
    }
  }
 
  /*
  * Name: hashCode
  * Purpose: Returns an int representing the polygon
  * Parameters: none
  * Return: int
  */  
  @Override
  public int hashCode()
  {
    return this.toString().hashCode(); //uses String's hashCode method
  }
 
  /*
  * Name: draw
  * Purpose: Draws the line on the canvas
  * Parameters: canvas - canvas to draw the line on
  *             c - color to draw the line
  *             fill - if the line should be filled or not
  * Return: void
  */    
  public void draw( DrawingCanvas canvas, Color c, boolean fill ) 
  {
    //draws the line
    Line line = new Line(this.getStart().getX(), this.getStart().getY(), 
		    this.getEnd().getX(), this.getEnd().getY(), canvas);

    if(c == null)
    {
      line.setColor(Color.BLACK); //sets the color to black if c is null
    }
    else
    {
      line.setColor(c); 
    }
  }

}

