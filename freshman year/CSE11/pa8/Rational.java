/* 
 * Name: Jamie Zhang
 * Login: cs11fmd
 * Date: November 25, 2014
 * File: Rational.java
 * Sources of Help: Christopher Cabreros (student), PA8 instructions
 * 
 * This program creates Rational objects that can be manipulated in various 
 * ways.
 */

/*
* Name: Rational
* Purpose: Creates Rational objects that can be manipulated (added, 
*          subtracted, multiplied, etc.).
*/ 
public class Rational
{
  private int num; //holds the value of the numerator
  private int denom; //holds the value of the denominator

  /* 
  * Name: Rational
  * Purpose: Creates a Rational object where the numerator is set to 0 and
  * the denominator is set to 1 by default.
  * Parameters: none
  */  
  public Rational()
  {
    //sets the numerator and denominator
    setNumerator(0);
    setDenominator(1);
  }

  /* 
  * Name: Rational
  * Purpose: Creates a Rational object where the numerator is set to the 
  * value passed as an actual argument and the denominator is set to 1 by 
  * default.
  * Parameters: num - the value of the numerator
  */ 
  public Rational(int num)
  {
    //sets the numerator and denominator	  
    setNumerator(num);
    setDenominator(1);
  }

  /* 
  * Name: Rational
  * Purpose: Creates a Rational object where both the numerator and 
  * denominator are set to the values passed as actual arguments and is in
  * reduced form.
  * Parameters: num - the value of the numerator
  *             denom - the value of the denominator
  */ 
  public Rational(int num, int denom)
  {
    //sets the numerator and denominator	  
    setNumerator(num);
    setDenominator(denom);
    reduce(); //puts the object in reduced form
  }

  /* 
  * Name: Rational
  * Purpose: Creates a Rational object where the numerator and denominator
  * are equal to the numerator and denominator of the Rational object passed
  * through as an actual argument.
  * Parameters: r - a Rational object to be copied
  */   
  public Rational(Rational r)
  {
    //sets the numerator and denominator	  
    setNumerator(r.getNumerator());
    setDenominator(r.getDenominator());
    reduce(); //puts the object in reduced form
  }

  /* 
  * Name: getNumerator
  * Purpose: Returns the numerator of the Rational object.
  * Parameters: none
  * Return: int
  */  
  private int getNumerator()
  {
    return num;
  }

  /* 
  * Name: getDenominator
  * Purpose: Returns the denominator of the Rational object.
  * Parameters: none
  * Return: int
  */    
  private int getDenominator()
  {
    return denom;
  }

  /* 
  * Name: setNumerator
  * Purpose: Sets the numerator.
  * Parameters: num - a value to set the numerator to
  * Return: void
  */    
  private void setNumerator(int num)
  {
    this.num = num;
  }

  /* 
  * Name: setDenominator
  * Purpose: Sets the denominator.
  * Parameters: num - a value to set the denominator to
  * Return: void
  */   
  private void setDenominator(int denom)
  {
    //sets the denominator if denom is not zero
    if(denom != 0)
    {
      this.denom = denom;
    }
    else
    {
      //throws an exception if the denominator is zero
      throw new IllegalArgumentException();
    }
  }

  /* 
  * Name: gcd
  * Purpose: Finds the greatest common divisor of two integers.
  * Parameters: x - an integer
  *             y - an integer
  * Return: int
  */    
  private int gcd(int x, int y) 
  {
    if(y == 0)
    {
      return x; //returns the greatest common divisor
    }
    else
    {
      return gcd(y, x % y); //recurses until the gcd is found
    }
  }

  /* 
  * Name: reduce
  * Purpose: Reduces the Rational object to its simplest form (ex. 2/4 -> 1/2).
  * Parameters: none
  * Return: void
  */    
  private void reduce()
  {
    //finds the gcd of the absolute value of the numerator and denominator
    int gcd = this.gcd(Math.abs(this.getNumerator()), 
		       Math.abs(this.getDenominator()));

    //if both numerator and denominator are negative
    if(this.getNumerator() < 0 && this.getDenominator() < 0)
    {
      //sets the numerator and denominator to positive, reduced values	    
      this.num = Math.abs(getNumerator() / gcd);
      this.denom = Math.abs(getDenominator() / gcd);
    }
    //if numerator is positive and denominator is negative
    else if(this.getNumerator() > 0 && this.getDenominator() < 0)
    {
      //moves the negative sign from the denominator to the numerator
      //sets the numerator and denominator to their reduced values	    
      this.num = -(Math.abs(getNumerator()/gcd));
      this.denom = Math.abs(getDenominator() / gcd);
    }
    //if the negative is already in the numerator or both numerator and 
    //denominator are positive
    else
    {
      //sets the numerator and denominator to their reduced values
      this.num = this.getNumerator() / gcd;
      this.denom = this.getDenominator() / gcd;
    }
  }

  /* 
  * Name: add
  * Purpose: Adds two Rational objects together and returns a new Rational 
  * object with the new value.
  * Parameters: r - a Rational object that is added (this + r)
  * Return: Rational
  */
  public Rational add(Rational r) 
  {
    //gets the common denominator
    int cd = this.getDenominator() * r.getDenominator();
    //finds the numerator
    int numer = this.getNumerator() * r.getDenominator() +
	        r.getNumerator() * this.getDenominator();
    
    Rational rat = new Rational(numer, cd); //new Rational object

    return rat;
  }
  
  /* 
  * Name: subtract
  * Purpose: Subtracts a Rational object from another Rational object and 
  * returns a new Rational object with the new value.
  * Parameters: r - a Rational object that is subtracted (this - r)
  * Return: Rational
  */  
  public Rational subtract(Rational r)
  {
    //finds the common denominator	  
    int cd = this.getDenominator() * r.getDenominator(); 
    //finds the numerator
    int numer = this.getNumerator() * r.getDenominator() -
	        r.getNumerator() * this.getDenominator();

    Rational rat = new Rational(numer, cd); //new Rational object

    return rat;
  }

