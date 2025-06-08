//#include"ObstacleAvoidance.h"

#define leftIRpin A5
#define middleIRpin A4
#define rightIRpin A3
#define leftIRvalue analogRead(leftIRpin)
#define middleIRvalue analogRead(middleIRpin)
#define rightIRvalue analogRead(rightIRpin)

#define leftThreshold 30
#define middleThreshold 30
#define rightThreshold  20
/*
                  Black                     Threshold                          White
Left           70 - 75 -  80                    100                        110 - 125 -  140
Middle         80 - 85 -  90                    103                        110 - 120 -  130
Right          65   70 -  75                    85                         90  - 100 -  105                        


*/

//Black value range 
//White value range

#define forwardPwm 200
#define leftRightPwm 250


//We'll try while loop instead of if else. 

/*
if all the sensor faces white then the car will not move and 
indicate it with the sensor servo moving a bit. 

*/
void followLine()
{
  
   if(leftIRvalue < leftThreshold)
      {
          while(middleIRvalue > middleThreshold) //Once the left sensor is on the black line, move until the car is aligned perfectly. 
          {
            goLeft(leftRightPwm); 
          }
      // delay(wait); 
      }
      else if(rightIRvalue < rightThreshold)
      {
          while(middleIRvalue > middleThreshold) //Once the right sensor is on the black line, move until the car is aligned perfectly. 
          {
          goRight(leftRightPwm);
          }
      // delay(wait); 
      }
      else
      {
          goForward(forwardPwm); 
      }

}
