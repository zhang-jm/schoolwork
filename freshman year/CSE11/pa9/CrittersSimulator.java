/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: December 4, 2014
* File: CritterSimulator.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs,
*                  Java API docs, PA9
*
* Starts and runs threads to allow the critters to move.
*/

import objectdraw.*;
import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;

/*
* Name: CrittersController
* Purpose: Runs threads so the critters move/react.
*/
public class CrittersSimulator extends ActiveObject
{
  private ArrayList<Critter> critters;

  private static final int DELAY = 75;
  private boolean isStarted;

  Critter firstCritter;
  Critter secondCritter;
  Critter closest;

  /*
  * Name: CrittersSimulator
  * Purpose: Gets the ArrayList of critters from the controller.
  * Parameters: critters - the ArrayList of critters created in the controller
  */ 
  public CrittersSimulator(ArrayList<Critter> critters)
  {
    this.critters = critters;
    isStarted = true;

    start();
  }	  

  /*
  * Name: run
  * Purpose: Creates the thread for the critters to move
  * Parameters: none
  * Return: void
  */   
  public void run() 
  {
    while(true) 
    {
      pause(DELAY);
 
      if(isStarted && critters.size() > 1)
      {
        for(int i = 0; i < critters.size(); i++)
	{
	  firstCritter = critters.get(i);
	  closest = null;

          for(int j = 0; j < critters.size(); j++)
	  {
	    secondCritter = critters.get(j);

	    if(firstCritter instanceof Chaser &&
	       (secondCritter instanceof Runner || 
	       secondCritter instanceof Random ||
	       secondCritter instanceof Imitator))
	    {
              this.compareCritters(firstCritter, secondCritter, closest);
	    }

	    else if(firstCritter instanceof Runner && 
		    secondCritter != firstCritter &&
		   (secondCritter instanceof Runner ||
		    secondCritter instanceof Chaser ||
		    secondCritter instanceof Random ||
		    secondCritter instanceof Imitator))
	    {	    
              this.compareCritters(firstCritter, secondCritter, closest);
	    }
	  }	    

	  firstCritter.reactTo(closest);
	}
      }
    }
  }

  /*
  * Name: compareCritters
  * Purpose: Used to find the closest critter to another critter
  * Parameters: first - a critter
  *             second - critter to compare distance
  *             closest - current critter closest to the first critter
  * Return: void
  */   
  public void compareCritters(Critter first, Critter second, Critter closest)
  {
    if(closest == null)
    {
      this.closest = second;
    }
    else if(first.distanceTo(second) < first.distanceTo(closest))
    {
      this.closest = second;
    }	 	  
  }

  /*
  * Name: setBoolean
  * Purpose: sets the boolean to find if the controller is started or not
  * Parameters: bool - boolean for if the controller is started or not
  * Return: void
  */   
  public void setBoolean(boolean bool)
  {
    isStarted = bool;
  }

  /*
  * Name: getBoolean
  * Purpose: Gets the boolean
  * Parameters: none
  * Return: boolean
  */ 
  public boolean getBoolean()
  {
    return isStarted;
  }
}

