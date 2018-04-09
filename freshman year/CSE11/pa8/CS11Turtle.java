/* 
 * Name: Jamie Zhang
 * Login: cs11fmd
 * Date: October 7, 2014
 * File: CS11Turtle.java 
 * Sources of Help: Christopher Cabreros (student), turtleClasses docs
 * 
 * This program creates a three-line drawing of the text "CS11FMD FALL 2014".
 */

import turtleClasses.*;
import java.awt.Color;

/*
* Name: CS11Turtle
* Purpose: This class creates a turtle that draws things.
*/ 
public class CS11Turtle extends Turtle {
  private final static int CHAR_WIDTH = 40;
  private final static int CHAR_HEIGHT = 80;
  private final static int PADDING_BETWEEN_CHARS = 40; 
  private final static int PADDING_BETWEEN_LINES = 40; 
  private final static int CHAR_SPACING = CHAR_WIDTH + PADDING_BETWEEN_CHARS;
  private final static int LINE_SPACING = CHAR_HEIGHT + PADDING_BETWEEN_LINES;

  private final static int HALF_OF_HEIGHT = CHAR_HEIGHT/2; 
   //half height of char
  private final static int FOURTH_OF_HEIGHT = CHAR_HEIGHT/4;
   //fourth of height
  private final static int HALF_OF_WIDTH = CHAR_WIDTH/2; //half width of char
  private final static int FOURTH_OF_WIDTH = CHAR_WIDTH/4; //fourth of width

  private final static int NUMBER_CHARS_LINE1 = 7; //number of chars in line 1
  private final static int NUMBER_CHARS_LINE2 = 4; //number of chars in line 2
  private final static int NUMBER_CHARS_LINE3 = 4; //number of chars in line 3
  private final static int NUMBER_LINES = 3; //number of lines

  private final static int PEN_WIDTH = 10;
  private final static int PEN_WIDTH2 = 30; 
  private final static int PEN_WIDTH3 = 60;
  private final static Color PEN_COLOR = new Color(238, 168, 29); 
  private final static Color PEN_COLOR2 = new Color (193, 18, 18);
  private final static Color PEN_COLOR3 = Color.BLUE;
  private final static Color PEN_COLOR4 = Color.WHITE;
 
  private final static int CIRCLE1_X = 115;
  private final static int CIRCLE1_Y = 185;
  private final static int LENGTH1 = 30;

  private final static int CIRCLE2_X = 180;
  private final static int CIRCLE2_Y = 175;
  private final static int LENGTH2 = 20;

  private final static int CIRCLE3_X = 227;
  private final static int CIRCLE3_Y = 175;
  private final static int LENGTH3 = 13;

  private final static int POINT1_X = 300;
  private final static int POINT1_Y = 120;
  private final static int POINT2_X = 350;
  private final static int POINT2_Y = 290;
  private final static int POINT3_X = 200;
  private final static int POINT3_Y = 175;
  private final static int POINT4_X = 400;
  private final static int POINT4_Y = 175;
  private final static int POINT5_X = 250;
  private final static int POINT5_Y = 290;

  private final static int WORLD_WIDTH = 600; 
  private final static int WORLD_HEIGHT = 400; 

  /*
  * Changes text position. Currently modified to center text within the world.
  */
  private final static int START_X_1 = (WORLD_WIDTH - (CHAR_SPACING *
   NUMBER_CHARS_LINE1 - PADDING_BETWEEN_CHARS))/2; 
   // starting x offset for line 1 
  private final static int START_X_2 = (WORLD_WIDTH - (CHAR_SPACING * 
   NUMBER_CHARS_LINE2 - PADDING_BETWEEN_CHARS))/2; 
   // starting x offset for line 2 
  private final static int START_X_3 = (WORLD_WIDTH - (CHAR_SPACING * 
   NUMBER_CHARS_LINE3 - PADDING_BETWEEN_CHARS))/2; 
   // starting x offset for line 3 
  private final static int START_Y = (WORLD_HEIGHT - (LINE_SPACING *
   NUMBER_LINES - PADDING_BETWEEN_LINES))/2;; // starting y offset 

