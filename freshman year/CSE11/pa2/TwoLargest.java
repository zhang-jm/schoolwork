import java.util.Scanner;

public class TwoLargest {

  public static void main(String [] args) {
    System.out.println("Enter a series of integers; EOF to Quit.");

    int largestNumber = 0; //holding
    int secondLargestNumber = 0;
    int secondHolding = 0;
    int lastInputtedNumber = 0;
    int inputs = 0;

    Scanner input = new Scanner(System.in);

    while (input.hasNext()) {
      int number = input.nextInt();
      if(number != lastInputtedNumber || inputs == 0 && number == 0) {
        inputs++;
      }
      lastInputtedNumber = number;

      if(inputs == 1) {
        largestNumber = number;
      }

      else if(inputs == 2) {
        if(number < largestNumber) {
          secondLargestNumber = number;
        }
        else if(number > largestNumber) {
          secondLargestNumber = largestNumber;
          largestNumber = number;
        }
      }

      else if(inputs > 2) {
        if(number > largestNumber){
          secondLargestNumber = largestNumber;
          largestNumber = number;
        }
        else if (number < largestNumber) {
          if (number > secondLargestNumber) {
            secondLargestNumber = number;
          }
        }
      }
    }

    if(inputs == 0) {
      System.out.println("No numbers entered.");
    }

    else if(inputs == 1) {
      System.out.println("Largest distinct number entered was " + largestNumber);
    }

    else if(inputs > 1) {
      System.out.println("Largest distinct number entered was " + largestNumber);
      System.out.println("Second largest distinct number entered was " + secondLargestNumber);
    }
  }
} //end class TwoLargest


