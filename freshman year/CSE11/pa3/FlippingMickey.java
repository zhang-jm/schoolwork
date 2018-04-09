/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: October 23, 2014
* File: FlippingMickey.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs
* 
* This program creates a Mickey within the Applet Viewer window when the 
* mouse is clicked, at the location where the mouse was clicked. After Mickey
* is created, if the mouse is pressed inside of the Mickey image, Mickey can
* be dragged around the window. If the mouse is pressed inside of the Mickey
* image for more than 0.5 seconds and then released, and Mickey is not dragged 
* within this time frame, Mickey will flip left, right, or upside down 
* depending on which part of Mickey was clicked.If the mouse exits the window,
* the Mickey image disappears.
*/
import objectdraw.*;

/*
* Name: FlippingMickey
* Purpose: Creates a Mickey image that can be manipulated within the 
* Applet Viewer window.
*/
public class FlippingMickey extends WindowController
{
  //location of instructions
  private static final int INSTR1_X = 50;
  private static final int INSTR1_Y = 50;
  private static final int INSTR2_X = INSTR1_X;
  private static final int INSTR2_Y = INSTR1_Y + 20;
  private static final int INSTR3_X = INSTR1_X;
  private static final int INSTR3_Y = INSTR2_Y + 20;

  //amount of time mouse has to be pressed to flip Mickey; half a second
  private static final double FLIP_PRESS_THRESHOLD = 0.5;

  private Text text1;
  private Text text2;
  private Text text3;
  private Location clickPoint; //point where the window is clicked
  private Location clickPoint2; //point where Mickey is pressed
  
  private Mickey mouse;
  private Timer aTimer;
  private double time; //records how much time has passed between presses
                       //and releases

  boolean isCreated = false; //checks if Mickey is created or not
  boolean isGrabbed = false; //checks if the mouse press is within the Mickey 
                             //image
  boolean isHidden = false; //checks if the text is hidden
  
  boolean inRightEar = false; //checks if mouse press is in right ear
  boolean inLeftEar = false; //checks if mouse press is in left ear
  boolean inHead = false; //checks if mouse press is in head
  boolean isUpsideDown = false; //checks if Mickey has been flipped upside down
  boolean isFlippedRight = false; //checks if Mickey has been flipped right
  boolean isFlippedLeft = false; //checks if Mickey has been flipped left
  boolean ifTimer = false; //checks if a timer object has been created
  boolean isDragged = false; //checks if Mickey has been dragged
  
  /*
  * Name: begin
  * Purpose: Sets up the Applet Viewer window and displays the instructions.
  * Parameters: none
  * Return: void
  */
  public void begin() 
  {
    text1 = new Text(
    "Click to display a Mickey silhouette centered at the mouse click.", 
    INSTR1_X, INSTR1_Y, canvas);
    text2 = new Text(
    "Mouse press in any part of the image and drag to move image around.", 
    INSTR2_X, INSTR2_Y, canvas);
    text3 = new Text(
    "Mouse click in any part of the image with a mouse press for more than " +
     "0.5 seconds to flip image.", INSTR3_X, INSTR3_Y, canvas);
  }
    
  /*
  * Name: onMouseClick
  * Purpose: Creates a Mickey image if one isn't already yet. The image is
  * created at the location the mouse is clicked.
  * Parameters: point - the point location where the mouse is clicked
  * Return: void
  */
  public void onMouseClick(Location point) 
  {
    //if there is no Mickey image currently drawn
    if(!isCreated) 
    {
      //hides the text
      text1.hide();
      text2.hide();
      text3.hide();

      clickPoint = point;
      mouse = new Mickey(clickPoint, canvas); //creates a new Mickey object
      
      isHidden = true; //text is hidden
    }

    isCreated = true; //Mickey is created
  }
  
  /*
  * Name: onMousePress
  * Purpose: Checks if Mickey is created and if the mouse press is within the
  * Mickey image. If so, this method lets the program know that Mickey is
  * being pressed on, the location where Mickey was pressed, and aTimer is 
  * initialized.
  * Parameters: point - the point location where the mouse is pressed
  * Return: void
  */
  public void onMousePress(Location point) 
  {
    if(isCreated) //if there is a Mickey image
    {
      isDragged = false;
      
      aTimer = new Timer(); //creates a new Timer object
      ifTimer = true; //there is a timer
      

      clickPoint2 = point;

      //if the right ear is being pressed
      if (mouse.inRightEar(clickPoint2))
      {
        inRightEar = true;
	isGrabbed = true; //mouse press is within Mickey
      }
      
      //if the left
      //ear is being pressed
      else if (mouse.inLeftEar(clickPoint2))
      {
        inLeftEar = true;
	isGrabbed = true; //mouse press is within Mickey
      }

      //if the head is being pressed
      else if (mouse.inHead(clickPoint2))
      {
        inHead = true;
	isGrabbed = true; //mouse press is within Mickey
      }
    }
  }