  /*
  * Constants for drawing the circles.
  */
  private final static int ANGLE_FULL = 360; //angle of circle
  private final static int ANGLE_FOURTIETH = ANGLE_FULL/40; //1/30th of 360deg
  private final static int ITERATIONS = 40; //number of iterations

  /* 
  * Delay between turtle actions (turns, moves) in milliseconds. 
  * 1000 = 1 sec. so 200 = 0.2 sec. 
  */ 
  private final static int DELAY = 50; 

 
  /* 
  * Name: CS11Turtle
  * Purpose: Creates a turtle that draws things with some delay on a world.
  * Parameters: w - a world
  *             delay - the delay between the turtle's actions
  */  
  public CS11Turtle(World w, int delay) { 
    super(w, delay); // Invoke superclass's ctor to create this turtle 
  }                  // on World w with delay of each turtle's action. 

 
  /* 
  * Name: drawC
  * Purpose: Draws the letter C in CS11FMD
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */ 
  private void drawC(int x, int y) { 
    penUp(); 
    moveTo(x, y); // always start in upper left corner of this char block 
    turnToFace(getXPos() + 1, getYPos()); // face right
    penDown();
    forward(CHAR_WIDTH); 
    backward(CHAR_WIDTH); 
    turnRight(); 
    forward(CHAR_HEIGHT); 
    turnLeft(); 
    forward(CHAR_WIDTH); 
  } 


  /* 
  * Name: drawS
  * Purpose: Draws the letter S in CS11FMD
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */ 
  private void drawS(int x, int y) { 
    penUp(); 
    moveTo(x, y); //starts drawing in the upper left corner of this char block
    turnToFace(getXPos() + 1, getYPos()); // face right 
    penDown();
    forward(CHAR_WIDTH);
    backward(CHAR_WIDTH);
    turnRight();
    forward(HALF_OF_HEIGHT);
    turnLeft();
    forward(CHAR_WIDTH);
    turnRight();
    forward(HALF_OF_HEIGHT);
    turnRight();
    forward(CHAR_WIDTH);
  } 


