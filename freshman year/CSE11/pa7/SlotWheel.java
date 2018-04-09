/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: November 20, 2014
* File: SlotWheel.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs, 
*                  Java API docs
* 
* This program contains all the attributes and behaviors of a SlotWheel
* object. The constructor can be called to create a new SlotWheel object
* and the methods can be called to manipulate the SlotWheel in various ways.
*/

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import java.util.*;
import objectdraw.*;

/*
* Name: SlotWheel
* Purpose: To define a SlotWheel object with various attributes 
* and behaviors.
*/
public class SlotWheel extends ActiveObject
                       implements ActionListener
{
  private Image[] imageArray;
  private int ticks; //number of times the images change
  private int delay; //time between image changes
  private int xValue;
  private int yValue;
  private Location point; //location of the slot wheel
  private int wheelNumber;
  private Slots slots;
  private DrawingCanvas canvas;

  private static final double FIRST_VALUE = 0.25;
  private static final double SECOND_VALUE = 0.5;
  private static final double THIRD_VALUE = 0.75;
  private static final int FIRST_INDEX = 2;
  private static final int SECOND_INDEX = 4;
  private static final int THIRD_INDEX = 6;

  //number of different images in the array
  private static final int NUM_OF_IMAGES = 8;
  private static final double IMAGE_WIDTH = 110; 
  private static final double IMAGE_HEIGHT = 145; 
  //used to check if a half-image is being shown
  private static final int CHECK_IF_EVEN = 2;

  private VisibleImage image;
  //the image the slot wheel is showing
  private int imageNumber; 
  //the number of images left to be cycled through
  private int imageChangesLeft; 

  //checks if the button has been pressed/the slots are spinning
  private boolean hasBeenRun = false;

  /*
  * Name: SlotWheel
  * Purpose: Initializes all variables within the SlotWheel object when
  * it is created by taking in parameters.
  * Parameters: imageArray - the array of images used in the slot wheel
  *             ticks - the number of times the images change
  *             delay - the time between the image changes
  *             xValue - the x value of the left-hand corner of the wheel
  *             yValue - the y value of the left-hand corner of the wheel
  *             wheelNumber - the number that the wheel is (compared to other
  *                           wheel objects)
  *             slots - a Slots object
  *             canvas - where the SlotWheel will be placed
  */
  public SlotWheel(Image[] imageArray, int ticks, int delay, int xValue, 
		   int yValue, int wheelNumber, Slots slots, 
		   DrawingCanvas canvas)
  {
    //sets all the instance variables
    this.imageArray = imageArray;
    this.ticks = ticks;
    this.delay = delay;
    point = new Location(xValue, yValue);
    this.wheelNumber = wheelNumber;
    this.slots = slots;
    this.canvas = canvas;

    //gets the wheel index to get the first image to be shown
    imageNumber = getWheelIndex();
    
    //creates a new VisibleImage using the image from above
    image = new VisibleImage(imageArray[imageNumber], point, IMAGE_WIDTH, 
		             IMAGE_HEIGHT, canvas);


    start(); //calls the run method
  }

  /*
  * Name: run
  * Purpose: Runs the thread for the SlotWheel; changes the images, stops the
  * images from changing, etc.
  * Parameters: none
  * Return: void
  */
  public void run()
  {	  
    while (true) //runs forever
    {
      synchronized(this)
      {	    
	//if the images have not changed ticks number of times yet
        if(imageChangesLeft > 0)
        {
          imageChangesLeft--; //decrements the amount of image changes left

	  int nextImageNumber = imageNumber + 1; //increments the array index
	  //mods so index loops from 0 to number of images - 1
          imageNumber = (nextImageNumber) % NUM_OF_IMAGES; 
	  //changes the image
          image.setImage(imageArray[imageNumber]);
	  hasBeenRun = true; //the wheel has been spun
	}
	//if the images have been changed the specified number of times
	//and the last image is a half-image
	else if(imageChangesLeft == 0 && ((imageNumber % CHECK_IF_EVEN) == 1))
        {
          //changes the image to the next whole image
          imageNumber = imageNumber + 1;
          image.setImage(imageArray[(imageNumber + 1) % NUM_OF_IMAGES]);
        }
	//if there are no more image changes left and the wheel has been spun
	else if(imageChangesLeft == 0 && hasBeenRun)
	{
          //runs the method in slots to record which index the slot wheel 
	  //landed on
          slots.showWinner(wheelNumber, imageNumber);
	  hasBeenRun = false; //the wheel is not spinning anymore
	}    
      }
      this.pause(delay); //delay between image changes
    }
  }

  /*
  * Name: getWheelIndex
  * Purpose: Randomizes which whole image is shown first for the SlotWheel
  * Parameters: none
  * Return: int
  */
  public int getWheelIndex()
  {
    //creates a new random generator to output a random double
    Random randomGenerator = new Random();
    double value = randomGenerator.nextDouble();

    //checks the value and assigns it to an index used to get an image
    if(value >= 0 && value < FIRST_VALUE)
    {
       return 0; //gets first whole image
    }
    else if(value >= FIRST_VALUE && value < SECOND_VALUE)
    {
      return FIRST_INDEX; //gets second whole image
    }
    else if(value >= SECOND_VALUE && value < THIRD_VALUE)
    {
      return SECOND_INDEX; //gets third whole image
    }
    else
    {
      return THIRD_INDEX; //gets fourth whole image
    }
  }

  /*
  * Name: actionPerformed
  * Purpose: Runs this code when the JButton is pressed
  * Parameters: evt - the ActionEvent
  * Return: void
  */
  public void actionPerformed(ActionEvent evt)
  {
    synchronized(this)
    {
      imageNumber = getWheelIndex(); //randomizes the new starting image 
      //sets imageChangesLeft to start the slot wheel spinning
      imageChangesLeft = ticks;
    }
  }

} //end class SlotWheel
