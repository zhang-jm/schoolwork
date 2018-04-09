/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: October 27, 2014
* File: ResizablePacManController.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs
*
* After first opening this program, the Applet Viewer window shows a blank
* canvas with one vertical and one horizontal line, separating the window
* into four quadrants. When the mouse is clicked, a new ResizablePacMan
* object is created and centered at the location where the mouse was 
* clicked. If the ResizablePacMan is in the upper left quadrant, its color
* is magenta; if its in the lower left quadrant, its color is yellow; 
* upper right quadrant, the color is cyan; and lower right quadrant, the
* color is black. The ResizablePacMan grows, shrinks, and rotates.
*/

import java.awt.*;
import java.awt.event.*;
import javax.swing.*;
import javax.swing.event.*;
import objectdraw.*;


/*
* Name: ResizablePacManController
* Purpose: Creates a canvas that can be manipulated to create new
* ResizablePacMan objects.
*/
public class ResizablePacManController extends WindowController
                                       implements ActionListener,
                                                  ChangeListener,
                                                  MouseListener,
                                                  MouseMotionListener
{
  private Line horizLine, vertLine;
  private double widthProportion = 0.5, heightProportion = 0.5;

  private JPanel topPanel;
  private JPanel textPanel;
  private JPanel buttonPanel;
  private JPanel bottomPanel;

  private JLabel instructions;
  private JLabel speedLabel;
  private JButton start;
  private JButton stop;
  private JButton clear;
  private JButton switchSpin;
  private JSlider speedSlider;

  //size of PacMan before growing or shrinking; start size
  private double pacManSize = 30;
  private static final int MIN_SPEED = 1;
  private static final int MAX_SPEED = 100;
  //speed of PacMan
  private int speed = 50;

  private Location horizLineStart;
  private Location vertLineStart;

  private double canvasWidth;
  private double canvasHeight;
  private static final double HALF = 0.5;
  
  //lines cannot move past this amount on the canvas
  private static final int LINE_PADDING = 5;

  //checks if the lines have been pressed on, and if so, which line(s)
  private boolean horizIsClicked = false;
  private boolean vertIsClicked = false;
  private boolean bothClicked = false;
  private boolean isRunning = true;
  private boolean isSpinningClockwise = true;

  /*
  * Name: begin
  * Purpose: Sets up the Applet Viewer window and displays a horizontal
  * and vertical line.
  * Parameters: none
  * Return: void
  */
  public void begin()
  { 
    topPanel = new JPanel();
    textPanel = new JPanel();
    buttonPanel = new JPanel();
    bottomPanel = new JPanel();

    instructions = new JLabel("Resizable PacMen Controls");
    start = new JButton("Start");
    stop = new JButton("Stop");
    clear = new JButton("Clear All");
    switchSpin = new JButton("Switch Spin");

    speedLabel = new JLabel("The speed is " + speed);
    speedSlider = new JSlider(JSlider.HORIZONTAL, MIN_SPEED, MAX_SPEED, speed);

    topPanel.setLayout(new BorderLayout());

    this.add(topPanel, BorderLayout.NORTH);
    this.add(bottomPanel, BorderLayout.SOUTH);
    topPanel.add(textPanel, BorderLayout.NORTH);
    topPanel.add(buttonPanel, BorderLayout.SOUTH);

    textPanel.add(instructions);
    buttonPanel.add(start);
    buttonPanel.add(stop);
    buttonPanel.add(clear);
    buttonPanel.add(switchSpin);
    
    bottomPanel.add(speedLabel);
    bottomPanel.add(speedSlider);

    canvas.addMouseListener(this);
    canvas.addMouseMotionListener(this);

    start.addActionListener(this);
    stop.addActionListener(this);
    clear.addActionListener(this);
    switchSpin.addActionListener(this);
    speedSlider.addChangeListener(this);

    this.validate();

    //creates the horizontal and vertical lines
    horizLine = new Line(0, canvas.getHeight() * HALF, canvas.getWidth(), 
		         canvas.getHeight() * HALF, canvas);
    vertLine = new Line (canvas.getWidth() * HALF, 0, canvas.getWidth() * HALF, 
		         canvas.getHeight(), canvas);
    
    horizLineStart = horizLine.getStart();
    vertLineStart = vertLine.getStart();

    //initializes the proportions of the lines
    widthProportion = vertLineStart.getX() / canvas.getWidth();
    heightProportion = horizLineStart.getY() / canvas.getHeight();
  }

  /*
  * Name: paint
  * Purpose: Moves the lines when the window is resized, so that the lines
  * keep their original location in relation to the size of the window.
  * Parameters: java.awt.Graphics g
  * Return: void
  */
  public void paint(java.awt.Graphics g)
  {
    //makes a call to the superclass's version of the method
    super.paint(g);

    //repositions the lines based on the proportions of where they were
    //previously
    horizLine.setStart(0, canvas.getHeight() * heightProportion);
    horizLine.setEnd(canvas.getWidth(), canvas.getHeight() * heightProportion);
    vertLine.setStart(canvas.getWidth() * widthProportion, 0);
    vertLine.setEnd(canvas.getWidth() * widthProportion, canvas.getHeight());

  }

  /*
  * Name: onMousePress
  * Purpose: Checks if any of the lines have been pressed on. 
  * Parameters: point - the point location where the mouse was pressed
  * Return: void
  */
  @Override
  public void mousePressed(MouseEvent evt)
  {
    Location point = new Location(evt.getX(), evt.getY());

    //if both lines were pressed on
    if (horizLine.contains(point) && vertLine.contains(point))
    {
      bothClicked = true;
    }

    //if the horizontal line was pressed on
    else if(horizLine.contains(point))
    {
      horizIsClicked = true;
    }

    //if the vertical line was pressed on
    else if (vertLine.contains(point))
    {
      vertIsClicked = true;
    }
  }

  /*
  * Name: onMouseRelease
  * Purpose: Resets the boolean values for if the lines are pressed on
  * Parameters: point - the point location where the mouse was released
  * Return: void
  */
  @Override
  public void mouseReleased(MouseEvent evt)
  {
    //none of the lines are being pressed on anymore
    horizIsClicked = false;
    vertIsClicked = false;
    bothClicked = false;
  }

  /*
  * Name: onMouseClick
  * Purpose: Creates a new ResizablePacMan object at the location where the
  * mouse was clicked.
  * Parameters: point - the point location where the mouse is clicked
  * Return: void
  */
  @Override
  public void mouseClicked(MouseEvent evt)
  {
    Location point = new Location(evt.getX(), evt.getY());

    //creates a new ResizablePacMan object
    new ResizablePacMan(point, pacManSize, canvas,
                        start, stop, switchSpin, speedSlider, isRunning,
                        isSpinningClockwise, speed, horizLine, vertLine);
  }

  @Override
  public void mouseEntered(MouseEvent evt)
  {

  }

  @Override
  public void mouseExited(MouseEvent evt)
  {
  
  }

  /*
  * Name: onMouseDrag
  * Purpose: If any line(s) were being pressed on, said line(s) can be dragged 
  * Parameters: point - the point location where the mouse is dragged to
  * Return: void
  */
  @Override
  public void mouseDragged(MouseEvent evt)
  {
    Location point = new Location(evt.getX(), evt.getY());
    
    //if the point where the line is being dragged to is within the 5 pixel
    //limit on all sides
    if(point.getX() > LINE_PADDING && 
       point.getX() < canvas.getWidth() - LINE_PADDING &&
       point.getY() > LINE_PADDING &&
       point.getY() < canvas.getHeight() - LINE_PADDING)
    {
      //if the vertical line has been pressed on
      if(vertIsClicked)
      {
        //moves the vertical line
        vertLine.setStart(point.getX(), 0);
        vertLine.setEnd(point.getX(), canvas.getHeight());
      }

      //if the horizontal line has been pressed on
      else if(horizIsClicked)
      {
        //moves the horizontal line
        horizLine.setStart(0, point.getY());
        horizLine.setEnd(canvas.getWidth(), point.getY());
      }
      
      //if both lines have been pressed on
      else if(bothClicked)
      {
        //moves both lines
        horizLine.setStart(0, point.getY());
        horizLine.setEnd(canvas.getWidth(), point.getY());  
        vertLine.setStart(point.getX(), 0);
        vertLine.setEnd(point.getX(), canvas.getHeight());
      }
      
      //gets the location of the lines
      horizLineStart = horizLine.getStart();
      vertLineStart = vertLine.getStart();

      //resets the proportions of the lines in relation to the canvas
      widthProportion = vertLineStart.getX() / canvas.getWidth();
      heightProportion = horizLineStart.getY() / canvas.getHeight();
    }
  }

  @Override
  public void mouseMoved(MouseEvent evt)
  {
   
  }

  @Override
  public void actionPerformed(ActionEvent evt)
  { 
    if(evt.getSource() == start)
    {
      isRunning = true;  
    } 
    else if(evt.getSource() == stop)
    {
      isRunning = false;
    }  
    else if(evt.getSource() == clear)
    {
      canvas.clear();
 
      horizLine = new Line(0, canvas.getHeight() * HALF, canvas.getWidth(), 
		         canvas.getHeight() * HALF, canvas);
      vertLine = new Line (canvas.getWidth() * HALF, 0, canvas.getWidth() * HALF, 
		         canvas.getHeight(), canvas);

      horizLine.setStart(0, canvas.getHeight() * heightProportion);
      horizLine.setEnd(canvas.getWidth(), canvas.getHeight() * 
                       heightProportion);
      vertLine.setStart(canvas.getWidth() * widthProportion, 0);
      vertLine.setEnd(canvas.getWidth() * widthProportion, 
                      canvas.getHeight());
    }  
    else
    {
      isSpinningClockwise = !isSpinningClockwise;
    }
  }

  @Override
  public void stateChanged(ChangeEvent evt)
  {
    speed = speedSlider.getValue();
    speedLabel.setText("The speed is " + speed);
  }


} //end class ResizablePacManController