  /*
  * Name: drawOne
  * Purpose: Draws the number 1 in CS11FMD and 2014
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawOne(int x, int y) {  
    penUp();
    moveTo(x, y);
    turnToFace(getXPos(), getYPos() + 1);
    forward(FOURTH_OF_HEIGHT);
    turnToFace(x + HALF_OF_WIDTH, y);
    penDown();
    moveTo(x + HALF_OF_WIDTH, y);
    turnToFace(getXPos(), getYPos() + 1);
    forward(CHAR_HEIGHT);
    penUp();
    moveTo(x, y + CHAR_HEIGHT);
    turnToFace(getXPos() + 1, getYPos());
    penDown();
    forward(CHAR_WIDTH);
  }
  

  /*
  * Name: drawF
  * Purpose: Draws the letter F in CS11FMD and FALL
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawF(int x, int y) {  
    penUp();
    moveTo(x, y);
    turnToFace(getXPos() + 1, getYPos());
    penDown();
    forward(CHAR_WIDTH);
    backward(CHAR_WIDTH);
    turnToFace(getXPos(), getYPos() + 1);
    forward(CHAR_HEIGHT);
    backward(HALF_OF_HEIGHT);
    turnToFace(getXPos() + 1, getYPos());
    forward(CHAR_WIDTH);
  }


  /*
  * Name: drawM
  * Purpose: Draws the letter M in CS11FMD
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawM(int x, int y) {  
    penUp();
    moveTo(x, y);
    turnToFace(getXPos(), getYPos() + 1);
    penDown();
    forward(CHAR_HEIGHT);
    backward(CHAR_HEIGHT);
    moveTo(x + HALF_OF_WIDTH, y + HALF_OF_HEIGHT);
    moveTo(x + CHAR_WIDTH, y);
    turnToFace(getXPos(), getYPos() + 1);
    forward(CHAR_HEIGHT);
  }


  /*
  * Name: drawD
  * Purpose: Draws the letter D in CS11FMD
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawD(int x, int y) {  
    penUp();
    moveTo(x, y);
    turnToFace(getXPos(), getYPos() + 1);
    penDown();
    forward(CHAR_HEIGHT);
    backward(CHAR_HEIGHT);
    moveTo(x + HALF_OF_WIDTH, y);
    moveTo(x + CHAR_WIDTH, y + HALF_OF_HEIGHT);
    moveTo(x + HALF_OF_WIDTH, y + CHAR_HEIGHT);
    moveTo(x, y + CHAR_HEIGHT);
  }
  

  /*
  * Name: drawA
  * Purpose: Draws the letter A in FALL
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawA(int x, int y) {  
    penUp();
    moveTo(x + HALF_OF_WIDTH, y);
    penDown();
    moveTo(x, y + CHAR_HEIGHT);
    penUp();
    moveTo(x + HALF_OF_WIDTH, y);
    penDown();
    moveTo(x + CHAR_WIDTH, y + CHAR_HEIGHT);
    penUp();
    moveTo(x + FOURTH_OF_WIDTH, y + HALF_OF_HEIGHT);
    penDown();
    moveTo(x + CHAR_WIDTH - FOURTH_OF_WIDTH, y + HALF_OF_HEIGHT);
  }


  /*
  * Name: drawL
  * Purpose: Draws the letter L in FALL
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawL(int x, int y) {  
    penUp();
    moveTo(x, y);
    turnToFace(x, y + 1);
    penDown();
    forward(CHAR_HEIGHT);
    moveTo(x + CHAR_WIDTH, y + CHAR_HEIGHT);
  }


  /*
  * Name: drawTwo
  * Purpose: Draws the number 2 in 2014
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawTwo(int x, int y) {  
    penUp();
    moveTo(x, y);
    turnToFace(x + 1, y);
    penDown();
    forward(CHAR_WIDTH);
    moveTo(x + CHAR_WIDTH, y + HALF_OF_HEIGHT);
    moveTo(x, y + HALF_OF_HEIGHT);
    moveTo(x, y + CHAR_HEIGHT);
    moveTo(x + CHAR_WIDTH, y + CHAR_HEIGHT);
  }


  /*
  * Name: drawZero
  * Purpose: Draws the number 0 in 2014
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawZero(int x, int y) {  
    penUp();
    moveTo(x, y);
    turnToFace(x + 1, y);
    penDown();
    forward(CHAR_WIDTH);
    moveTo(x + CHAR_WIDTH, y + CHAR_HEIGHT);
    moveTo(x, y + CHAR_HEIGHT);
    moveTo(x, y);
    penUp();
    moveTo(x, y + CHAR_HEIGHT);
    penDown();
    moveTo(x + CHAR_WIDTH, y);
  }


  /*
  * Name: drawFour
  * Purpose: Draws the number 4 in 2014
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawFour(int x, int y) {  
    penUp();
    moveTo(x + CHAR_WIDTH - FOURTH_OF_WIDTH, y);
    turnToFace(getXPos(), getYPos() + 1);
    penDown();
    forward(CHAR_HEIGHT);
    penUp();
    moveTo(x + CHAR_WIDTH - FOURTH_OF_WIDTH, y);
    penDown();
    moveTo(x, y + HALF_OF_HEIGHT);
    moveTo(x + CHAR_WIDTH, y + HALF_OF_HEIGHT);
  }


  /*
  * Name: drawCircle1
  * Purpose: Draws a circle
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawCircle1(int x, int y) {  
    penUp();
    moveTo(x, y);
    penDown();
    for (int a = 0; a <= ITERATIONS; a++) { //draws a circle
      turn(ANGLE_FOURTIETH);
      forward(LENGTH1);
    }
  }


  /*
  * Name: drawCircle2
  * Purpose: Draws a second circle
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawCircle2(int x, int y) {  
    penUp();
    moveTo(x, y);
    penDown();
    for (int a = 0; a <= ITERATIONS; a++) { //draws a circle
      turn(ANGLE_FOURTIETH);
      forward(LENGTH2);
    }
  }


 /*
  * Name: drawCircle3
  * Purpose: Draws a third circle
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawCircle3(int x, int y) {  
    penUp();
    moveTo(x, y);
    penDown();
    for (int a = 0; a <= ITERATIONS; a++) { //draws a circle
      turn(ANGLE_FOURTIETH);
      forward(LENGTH3);
    }
  }


  /*
  * Name: drawStar
  * Purpose: Draws a star
  * Parameters: x - some position x, the starting position of the turtle
  *             y - some position y, the starting position of the turtle
  * Return: void
  */
  private void drawStar(int x, int y) {  
    penUp();
    moveTo(POINT1_X, POINT1_Y);
    penDown();
    moveTo(POINT2_X, POINT2_Y);
    moveTo(POINT3_X, POINT3_Y);
    moveTo(POINT4_X, POINT4_Y);
    moveTo(POINT5_X, POINT5_Y);
    moveTo(POINT1_X, POINT1_Y);
  }


