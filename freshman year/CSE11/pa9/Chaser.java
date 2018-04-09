/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: December 4, 2014
* File: Chaser.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs,
*                  Java API docs, PA9
*
* A critter that chases other critters (except other chasers).
*/

import objectdraw.*;
import java.awt.Color;

/*
* Name: Chaser
* Purpose: A critter that chases other critters (except other chasers).
*/
public class Chaser extends Critter
{
  private Location loc;
  private FilledOval newChaser;

  private Location[] locations;

  public Chaser(Location loc, DrawingCanvas canvas)
  {
    super(loc, canvas);
    
    this.loc = new Location(loc.getX() - super.HALF_SIZE, 
		            loc.getY() - super.HALF_SIZE);    

    if(this.loc.getX() + super.SIZE <= canvas.getWidth() &&
       this.loc.getX() >= 0 && 
       this.loc.getY() + super.SIZE <= canvas.getHeight() &&
       this.loc.getY() >= 0)
    {
      newChaser = new FilledOval(this.loc, super.SIZE, super.SIZE, canvas);
      newChaser.setColor(Color.MAGENTA);
    }
  }

  /*
  * Name: reactTo
  * Purpose: Reacts to other critters, chases
  * Parameters: other - another critter
  * Return: void
  */ 
  public void reactTo(Critter other)
  {
    if(other == null)
    {
      return;
    }
    else if(other instanceof Runner ||
	    other instanceof Random ||
	    other instanceof Imitator)
    {
      locations = super.getSurroundingLocations(1);

      Location closest = locations[0];;

      int i = 0;
      double distance = closest.distanceTo(other.getLocation());
      while(i < locations.length)
      {
	if(locations[i].distanceTo(other.getLocation()) < distance &&
           locations[i].getX() > 0 && 
	   locations[i].getX() < canvas.getWidth() && 
	   locations[i].getY() > 0 && 
	   locations[i].getY() < canvas.getHeight())
	{
          closest = locations[i];
	  distance = locations[i].distanceTo(other.getLocation());
	}

	i++;
      }

      newChaser.moveTo(closest);      
      super.setLocation(closest);
    }
  }

  /*
  * Name: kill
  * Purpose: removes critter from canvas
  * Parameters: none
  * Return: void
  */   
  public void kill()
  {
    newChaser.removeFromCanvas();    
  }
}