  /*
  * Name: onMouseRelease
  * Purpose: Lets the program know that Mickey is no longer being pressed
  * on and flips Mickey if Mickey had been pressed on for more than 0.5
  * seconds and not dragged. Mickey is flipped depending on which part of
  * Mickey was pressed.
  * Parameters: point - the point location where the mouse is released
  * Return: void
  */
  public void onMouseRelease(Location point) 
  {
    isGrabbed = false; //Mickey is not being pressed on
    
    //if a timer was created and Mickey was not dragged
    if (ifTimer && !isDragged)
    {
      //checks how much time has passed
      time = aTimer.elapsedSeconds();
      
      //if the time was greater than 0.5 sec
      if (time > 0.5)
      {
        //checks the current orientation of Mickey
	isUpsideDown = mouse.isUpsideDown();
	isFlippedRight = mouse.isFlippedRight();
	isFlippedLeft = mouse.isFlippedLeft();

	//if Mickey's head is pressed
        if(inHead && !isUpsideDown)
        {
          mouse.flipUpsideDown(); //flips Mickey upside down
        }
        else if (inHead && isUpsideDown)
        {
          mouse.flipRightsideUp(); //flips Mickey rightside up
        }

        //if Mickey's right ear (in respect to the head) is pressed
        else if (inRightEar && !isFlippedRight)
        {
          mouse.flipRight(); //flips Mickey right
	  isFlippedRight = true;
        }
	else if (inLeftEar && isFlippedLeft)
	{
	  mouse.flipRight2(); //flips Mickey right
	  isFlippedLeft = false;
	}
	else if (inLeftEar && isFlippedRight)
	{
          mouse.flipRight2(); //flips Mickey right
	  isFlippedRight = false;
	}

	//if Mickey's left ear (in respect to the head) is pressed
        else if (inLeftEar && !isFlippedLeft)
        {
          mouse.flipLeft(); //flips Mickey left
	  isFlippedLeft = true;
        }
	else if (inRightEar && isFlippedRight)
	{
          mouse.flipLeft2(); //flips Mickey left
	  isFlippedRight = false;
	}
	else if (inRightEar && isFlippedLeft)
	{
          mouse.flipLeft2(); //flips Mickey left
	  isFlippedLeft = false;
	}
      }
      
      //resets the values that check where Mickey was pressed
      inHead = false;
      inRightEar = false;
      inLeftEar = false;
    }
  }
 
  /*
  * Name: onMouseDrag
  * Purpose: Checks if a Mickey image has been created and if Mickey has
  * been pressed on. If so, this method allows Mickey to move after being
  * dragged. 
  * Parameters: point - the point location where the mouse is being dragged 
  * to
  * Return: void
  */
  public void onMouseDrag(Location point) 
  {
    //if there is a Mickey image on the canvas and it has been pressed on
    if(isCreated && isGrabbed)
    {
      //moves Mickey
      mouse.moveMickey(point, clickPoint2);
      clickPoint2 = point;

      isDragged = true; //Mickey has been dragged (will not flip)
    }

    //resets the values that check where Mickey was pressed
    inHead = false;
    inRightEar = false;
    inLeftEar = false;

  }

  /*
  * Name: onMouseExit
  * Purpose: Checks if a Mickey image has been created. If it has been
  * created, the Mickey image disappears as the mouse exits the window.
  * Parameters: point - the point location where the mouse exits
  * Return: void
  */
  public void onMouseExit(Location point)
  {
    if(isCreated) //if there is a Mickey image
    { 
      //hides the Mickey image
      mouse.hideMickey();
    
      isCreated = false; //there is no longer a Mickey image
    }
  }

  /*
  * Name: onMouseEnter
  * Purpose: If the instructions have been hidden, as the mouse enters 
  * the window, the instructions are re-shown.
  * Parameters: point - the point location where the mouse enters the window
  * Return: void
  */
  public void onMouseEnter(Location point)
  {
    if(isHidden == true) { //if the instructions are hidden
      text1.show(); //shows the instructions
      text2.show();    
      text3.show();
    }
  }

} //end class Mickey