  /* 
  * Name: main
  * Purpose: Runs the code in a defined order.
  * Parameters: None
  * Return: void 
  */   
  public static void main(String [] args) { 
    int startX1 = START_X_1, // starting x offset for line 1 
    startX2 = START_X_2, // starting x offset for line 2 
    startX3 = START_X_3; // starting x offset for line 3 
    int startY = START_Y; // starting y offset 
    int x, y;  
    World w = new World(WORLD_WIDTH, WORLD_HEIGHT); 
    CS11Turtle myTurtle = new CS11Turtle(w, DELAY); 
  
    myTurtle.setColor(Color.PINK); //changes the turtle color
    
    //draws the background
    myTurtle.setPenWidth(PEN_WIDTH2); 
    myTurtle.setPenColor(PEN_COLOR2);
    myTurtle.drawCircle1(CIRCLE1_X, CIRCLE1_Y);

    myTurtle.setPenWidth(PEN_WIDTH2); 
    myTurtle.setPenColor(PEN_COLOR2);
    myTurtle.drawCircle2(CIRCLE2_X, CIRCLE2_Y);
 
    myTurtle.setPenWidth(PEN_WIDTH3); 
    myTurtle.setPenColor(PEN_COLOR3);
    myTurtle.drawCircle3(CIRCLE3_X, CIRCLE3_Y);

    myTurtle.setPenWidth(PEN_WIDTH2);
    myTurtle.setPenColor(PEN_COLOR4);
    myTurtle.drawStar(0, 0);

    //draws the text
    myTurtle.setPenWidth(PEN_WIDTH); 
    myTurtle.setPenColor(PEN_COLOR);
    myTurtle.drawC(x = startX1, y = startY); 
    myTurtle.drawS(x += CHAR_SPACING, y); 
    myTurtle.drawOne(x += CHAR_SPACING, y);
    myTurtle.drawOne(x += CHAR_SPACING, y);
    myTurtle.drawF(x += CHAR_SPACING, y);
    myTurtle.drawM(x += CHAR_SPACING, y);
    myTurtle.drawD(x += CHAR_SPACING, y);
    myTurtle.drawF(x = startX2, y = startY + LINE_SPACING);
    myTurtle.drawA(x += CHAR_SPACING, y = startY + LINE_SPACING);
    myTurtle.drawL(x += CHAR_SPACING, y = startY + LINE_SPACING);
    myTurtle.drawL(x += CHAR_SPACING, y = startY += LINE_SPACING);
    myTurtle.drawTwo(x = startX3, y = startY + LINE_SPACING);
    myTurtle.drawZero(x += CHAR_SPACING, y = startY + LINE_SPACING);
    myTurtle.drawOne(x += CHAR_SPACING, y = startY + LINE_SPACING);
    myTurtle.drawFour(x += CHAR_SPACING, y = startY + LINE_SPACING);
  } 

} // End of public class CS11Turtle extends Turtle
