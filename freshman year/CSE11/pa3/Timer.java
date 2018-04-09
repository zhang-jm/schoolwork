/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: October 23, 2014
* File: Timer.java
* Sources of Help: Java: An Eventful Approach (textbook)
* 
* This program creates a timer that records the amount of time that passes
* between the start and stop of the timer in milliseconds and can convert that
* amount of time into seconds.
*/

/*
* Name: Timer
* Purpose: Records the amount of time that passes between the start and stop
* of the timer in milliseconds and can convert that time into seconds.
*/
public class Timer
{ 
  //time when timer started or was resetted
  private double startTime;
  
  /*
  * Name: Timer
  * Purpose: Creates a timer and initializes startTime with the current
  * time
  * Parameters: none
  */
  public Timer()
  {
    startTime = System.currentTimeMillis();
  }

  /*
  * Name: elapsedMilliseconds
  * Purpose: Returns the number of milliseconds since the timer was started
  * or last reset
  * Parameters: none
  * Return: double
  */
  public double elapsedMilliseconds()
  {
    //calculates the amount of time in milliseconds from the start and stop
    //of the timer
    return System.currentTimeMillis() - startTime;
  }

  /*
  * Name: elapsedSeconds
  * Purpose: Returns the number of seconds since the timer was started
  * or last reset
  * Parameters: none
  * Return: double
  */
  public double elapsedSeconds()
  {
    //converts milliseconds into seconds
    return this.elapsedMilliseconds() / 1000;
  }

  /*
  * Name: reset
  * Purpose: Resets the timer
  * Parameters: none
  * Return: void
  */
  public void reset()
  {
    startTime = System.currentTimeMillis();
  }

} //end class Timer
