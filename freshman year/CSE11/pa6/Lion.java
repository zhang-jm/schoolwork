/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: November 11, 2014
* File: Lion.java
* Sources of Help: PA6 instructions, objectdraw docs, Java API docs
* 
* This program contains all the attributes and behaviors of a Lion object.
* The constructor can be called to create a new Lion object and the methods
* can be called to manipulate the Lion object in various ways.
*/

import objectdraw.*;
import java.awt.*;
import java.awt.Color; 

/*
* Name: Lion
* Purpose: To define a Lion object with various attributes and behaviors.
*/
public class Lion implements AnimalCard
{
  private static final String LION_SPEECH = "Roar";
 
  private static final int PADDING = 2;

  VisibleImage image;
  FramedRect highlight;
  FramedRect highlight2;

  /*
  * Name: Lion
  * Purpose: Initializes all variables within a Lion object when it is 
  * created.
  */
  public Lion(Image image, Location origin, DrawingCanvas canvas)
  {
    Location origin2 = new Location(origin.getX() + 1, origin.getY() + 1);

    //creates the lion image and the highlight
    this.image = new VisibleImage(image, origin, canvas);
   
    highlight = new FramedRect(origin, this.image.getWidth() - 1, 
		                this.image.getHeight() - 1, canvas);
    highlight2 = new FramedRect(origin2, this.image.getWidth() - PADDING - 1, 
		     this.image.getHeight() - PADDING - 1, canvas);
    
    highlight.hide();
    highlight2.hide();
  }

  /*
  * Name: speak
  * Purpose: Returns the sound a lion makes.
  * Parameters: none
  * Return: String
  */
  public String speak()
  {
    return LION_SPEECH;
  }

  /*
  * Name: contains
  * Purpose: Checks if a point is within the Lion image.
  * Parameters: point - a point on the canvas
  * Return: boolean
  */
  public boolean contains(Location point)
  {
    return image.contains(point);
  }

  /*
  * Name: showHighlight
  * Purpose: Shows the highlighted box around the Lion image.
  * Parameters: color - the color of the box
  * Return: void
  */
  public void showHighlight(Color color)
  {
    highlight.setColor(color);
    highlight2.setColor(color);
    highlight.show();
    highlight2.show();
  }

  /*
  * Name: hideHighlight
  * Purpose: Hides the highlighted box around the Lion image.
  * Parameters: none
  * Return: void
  */
  public void hideHighlight()
  {
    highlight.hide();
    highlight2.hide();
  }

  /*
  * Name: getHighlightColor
  * Purpose: Gets the highlight color.
  * Parameters: none
  * Return: Color
  */
  public Color getHighlightColor()
  {
    return highlight.getColor();
  }

  /*
  * Name: show
  * Purpose: Shows the image.
  * Parameters: none
  * Return: void
  */
  public void show()
  {
    image.show();
  }

  /*
  * Name: hide
  * Purpose: Hides the image.
  * Parameters: none
  * Return: void
  */
  public void hide()
  {
    image.hide();
  }

  /*
  * Name: equals
  * Purpose: Checks if an object is of the Lion class.
  * Parameters: none
  * Return: boolean
  */
  public boolean equals(Object o)
  {
    if(o instanceof Lion)
    {
      return true;
    }
    else
    {
      return false;
    }
  }
}

