/*
* Name: Jamie Zhang
* Login: cs11fmd
* Date: December 4, 2014
* File: CrittersController.java
* Sources of Help: Christopher Cabreros (student), objectdraw docs,
*                  Java API docs, PA9
*
* Sets up the GUI for the Critters program.
*/

import objectdraw.*;
import java.awt.*;
import java.awt.event.*;
import java.util.*;
import javax.swing.*;
import javax.swing.event.*;

/*
* Name: CrittersController
* Purpose: Creates a canvas that can be manipulated to create new
* critters.
*/
public class CrittersController extends WindowController
                                implements ActionListener, MouseListener
{
  //the panels, buttons, labels, used in the GUI	
  private JPanel topPanel;
  private JPanel bottomPanel;
  private JPanel instructionPanel;
  private JPanel critterButtonPanel;

  private JLabel status;
  private JLabel instruction;

  private JButton start;
  private JButton stop;
  private JButton clear;
  private JButton chaser;
  private JButton runner;
  private JButton random;
  private JButton imitator;

  private String stopped = new String("Simulation is stopped.");
  private String startedLess = new String("Please add two or more critters.");
  private String startedMore = new String("Simulation is running.");

  private String select = new String("Select which critter to place:");
  private String chaserSelected = 
	                new String("Click on canvas to place a chaser.");
  private String runnerSelected = 
	                new String("Click on canvas to place a runner.");
  private String randomSelected = 
	                new String("Click on canvas to place a random.");
  private String imitatorSelected = 
	                new String("Click on canvas to place a imitator.");

  //sets booleans
  private boolean chaserChosen;
  private boolean runnerChosen;
  private boolean randomChosen;
  private boolean imitatorChosen;

  private boolean isStarted;
  private boolean isStopped;

  private int numCritters;

  //creates a new ArrayList to hold the critters created
  private ArrayList<Critter> critters = new ArrayList<Critter>();
  
  private CrittersSimulator simulator; //used to pass in the ArrayList

  /*
  * Name: begin
  * Purpose: Sets up the Applet Viewer window with all the GUI components/
  * containers.
  * Parameters: none
  * Return: void
  */
  public void begin()
  {
    //sets up the GUI with all the buttons, panels, labels, etc
    topPanel = new JPanel();
    bottomPanel = new JPanel();
    instructionPanel = new JPanel();
    critterButtonPanel = new JPanel();

    bottomPanel.setLayout(new BorderLayout());

    status = new JLabel(startedLess);
    instruction = new JLabel(select);

    start = new JButton("Start");
    stop = new JButton("Stop");
    clear = new JButton("Clear");
    chaser = new JButton("Chaser");
    runner = new JButton("Runner");
    random = new JButton("Random");
    imitator = new JButton("Imitator");

    this.add(topPanel, BorderLayout.NORTH);
    this.add(bottomPanel, BorderLayout.SOUTH);
    topPanel.add(status);
    topPanel.add(start);
    topPanel.add(stop);
    topPanel.add(clear);
    bottomPanel.add(instructionPanel, BorderLayout.NORTH);
    bottomPanel.add(critterButtonPanel, BorderLayout.SOUTH);
    instructionPanel.add(instruction);
    critterButtonPanel.add(chaser);
    critterButtonPanel.add(runner);
    critterButtonPanel.add(random);
    critterButtonPanel.add(imitator);

    //lets button presses or mouse clicks do something
    start.addActionListener(this);
    stop.addActionListener(this);
    clear.addActionListener(this);
    chaser.addActionListener(this);
    runner.addActionListener(this);
    random.addActionListener(this);
    imitator.addActionListener(this);
    canvas.addMouseListener(this);

    //passes in the array list
    simulator = new CrittersSimulator(critters);

    this.validate();
  }

  /*
  * Name: actionPerformed
  * Purpose: Changes labels and boolean statuses based on what buttons were 
  * pressed.
  * Parameters: evt - the action event
  * Return: void
  */
  @Override
  public void actionPerformed(ActionEvent evt)
  {
    if(evt.getSource() == start)
    {
      //sets boolean 
      isStopped = false;
      isStarted = true;

      simulator.setBoolean(isStarted); //sets boolean in the simulator

      //changes the label text
      if(critters.size() <= 1 && isStarted)
      {
        status.setText(startedLess);
      }
      else if(critters.size() > 1 && isStarted)
      {
        status.setText(startedMore);
      }
    }

    else if(evt.getSource() == stop)
    {
      //sets boolean
      isStarted = false;
      isStopped = true;

      simulator.setBoolean(isStarted); //sets boolean in simuulator

      status.setText(stopped); //changes the label text
    }

    else if(evt.getSource() == clear)
    {	    
      int pos = 0;
      Critter crit;

      //goes through all elements in the arraylist and removes them
      while(pos < critters.size())
      {
        crit = critters.get(pos);
	crit.kill();

	pos++;
      }

      critters.clear(); //clears the array list

      //sets the text
      if(isStarted)
      {
        status.setText(startedLess);
      }
      else if(isStopped)
      {
	status.setText(stopped);
      }
    }

    else if(evt.getSource() == chaser)
    {
      runnerChosen = false;
      randomChosen = false;
      imitatorChosen = false;

      chaserChosen = true; //sets boolean to create chaser

      instruction.setText(chaserSelected);    //sets the text 
    }

    else if(evt.getSource() == runner)
    {
      chaserChosen = false;
      randomChosen = false;
      imitatorChosen = false; 

      runnerChosen = true;  //sets boolean to create runner

      instruction.setText(runnerSelected);     //sets the text
    }

    else if(evt.getSource() == random)
    {
      chaserChosen = false;
      runnerChosen = false;
      imitatorChosen = false;

      randomChosen = true; //sets boolean to create random

      instruction.setText(randomSelected);   //sets the text           
    }

    else if(evt.getSource() == imitator)
    {
      chaserChosen = false;
      runnerChosen = false;
      randomChosen = false;
      
      imitatorChosen = true; //sets boolean to create imitator

      instruction.setText(imitatorSelected);  //sets the text      
    }    
  }

  /*
  * Name: mouseClicked
  * Purpose: Creates new critters at the location clicked.
  * Parameters: evt - the mouse event
  * Return: void
  */  
  @Override  
  public void mouseClicked(MouseEvent evt)
  {
    Location loc = new Location(evt.getX(), evt.getY()); 

    if(chaserChosen) //creates new chaser at the location
    {
      critters.add(new Chaser(loc, canvas));
      
      if(critters.size() <= 1 && isStarted ) //changes the text
      {
        status.setText(startedLess);
      }
      else if(critters.size() > 1 && isStarted)
      {
        status.setText(startedMore);
      }
    }
    else if(runnerChosen) //creates new runner at the location 
    {
      critters.add(new Runner(loc, canvas)); //changes the text

      if(critters.size() <= 1 && isStarted)
      {
        status.setText(startedLess);
      }
      else if(critters.size() > 1 && isStarted)
      {
        status.setText(startedMore);
      }      
    }
    else if(randomChosen) //creates new random at the location
    {
      critters.add(new Random(loc, canvas)); //changes the text
      
      if(critters.size() <= 1 && isStarted)
      {
        status.setText(startedLess);
      }
      else if(critters.size() > 1 && isStarted)
      {
        status.setText(startedMore);
      }      
    }
    else if(imitatorChosen) //creates new imitator at the location
    {
      critters.add(new Imitator(loc, canvas)); //changes the text

      if(critters.size() <= 1 && isStarted)
      {
        status.setText(startedLess);
      }
      else if(critters.size() > 1 && isStarted)
      {
        status.setText(startedMore);
      }      
    }
  }

  /*
  * Name: mousePressed
  * Purpose: To compile.
  * Parameters: evt - the mouse event
  * Return: void
  */ 
  @Override
  public void mousePressed(MouseEvent evt)
  {
  }

  /*
  * Name: mouseReleased
  * Purpose: To compile.
  * Parameters: evt - the mouse event
  * Return: void
  */   
  @Override
  public void mouseReleased(MouseEvent evt)
  {
  }

  /*
  * Name: mouseEntered
  * Purpose: To compile.
  * Parameters: evt - the mouse event
  * Return: void
  */   
  @Override
  public void mouseEntered(MouseEvent evt)
  {
  }

  /*
  * Name: mouseExited
  * Purpose: To compile.
  * Parameters: evt - the mouse event
  * Return: void
  */ 
  @Override  
  public void mouseExited(MouseEvent evt)
  {
  }  
}