  /* 
  * Name: multiply
  * Purpose: Multiplies two Rational objects and returns a new Rational object
  * with the new value.
  * Parameters: r - a Rational object that is multiplied (this * r)
  * Return: Rational
  */  
  public Rational multiply(Rational r) 
  {
    //finds the numerator
    int numerator = this.getNumerator() * r.getNumerator();
    //finds the denominator
    int denominator = this.getDenominator() * r.getDenominator();

    Rational rat = new Rational(numerator, denominator); //new Rational object

    return rat;
  }

  /* 
  * Name: divide
  * Purpose: Divides two Rational objects and returns a new Rational object
  * with the new value.
  * Parameters: r - a Rational object that is divided (this / r)
  * Return: Rational
  */   
  public Rational divide(Rational r)
  {
    //finds the numerator
    int numerator = this.getNumerator() * r.getDenominator();
    //finds the denominator
    int denominator = this.getDenominator() * r.getNumerator();

    Rational rat = new Rational(numerator, denominator); //new Rational object

    return rat;
  }

  /* 
  * Name: toString
  * Purpose: Returns the Rational object as a string.
  * Parameters: none
  * Return: String
  */   
  public String toString()
  {
    //if the denominator value is 1
    if(this.getDenominator() == 1)
    {
      return getNumerator() + ""; //return only the numerator as a string
    }
    else 
    {
      //returns numerator/denominator as a string
      return getNumerator() + "/" + getDenominator();
    }
  }

  /* 
  * Name: toFloat
  * Purpose: Returns the Rational object as a float.
  * Parameters: none
  * Return: float
  */     
  public float toFloat()
  {
    //returns numerator/denominator as a float
    return (float)(this.getNumerator()) / this.getDenominator();
  }

  /* 
  * Name: equals
  * Purpose: Checks if a Rational object is equal to another object.
  * Parameters: o - an object to compare to
  * Return: boolean
  */    
  @Override
  public boolean equals(Object o)
  {
    if(o == null) //if o is null
    {
      return false; //the two objects aren't equal
    } 
    else if(o.getClass() != this.getClass()) //if o is not the same class
    {
      return false; //the two objects aren't equal
    } 
    //if o does not have the same numerator or denominator
    else if(this.getNumerator() != ((Rational)(o)).getNumerator() || 
            this.getDenominator() != ((Rational)(o)).getDenominator())
    {
      return false; //the two objects aren't equal
    }
    //if the numerator and denominators of both objects are equal
    else if(this.getNumerator() == ((Rational)(o)).getNumerator() &&
            this.getDenominator() == ((Rational)(o)).getDenominator())
    {
      return true; //the two objects are equal
    }
    else
    {
      return false; //returns false if no cases match
    }
  }

  /* 
  * Name: hashcode
  * Purpose: Returns a hashcode.
  * Parameters: none
  * Return: int
  */     
  @Override
  public int hashCode()
  {
    return this.toString().hashCode();
  }

  /* 
  * Name: reciprocal
  * Purpose: Flips the numerator and denominator and returns a new Rational
  * object with those values.
  * Parameters: none
  * Return: Rational
  */     
  public Rational reciprocal()
  {
    if(this.getNumerator() == 0) //if the numerator is 0
    {
      //throws an exception because the denominator can't be 0
      throw new UnsupportedOperationException();
    }
    else
    {
      //creates a new object and flips the numerator and denominator
      Rational rat = new Rational(this.getDenominator(), this.getNumerator());

      return rat;
    }
  }

  /* 
  * Name: floor
  * Purpose: Changes a Rational into a whole number rounding down and returns
  * that value as a Rational object.
  * Parameters: none
  * Return: Rational
  */   
  public Rational floor()
  {
    int numerator;

    //if one out of the two values are negative
    if(this.getNumerator() < 0 && this.getDenominator() > 0 || 
       this.getNumerator() > 0 && this.getDenominator() < 0)
    {
      //divides to get a whole number rounded up
      numerator = this.getNumerator() / this.getDenominator();
      numerator = numerator - 1; //subtract one to get the value rounded down
    }
    //if both values are negative or both positive
    else
    {
      //divides to get a whole number rounded down	    
      numerator = this.getNumerator() / this.getDenominator();
    }

    Rational rat = new Rational(numerator); //new Rational object

    return rat;    
  }

  /* 
  * Name: ceiling
  * Purpose: Changes a Rational into a whole number rounding up and returns
  * that value as a Rational object.
  * Parameters: none
  * Return: Rational
  */     
  public Rational ceiling()
  {
    int numerator;

    //if dividing the two numbers does not give a remainder
    if((this.getNumerator() % this.getDenominator()) == 0) 
    {
      //returns that whole number	    
      numerator = this.getNumerator() / this.getDenominator();

      Rational rat = new Rational(numerator); //new Rational object

      return rat;
    }
    //if dividing the two numbers gives a remainder
    else
    {
      //if one of the two numbers is negative	    
      if(this.getNumerator() < 0 && this.getDenominator() > 0 || 
       this.getNumerator() > 0 && this.getDenominator() < 0)
      {
	//divides to get a whole number rounded up       
        numerator = this.getNumerator() / this.getDenominator();
      }
      //if both numbers are positive or both are negative
      else
      {
	//divides to get a whole number rounded down (so add one to round up)
        numerator = this.getNumerator() / this.getDenominator() + 1;
      }

      Rational rat = new Rational(numerator); //new Rational object

      return rat;
    }
  }
 
} //end class Rational
