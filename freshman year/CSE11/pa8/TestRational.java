/*
 * Tester for Rational, CSE 11 PA8 WI14
 */
public class TestRational
{
  private static final int MAXTESTS = 40;
  private static int failedTests = 0;


  /*
   * Test Driver
   */
  public static void main( String[] args )
  {
    System.out.println("\nRunning TestRational...\n\n");

    /*
     * Run tests for all the functions that are public.
     * You may comment out tests that you do not wish to run; however, to
     * receive full points you must pass all the tests.
     */
    testNoArgCtor();
    testOneArgCtor();
    testTwoArgCtor();
    testCopyCtor();
    testAdd();
    testSubtract();
    testMultiply();
    testDivide();
    testToFloat();
    testEquals();
    testHashCode();

    System.out.println("\n\n...Finished.\n");

    //results calculation
    System.out.print("RESULT: Total # of Tests Passed: "+ (MAXTESTS-failedTests)
		       + "/40");
    if(failedTests != 0)
      System.out.println(" -- FIX ME!!! :(");
    else
      System.out.println(" -- Yay! :)"); 
  }



  /*
   * Tests for no-argument constructor
   */
  private static void testNoArgCtor()
  {
    System.out.println("===== Testing No-Arg  Constructor =====");

    //Test 1
    System.out.print("  Test 1:  ");
    try
    {
      Rational r1 = new Rational();
      if( r1.toString().equals("0") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
	failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for one-argument constructor
   */
  private static void testOneArgCtor()
  {
    System.out.println("\n===== Testing One-Arg Constructor =====");

    //Test 2
    System.out.print("  Test 2:  ");
    try
    {	    
      Rational r2 = new Rational(0);
      if( r2.toString().equals("0") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
 

    //Test 3
    System.out.print("  Test 3:  ");
    try
    {	    
      Rational r3 = new Rational(3);
      if( r3.toString().equals("3") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 4
    System.out.print("  Test 4:  ");
    try
    {	    
      Rational r4 = new Rational(-1);
      if( r4.toString().equals("-1") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for two-argument constructor
   */ 
  private static void testTwoArgCtor()
  {
    System.out.println("\n===== Testing Two-Arg Constructor =====");

    //Test 5
    System.out.print("  Test 5:  ");
    try
    {	    
      Rational r5 = new Rational(1,1);
      if( r5.toString().equals("1") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 6
    System.out.print("  Test 6:  ");
    try
    {	    
      Rational r6 = new Rational(1,5);
      if( r6.toString().equals("1/5") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 7
    System.out.print("  Test 7:  ");
    try
    {	    
      Rational r7 = new Rational(5,2);
      if( r7.toString().equals("5/2") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 8
    System.out.print("  Test 8:  ");
    try
    {	    
      Rational r8 = new Rational(4,2);
      if( r8.toString().equals("2") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 9
    System.out.print("  Test 9:  ");
    try
    {	    
      Rational r9 = new Rational(-1,2);
      if( r9.toString().equals("-1/2") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 10
    System.out.print("  Test 10: ");
    try
    {	    
      Rational r10 = new Rational(1,-2);
      if( r10.toString().equals("-1/2") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }

      
    //Test 11
    System.out.print("  Test 11: ");
    try
    {	    
      Rational r11 = new Rational(-1,-2);
      if( r11.toString().equals("1/2") )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 12
    System.out.print("  Test 12: ");
    try
    {	    
      Rational r12 = new Rational(1,0);
      System.out.println("FAIL");
      failedTests++;
    }
    catch(IllegalArgumentException e)
    {
      System.out.println("PASS");
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for copy constructor
   */
  private static void testCopyCtor()
  {
    System.out.println("\n===== Testing   Copy  Constructor =====");

    //Test 13
    System.out.print("  Test 13: ");
    try
    {	    
      Rational r13a = new Rational(1,2);
      Rational r13b = new Rational(r13a);

      if(!r13a.equals(r13b))
        System.out.println("FAIL");
      else
      {
        if(r13a != r13b)
          System.out.println("PASS");
        else
        {
          System.out.println("FAIL");
          failedTests++;
	}
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for add()
   */
  private static void testAdd()
  {
    System.out.println("\n===== Testing     add()    method =====");

    //Test 14
    System.out.print("  Test 14: ");
    try
    {	    
      Rational r14a = new Rational(1,2);
      Rational r14b = new Rational(1,2);
      Rational r14res = r14a.add(r14b);

      if(r14res.toString().equals("1"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 15
    System.out.print("  Test 15: ");
    try
    {	    
      Rational r15a = new Rational(1,2);
      Rational r15b = new Rational(-1,2);
      Rational r15res = r15a.add(r15b);

      if(r15res.toString().equals("0"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 16
    System.out.print("  Test 16: ");
    try
    {	    
      Rational r16a = new Rational(1,2);
      Rational r16b = new Rational(4,3);
      Rational r16res = r16a.add(r16b);

      if(r16res.toString().equals("11/6"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 17
    System.out.print("  Test 17: ");
    try
    {	    
      Rational r17a = new Rational(-2,5);
      Rational r17b = new Rational(2,-5);
      Rational r17res = r17a.add(r17b);

      if(r17res.toString().equals("-4/5"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for subtract()
   */
  private static void testSubtract()
  {
    System.out.println("\n===== Testing  subtract()  method =====");


    //Test 18
    System.out.print("  Test 18: ");
    try
    {	    
      Rational r18a = new Rational(1,3);
      Rational r18b = new Rational(1,3);
      Rational r18res = r18a.subtract(r18b);

      if(r18res.toString().equals("0"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 19
    System.out.print("  Test 19: ");
    try
    {	    
      Rational r19a = new Rational(1,3);
      Rational r19b = new Rational(-1,3);
      Rational r19res = r19a.subtract(r19b);

      if(r19res.toString().equals("2/3"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 20
    System.out.print("  Test 20: ");
    try
    {	    
      Rational r20a = new Rational(-1,3);
      Rational r20b = new Rational(-1,3);
      Rational r20res = r20a.subtract(r20b);

      if(r20res.toString().equals("0"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 21
    System.out.print("  Test 21: ");
    try
    {	    
      Rational r21a = new Rational(1,2);
      Rational r21b = new Rational(2,3);
      Rational r21res = r21a.subtract(r21b);

      if(r21res.toString().equals("-1/6"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for multiply()
   */
  private static void testMultiply()
  {
    /*
     * Testing multiply()
     */ 
    System.out.println("\n===== Testing  multiply()  method =====");

    //Test 22
    System.out.print("  Test 22: ");
    try
    {	    
      Rational r22a = new Rational(1,2);
      Rational r22b = new Rational(1,2);
      Rational r22res = r22a.multiply(r22b);

      if(r22res.toString().equals("1/4"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 23
    System.out.print("  Test 23: ");
    try
    {	    
      Rational r23a = new Rational(1,2);
      Rational r23b = new Rational(1,-2);
      Rational r23res = r23a.multiply(r23b);

      if(r23res.toString().equals("-1/4"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 24
    System.out.print("  Test 24: ");
    try
    {	    
      Rational r24a = new Rational(-1,2);
      Rational r24b = new Rational(1,-2);
      Rational r24res = r24a.multiply(r24b);

      if(r24res.toString().equals("1/4"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 25
    System.out.print("  Test 25: ");
    try
    {	    
      Rational r25a = new Rational(2,4);
      Rational r25b = new Rational(3,5);
      Rational r25res = r25a.multiply(r25b);

      if(r25res.toString().equals("3/10"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 26
    System.out.print("  Test 26: ");
    try
    {	    
      Rational r26a = new Rational(0);
      Rational r26b = new Rational(9,9000);
      Rational r26res = r26a.multiply(r26b);

      if(r26res.toString().equals("0"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for divide()
   */
  private static void testDivide()
  {
    System.out.println("\n===== Testing   divide()   method =====");

    //Test 27
    System.out.print("  Test 27: ");
    try
    {	    
      Rational r27a = new Rational(1,2);
      Rational r27b = new Rational(1,2);
      Rational r27res = r27a.divide(r27b);

      if(r27res.toString().equals("1"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 28
    System.out.print("  Test 28: ");
    try
    {	    
      Rational r28a = new Rational(1,-2);
      Rational r28b = new Rational(-1,2);
      Rational r28res = r28a.divide(r28b);

      if(r28res.toString().equals("1"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 29
    System.out.print("  Test 29: ");
    try
    {	    
      Rational r29a = new Rational(2);
      Rational r29b = new Rational(2);
      Rational r29res = r29a.divide(r29b);

      if(r29res.toString().equals("1"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 30
    System.out.print("  Test 30: ");
    try
    {	    
      Rational r30a = new Rational(-3);
      Rational r30b = new Rational(5);
      Rational r30res = r30a.divide(r30b);

      if(r30res.toString().equals("-3/5"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 31
    System.out.print("  Test 31: ");
    try
    {	    
      Rational r31a = new Rational(-9000);
      Rational r31b = new Rational(0);
      Rational r31res = r31a.divide(r31b);

      System.out.println("FAIL");
      failedTests++;
    }
    catch(IllegalArgumentException e)
    {
      System.out.println("PASS");
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }

    //Test 32
    System.out.print("  Test 32: ");
    try
    {	    
      Rational r32a = new Rational(0);
      Rational r32b = new Rational(9000);
      Rational r32res = r32a.divide(r32b);

      if(r32res.toString().equals("0"))
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for toFloat()
   */
  private static void testToFloat()
  {
    System.out.println("\n===== Testing   toFloat()  method =====");

    //Test 33
    System.out.print("  Test 33: ");
    try
    {	    
      Rational r33 = new Rational(1,2);
      if((Math.abs(r33.toFloat() - 0.5)) < 0.5)
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }

    //Test 34
    System.out.print("  Test 34: ");
    try
    {	    
      Rational r34 = new Rational(1);
      if((Math.abs(r34.toFloat() - 1)) < 0.5)
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 35
    System.out.print("  Test 35: ");
    try
    {	    
      Rational r35 = new Rational(0);
      if((Math.abs(r35.toFloat() - 0)) < 0.5)
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 36
    System.out.print("  Test 36: ");
    try
    {	    
      Rational r36 = new Rational(-1,2);
      if((Math.abs(r36.toFloat() - (-0.5))) < 0.5)
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for equals()
   */
  private static void testEquals()
  {
    System.out.println("\n===== Testing   equals()   method =====");

    //Test 37
    System.out.print("  Test 37: ");
    try
    {
      Rational r37a = new Rational(1,2);
      Rational r37b = new Rational(1,2);

      if(r37a.equals(r37b))
      {
        if(r37a != r37b)
          System.out.println("PASS");
        else
        {
          System.out.println("FAIL");
          failedTests++;
        }
      }
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }

    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 38
    System.out.print("  Test 38: ");
    try
    {
      Rational r38a = new Rational(1,2);
      Rational r38b = new Rational(-1,2);

      if(!r38a.equals(r38b))
      {
        if(r38a != r38b)
          System.out.println("PASS");
        else
        {
          System.out.println("FAIL");
          failedTests++;
        }
      }
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }

    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }


    //Test 39
    System.out.print("  Test 39: ");
    try
    {
      Rational r39a = new Rational(10);
      Rational r39b = new Rational(20/2);

      if(r39a.equals(r39b))
      {
        if(r39a != r39b)
          System.out.println("PASS");
        else
        {
          System.out.println("FAIL");
          failedTests++;
        }
      }
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }

    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }



  /*
   * Tests for hashCode()
   */
  private static void testHashCode()
  {
    System.out.println("\n===== Testing  hashCode()  method =====");

    //Test 40
    System.out.print("  Test 40: ");
    try
    {	    
      Rational r40a = new Rational(1,1);
      Rational r40b = new Rational(1,1);

      if( r40a.hashCode() == r40b.hashCode() )
        System.out.println("PASS");
      else
      {
        System.out.println("FAIL");
        failedTests++;
      }
    }
    catch(Exception e)
    {
      System.out.println("EXCEPTION");
      failedTests++;
    }
  }

}
