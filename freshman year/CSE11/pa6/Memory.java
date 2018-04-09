/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: November 11, 2014
* File: Memory.java
* Sources of Help: Christopher Cabreros (student), PA6 instructions, 
* discussion slides, objectdraw docs, Java API docs
* 
* This program is a memory game that can be run in the AppletViewer window. 
* The goal of the game is to correctly match animal images.
*/

import objectdraw.*;
import java.awt.*;
import java.util.*;
import java.awt.event.*;

/*
* Name: Memory
* Purpose: Creates a memory game that can be run in the AppletViewer window.
*/
public class Memory extends WindowController
                    implements MouseListener    
{
  AnimalCard animal;
  AnimalCard animal2;

  Chicken chicken1;
  Chicken chicken2; 
  Cow cow1;
  Cow cow2;
  Duck duck1;
  Duck duck2;
  Kitty kitty1;
  Kitty kitty2;
  Lamb lamb1;
  Lamb lamb2;
  Lion lion1;
  Lion lion2;
  Mouse mouse1;
  Mouse mouse2;
  Puppy puppy1;
  Puppy puppy2;
  Turkey turkey1;
  Turkey turkey2;

  //the number of images to be displayed, twice the number of animals
  private static final int NUMBER_OF_IMAGES = 18;
  //placement of the text from the bottom of the canvas
  private static final int PIXELS_FROM_BOTTOM = 20;
  private static final int HALF = 2; //used to center text

  //used within the switch statement used to pick an animal
  private static final int FIRST_CASE = 0;
  private static final int SECOND_CASE = 1;
  private static final int THIRD_CASE = 2;
  private static final int FOURTH_CASE = 3;
  private static final int FIFTH_CASE = 4;
  private static final int SIXTH_CASE = 5;
  private static final int SEVENTH_CASE = 6;
  private static final int EIGHTH_CASE = 7;
  private static final int NINTH_CASE = 8;
  private static final int TENTH_CASE = 9;
  private static final int ELEVENTH_CASE = 10;
  private static final int TWELFTH_CASE = 11;
  private static final int THIRTEENTH_CASE = 12;
  private static final int FOURTEENTH_CASE = 13;
  private static final int FIFTEENTH_CASE = 14;
  private static final int SIXTEENTH_CASE = 15;
  private static final int SEVENTEENTH_CASE = 16; 
  private static final int EIGHTEENTH_CASE = 17;
  
  //upper left corner Location of an image
  Location origin;

  //used to place the images at their correct locations on the canvas
  Location first = new Location(5, 5);
  Location second = new Location(110, 5);
  Location third = new Location(215, 5);
  Location fourth = new Location(5, 110);
  Location fifth = new Location(110, 110);
  Location sixth = new Location(215, 110);
  Location seventh = new Location(5, 215);
  Location eighth = new Location(110, 215);
  Location ninth = new Location(215, 215);
  Location tenth = new Location(5, 320);
  Location eleventh = new Location(110, 320);
  Location twelfth = new Location(215, 320);
  Location thirteenth = new Location(5, 425);
  Location fourteenth = new Location(110, 425);
  Location fifteenth = new Location(215, 425);
  Location sixteenth = new Location(5, 530);
  Location seventeenth = new Location(110, 530);
  Location eighteenth = new Location(215, 530);

  Text speech;
  
  //checks if the first image has been clicked/flipped over
  boolean clickedOnce = false;
  //checks if the animal/animal2 objects are null or not
  boolean hasAValue = false;
  //checks if a pair has been made
  boolean madeAPair = false;
  //checks if the pair is incorrect or correct
  boolean isIncorrect = false;
  //checks if the cards are being flipped over/reset after an incorrect pair
  boolean isBeingReset = false;

  /*
  * Name: begin
  * Purpose: Sets up the AppletViewer window with everything necessary to
  * start the game.
  * Parameters: none
  * Return: void
  */
  public void begin()
  {
    //allows the canvas to react to Mouse Events
    canvas.addMouseListener(this); 

    //creates and hides a text object that will display the animals' speech
    speech = new Text(" ", 1, 1, canvas);
    speech.hide();
  
    //places the animalcards in randomized locations on the board
    placeAnimals();
    //hides the images and places a black border around them
    moveToStartPosition();
  }

  /*
  * Name: mouseClicked
  * Purpose: Reacts when the mouse has been clicked. Flips images over, checks
  * if pairs have been made, checks if the pairs are correct or incorrect,
  * etc.
  * Parameters: evt - the MouseEvent
  * Return: void
  */
  public void mouseClicked(MouseEvent evt)
  {
    //location of the mouse press
    Location point = new Location(evt.getX(), evt.getY());

    //checks if this is the click to show the first image in the pair
    if(!clickedOnce && !isBeingReset)
    {
      //if an image is being clicked on and that animal was not previously
      //matched correctly already, set that image's object to an 
      //AnimalCard reference
      if (kitty1.contains(point) && kitty1.getHighlightColor() != Color.GREEN)
      {	
        animal = kitty1;
        animal.show(); //shows the image
        clickedOnce = true; //the first image has been revealed	
      }
      else if (kitty2.contains(point) && 
	       kitty2.getHighlightColor() != Color.GREEN)
      {	
        animal = kitty2;
        animal.show();	
        clickedOnce = true;	
      }
      else if (duck1.contains(point) &&
	       duck1.getHighlightColor() != Color.GREEN)
      {
        animal = duck1;
        animal.show();	
        clickedOnce = true;	
      }
      else if (duck2.contains(point) && 
		      duck2.getHighlightColor() != Color.GREEN)
      {
        animal = duck2;
        animal.show();	
	clickedOnce = true;
      }
      else if (puppy1.contains(point) && 
		      puppy1.getHighlightColor() != Color.GREEN)
      {
        animal = puppy1;
        animal.show();	
        clickedOnce = true;	
      }
      else if (puppy2.contains(point) && 
		      puppy2.getHighlightColor() != Color.GREEN)
      {
        animal = puppy2;
        animal.show();	
        clickedOnce = true;	
      }      
      else if (lion1.contains(point) && 
		      lion1.getHighlightColor() != Color.GREEN)
      {
        animal = lion1;
        animal.show();	
        clickedOnce = true;	
      }
      else if (lion2.contains(point) && 
		      lion2.getHighlightColor() != Color.GREEN)
      {
        animal = lion2;
        animal.show();	
        clickedOnce = true;	
      }      
      else if (cow1.contains(point) && 
		      cow1.getHighlightColor() != Color.GREEN)
      {	
        animal = cow1;
        animal.show();	
        clickedOnce = true;	
      } 
      else if (cow2.contains(point) && 
		      cow2.getHighlightColor() != Color.GREEN)
      {	
        animal = cow2;
        animal.show();
        clickedOnce = true;	
      }
      else if (lamb1.contains(point) && 
		      lamb1.getHighlightColor() != Color.GREEN)
      {
        animal = lamb1;
        animal.show();	
        clickedOnce = true;	
      }
      else if (lamb2.contains(point) && 
		      lamb2.getHighlightColor() != Color.GREEN)
      {
        animal = lamb2;
        animal.show();	
        clickedOnce = true;	
      }      
      else if (chicken1.contains(point) && 
		      chicken1.getHighlightColor() != Color.GREEN)
      {
        animal = chicken1;
        animal.show();	
        clickedOnce = true;	
      }
      else if (chicken2.contains(point) && 
		      chicken2.getHighlightColor() != Color.GREEN)
      {
        animal = chicken2;
        animal.show();	
        clickedOnce = true;	
      }
      else if (turkey1.contains(point) && 
		      turkey1.getHighlightColor() != Color.GREEN)
      {
        animal = turkey1;
        animal.show();	
        clickedOnce = true;	
      }
      else if (turkey2.contains(point) && 
		      turkey2.getHighlightColor() != Color.GREEN)
      {
        animal = turkey2;
        animal.show();	
        clickedOnce = true;	
      }
      else if (mouse1.contains(point) &&
		     mouse1.getHighlightColor() != Color.GREEN)
      {
	animal = mouse1;
        animal.show();	
        clickedOnce = true;	
      }
      else if (mouse2.contains(point) &&
		     mouse2.getHighlightColor() != Color.GREEN)
      {
	animal = mouse2;
        animal.show();	
        clickedOnce = true;	
      }
    }

    //checks if this is the click to show the second image in the pair
    else if (clickedOnce && !isBeingReset)
    {
      //if an image is being clicked on and that animal was not previously
      //matched correctly already, set that image's object to a second
      //AnimalCard reference
      if (kitty1.contains(point) && kitty1.getHighlightColor() != Color.GREEN)
      {	
        animal2 = kitty1;
	hasAValue = true; //animal2 is not null
      }
      else if (kitty2.contains(point) && 
		      kitty2.getHighlightColor() != Color.GREEN)
      {	
        animal2 = kitty2;
	hasAValue = true;	
      }
      else if (duck1.contains(point) &&
	       duck1.getHighlightColor() != Color.GREEN)
      {
        animal2 = duck1;
	hasAValue = true;	
      }
      else if (duck2.contains(point) && 
		      duck2.getHighlightColor() != Color.GREEN)
      {
        animal2 = duck2;
	hasAValue = true;	
      }
      else if (puppy1.contains(point) && 
		      puppy1.getHighlightColor() != Color.GREEN)
      {
        animal2 = puppy1;
	hasAValue = true;	
      }
      else if (puppy2.contains(point) && 
		     puppy2.getHighlightColor() != Color.GREEN)
      {
        animal2 = puppy2;
	hasAValue = true;	
      }      
      else if (lion1.contains(point) && 
		      lion1.getHighlightColor() != Color.GREEN)
      {
        animal2 = lion1;
	hasAValue = true;	
      }
      else if (lion2.contains(point) && 
		      lion2.getHighlightColor() != Color.GREEN)
      {
        animal2 = lion2;
	hasAValue = true;	
      }      
      else if (cow1.contains(point) && 
		      cow1.getHighlightColor() != Color.GREEN)
      {	
        animal2 = cow1;
	hasAValue = true;	
      } 
      else if (cow2.contains(point) && 
		      cow2.getHighlightColor() != Color.GREEN)
      {	
        animal2 = cow2;
	hasAValue = true;	
      }
      else if (lamb1.contains(point) && 
		     lamb1.getHighlightColor() != Color.GREEN)
      {
        animal2 = lamb1;
	hasAValue = true;	
      }
      else if (lamb2.contains(point) && 
		      lamb2.getHighlightColor() != Color.GREEN)
      {
        animal2 = lamb2;
	hasAValue = true;	
      }      
      else if (chicken1.contains(point) && 
		      chicken1.getHighlightColor() != Color.GREEN)
      {
        animal2 = chicken1;
	hasAValue = true;	
      }
      else if (chicken2.contains(point) && 
		      chicken2.getHighlightColor() != Color.GREEN)
      {
        animal2 = chicken2;
	hasAValue = true;	
      }
      else if (turkey1.contains(point) && 
		     turkey1.getHighlightColor() != Color.GREEN)
      {
        animal2 = turkey1;
	hasAValue = true;	
      }
      else if (turkey2.contains(point) && 
		     turkey2.getHighlightColor() != Color.GREEN)
      {
        animal2 = turkey2;
	hasAValue = true;	
      }
      else if (mouse1.contains(point) && 
		     mouse1.getHighlightColor() != Color.GREEN)
      {
	animal2 = mouse1;
	hasAValue = true;
      }
      else if (mouse2.contains(point) && 
		     mouse2.getHighlightColor() != Color.GREEN)
      {
	animal2 = mouse2;
	hasAValue = true;	
      }

      //checks if the second click was on the same image
      if(animal == animal2 && hasAValue)
      {
        clickedOnce = true; //allows a different image to be clicked on
      }
      else if(animal2 == null)
      {
        clickedOnce = true;
      }
      //if the two clicks were of different images
      else if (animal != animal2 && hasAValue)
      {
	//resets the boolean so that a new pair can be made      
        clickedOnce = false; 
	//a pair has been made
	madeAPair = true;
	animal2.show();
      }

      //checks if a pair has been made
      if(madeAPair)
      {
	//if the two images are the same (user was correct)
        if (animal.equals(animal2))
        {
          //green border around the two images
          animal.showHighlight(Color.GREEN);
          animal2.showHighlight(Color.GREEN);

	  speech.removeFromCanvas(); //removes the old text from the canvas

	  //shows the new text with that animal's speech
          speech = new Text(animal.speak(), 1, 1, canvas);
          speech.moveTo((canvas.getWidth() - speech.getWidth()) / HALF,
	  	     canvas.getHeight() - PIXELS_FROM_BOTTOM);
	  
	  //resets the AnimalCard objects
	  animal = null;
	  animal2 = null;
	  madeAPair = false; //a new pair can be made
        }
	//if the two images aren't the same (user was incorrect)
        else if(!animal.equals(animal2))
        {
          //shows a red border around the images
          animal.showHighlight(Color.RED);
          animal2.showHighlight(Color.RED);

          isIncorrect = true; //the two images were incorrect
	  madeAPair = false; //a new pair can be made
        }
      }
    }

    //if the incorrect images are being reset
    if(isBeingReset)
    {	   
      //resets the images 
      animal.hide();
      animal2.hide();
      animal.showHighlight(Color.BLACK);
      animal2.showHighlight(Color.BLACK);
     
      //resets boolean values
      isIncorrect = false; 
      isBeingReset = false;
 
      //resets the AnimalCard objects
      animal = null;
      animal2 = null;
      hasAValue = false;

      //runs mouseClicked in order to flip over the image that was clicked on
      //(if any) while flipping over the incorrect images at the same time
      mouseClicked(evt);
    }
  }

  /*
  * Name: mousePressed
  * Purpose: Reacts when the mouse is pressed. Checks if the mouse click is to 
  * flip over images for a new pair or to reset incorrect images.
  * Parameters: evt - the MouseEvent
  * Return: void
  */
  public void mousePressed(MouseEvent evt)
  {
    //if the pair is incorrect
    if(isIncorrect)
    {
      //the mouse click is to reset the images
      isBeingReset = true;
    }
  }

  /*
  * Name: placeAnimals
  * Purpose: Places the animal images at randomized locations on the canvas
  * Parameters: none
  * Return: void
  */
  public void placeAnimals()
  {
    //boolean values to check if an image has already been placed or not
    boolean one = false;
    boolean two = false;
    boolean three = false;
    boolean four = false;
    boolean five = false;
    boolean six = false;
    boolean seven = false;
    boolean eight = false;
    boolean nine = false;
    boolean ten = false;
    boolean eleven = false;
    boolean twelve = false;
    boolean thirteen = false;
    boolean fourteen = false;
    boolean fifteen = false;
    boolean sixteen = false;
    boolean seventeen = false;
    boolean eighteen = false;

    //generates random ints
    Random random = new Random();
    int o;

    int n = 0;

    //runs the program for each of the 18 images
    while (n < NUMBER_OF_IMAGES)
    {
      //sets the location to place the next image, going from first to last
      switch(n)
      {
        case FIRST_CASE: origin = first; break;
        case SECOND_CASE: origin = second; break;
        case THIRD_CASE: origin = third; break;
        case FOURTH_CASE: origin = fourth; break;
        case FIFTH_CASE: origin = fifth; break;
        case SIXTH_CASE: origin = sixth; break;
        case SEVENTH_CASE: origin = seventh; break;
        case EIGHTH_CASE: origin = eighth; break;
        case NINTH_CASE: origin = ninth; break;
        case TENTH_CASE: origin = tenth; break;
        case ELEVENTH_CASE: origin = eleventh; break;
        case TWELFTH_CASE: origin = twelfth; break;
        case THIRTEENTH_CASE: origin = thirteenth; break;
        case FOURTEENTH_CASE: origin = fourteenth; break;
        case FIFTEENTH_CASE: origin = fifteenth; break;
        case SIXTEENTH_CASE: origin = sixteenth; break;
        case SEVENTEENTH_CASE: origin = seventeenth; break;
        case EIGHTEENTH_CASE: origin = eighteenth; break;	      
      }

      //chooses a random int in order to pick a random image to place in 
      //a specific location
      o = random.nextInt(NUMBER_OF_IMAGES);

      //uses the random integer picked in order to determine which image
      //to place for a specific location; only places the image there if the
      //image was not placed anywhere else
      if(o == SECOND_CASE && !one)
      {
	//creates the new image
        chicken1 = new Chicken(getImage("chicken.jpg"), origin, canvas);

	//this image has already been created, will not be created again
	one = true;

	//the loop will run again and go on to the next location
	n++;
      }
      else if(o == THIRD_CASE && !two)
      {
        chicken2 = new Chicken(getImage("chicken.jpg"), origin, canvas);
	two = true;
	n++;
      }
      else if(o == FOURTH_CASE && !three)
      {
        cow1 = new Cow(getImage("cow.jpg"), origin, canvas);
	three = true;
	n++;
      }
      else if(o == FIFTH_CASE && !four)
      {
        cow2 = new Cow(getImage("cow.jpg"), origin, canvas);
	four = true;
	n++;
      }
      else if(o == SIXTH_CASE && !five)
      {
        duck1 = new Duck(getImage("duck.jpg"), origin, canvas);
	five = true;
	n++;
      }
      else if(o == SEVENTH_CASE && !six)
      {
        duck2 = new Duck(getImage("duck.jpg"), origin, canvas);
	six = true;
	n++;
      }
      else if(o == EIGHTH_CASE && !seven)
      {
        kitty1 = new Kitty(getImage("kitty.jpg"), origin, canvas);
	seven = true;
	n++;
      }
      else if(o == NINTH_CASE && !eight)
      {
        kitty2 = new Kitty(getImage("kitty.jpg"), origin, canvas);
	eight = true;
	n++;
      }
      else if(o == TENTH_CASE && !nine)
      {
        lamb1 = new Lamb(getImage("lamb.jpg"), origin, canvas);
	nine = true;
	n++;
      }
      else if(o == ELEVENTH_CASE && !ten)
      {
        lamb2 = new Lamb(getImage("lamb.jpg"), origin, canvas);
	ten = true;
	n++;
      }
      else if(o == TWELFTH_CASE && !eleven)
      {
        lion1 = new Lion(getImage("lion.jpg"), origin, canvas);
	eleven = true;
	n++;
      } 
      else if(o == THIRTEENTH_CASE && !twelve)
      {
        lion2 = new Lion(getImage("lion.jpg"), origin, canvas);
	twelve = true;
	n++;
      } 
      else if(o == FOURTEENTH_CASE && !thirteen)
      {
        mouse1 = new Mouse(getImage("mouse.jpg"), origin, canvas);
	thirteen = true;
	n++;
      } 
      else if(o == FIFTEENTH_CASE && !fourteen)
      {
        mouse2 = new Mouse(getImage("mouse.jpg"), origin, canvas);
	fourteen = true;
	n++;
      } 
      else if(o == SIXTEENTH_CASE && !fifteen)
      {
        puppy1 = new Puppy(getImage("puppy.jpg"), origin, canvas);
	fifteen = true;
	n++;
      } 
      else if(o == SEVENTEENTH_CASE && !sixteen)
      {
        puppy2 = new Puppy(getImage("puppy.jpg"), origin, canvas);
	sixteen = true;
	n++;
      } 
      else if(o == EIGHTEENTH_CASE && !seventeen)
      {
        turkey1 = new Turkey(getImage("turkey.jpg"), origin, canvas);
	seventeen = true;
	n++;
      } 
      else if(o == FIRST_CASE && !eighteen)
      {
        turkey2 = new Turkey(getImage("turkey.jpg"), origin, canvas);
	eighteen = true;
	n++;
      } 
      
      //if the image is already on the canvas, choose a different image
      else
      {
	//picks a new random image to place at that location
        o = random.nextInt(NUMBER_OF_IMAGES);
      }
    }
  }

  /*
  * Name: moveToStartPosition
  * Purpose: Places the animal images at their starting position (hidden, with
  * a black border around the image).
  * Parameters: none
  * Return: void
  */  
  public void moveToStartPosition()
  {
    //hides all the images
    chicken1.hide();
    chicken2.hide(); 
    cow1.hide();
    cow2.hide();
    duck1.hide();
    duck2.hide();
    kitty1.hide();
    kitty2.hide();
    lamb1.hide();
    lamb2.hide();
    lion1.hide();
    lion2.hide();
    mouse1.hide();
    mouse2.hide();
    puppy1.hide();
    puppy2.hide();
    turkey1.hide();
    turkey2.hide();

    //adds a black border around all the images
    chicken1.showHighlight(Color.BLACK);
    chicken2.showHighlight(Color.BLACK); 
    cow1.showHighlight(Color.BLACK);
    cow2.showHighlight(Color.BLACK);
    duck1.showHighlight(Color.BLACK);
    duck2.showHighlight(Color.BLACK);
    kitty1.showHighlight(Color.BLACK);
    kitty2.showHighlight(Color.BLACK);
    lamb1.showHighlight(Color.BLACK);
    lamb2.showHighlight(Color.BLACK);
    lion1.showHighlight(Color.BLACK);
    lion2.showHighlight(Color.BLACK);
    mouse1.showHighlight(Color.BLACK);
    mouse2.showHighlight(Color.BLACK);
    puppy1.showHighlight(Color.BLACK);
    puppy2.showHighlight(Color.BLACK);
    turkey1.showHighlight(Color.BLACK);
    turkey2.showHighlight(Color.BLACK);
  }

  /*
  * Name: mouseReleased
  * Purpose: Needed for the program to compile.
  * Parameters: evt - the mouse event
  * Return: void
  */ 
  public void mouseReleased(MouseEvent evt)
  {
  }

  /*
  * Name: mouseEntered
  * Purpose: Needed for the program to compile.
  * Parameters: evt - the mouse event
  * Return: void
  */  
  public void mouseEntered(MouseEvent evt)
  {
  }

  /*
  * Name: mouseExited
  * Purpose: Needed for the program to compile.
  * Parameters: evt - the mouse event
  * Return: void
  */
  public void mouseExited(MouseEvent evt)
  {
  }  

} //end class Memory

