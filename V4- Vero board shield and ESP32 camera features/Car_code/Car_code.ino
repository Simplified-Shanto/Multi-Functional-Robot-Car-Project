/*Written by: Nayeem Islam Shanto (islamshafiul283@gmail.com)
 *Intended board: Arduino Uno.
 *Controller: MI2 AI2 built custom Mobile App. 
 * 
 *Modification start: 15/10/2023
 *Modification end: 18/10/2023
 *
 * What's new in this version? 
 * 
 * 1.Second joint from base will contain 180degree servo whose positions
 *  will be tuned by a arraylist
 * 2. The code has been divided into separate header files 
 *    for managing and developing easily. 
 *
 * 
  
 * 
 */

#include<Servo.h>
#include "Shift_Register.h"
#include "WheelMotors.h"
#include "RoboticArm.h"
#include "ObstacleAvoidance.h"


void setup() {
  pinMode(latchPin, OUTPUT); 
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT); 

  //Pulling all extra pins to LOW initially
  digitalWrite(latchPin, LOW); 
  shiftOut(dataPin, clockPin, MSBFIRST, 0); 
  digitalWrite(latchPin, HIGH); 

  //Declaring 4 wheel's motor's common enable pin and setting it to 0 (In range 0 to 255)
   pinMode(commonEnablePin, OUTPUT);
   analogWrite(commonEnablePin, 0); 

 //PWM pins in arduino Uno:  3, 5, 6, 9, 10, 11
  //Handling the servo motors
  sensorServo.attach(SensorServoPin); 
  sensorServo.write(90); 
  armServo1.attach(9); 
  armServo2Left.attach(10); 
  armServo2Right.attach(11); 
  armServo3.attach(5); 
  armServo4.attach(6); //6th port of the servo power module. 
  armServo5.attach(7); 
  

  initializeRoboticArm(); 
  delay(100); 

  Serial.begin(9600); 

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

  //IR array pin declarations can be found in NotInUseFunctionalities. 
}


unsigned long preMillis = millis(); 

int actionValue = 150; 
int timeThreshold = 100; //After how much time we will call Stop(); 


String command = "a";   //Command received via Serial
char Action = 'a'; //Which action we will take. Example: Mode setup
String speedString = "x"; 
unsigned short carMode = 1; // 1 - Joystick driving mood
                            // 2- Obstacle avoidance mood


/*
 * Control Characters
 * Forward - 'f'
 * Backward - 'b'
 * Left - 'l'
 * Right - 'r'
 * 
 * 5 servos on the robotic arm (considering 2 cont. rot. servos on the second  
 * joint as a single one) starting from the first one at the base:  'u', 'v' 'w', 'x', 'y'
 *
 * Servo positions are changed commands "character:1." for increment" and "character:-1." for decrement
 * Here character = u, v, w, x, y  //v controls 2 servo motors
 * Gripper is controlled with character 'z'
 * 
 * Mode setup - 'm' 
 * OA variables settings - '0' to last index of oaSettings array. 
 * 
 * Serial command print format - "Command:Value" for example: "f:120." means go forward with a pwm of 120
 *                                                            "u:120." means to set the base servo to 120 degree. 
 *                                                            "m:1." means to set the car to mode 1 or driving mode. 
 *                                                            "1:120." means set oaSettings[1] to 120
 */
 
 
void loop()
{
 
           if (Serial.available())
           {
                command = Serial.readStringUntil('.');  
                Action = command[0]; //The type of action the remote wants us to take. 
                speedString = command.substring(2, command.length()); 
                actionValue = speedString.toInt();   
                preMillis = millis(); 
        
                        switch(Action) //Which action we are going to take. 
                        {
                              case 'f': 
                              if(carMode==1){        
                             goForward(actionValue); 
                              timeThreshold = 100; 
                              }
                              break; 
                              
                              case 'b': 
                              if(carMode==1){
                              goBackward(actionValue); 
                              timeThreshold = 100; 
                              }
                              break; 
                              case 'l': //Left direction
                              if(carMode==1){
                              goLeft(actionValue);  //Turning half at a speed of going. 
                              timeThreshold = 60; 
                              }
                              break; 
                              
                              case 'r': //Right direction
                              if(carMode==1){
                              goRight(actionValue); 
                              timeThreshold = 60;   //Turning at the half speed of going
                              }
                              break; 
                      
                              case 'm': //Mode setup 
                              {
                                Stop(); 
                                sensorServo.write(90); //Making the sensor front facing in the beginning of all modes
                                delay(1000); 
                                carMode = actionValue; 
                              }
                              break; 
                      
                              case 'u': //The base continuous rotation servo of the arm
                              {
                               if(actionValue==1){     armServo1.write(CRSspeed+90);       }  //moving in the forward direction. 
                               else { armServo1.write(90-CRSspeed); }  //moving in the backward direction
                               delay(CRSruntime);   //for a short time. 
                               armServo1.write(90); //then stopping the motor from rotating. 
                              }
                              break; 
                      
                              case 'v': //The left and right CSR from the base of the arm. 
                              {
                                  if(abs(actionValue) ==1 && anglePairIndex+actionValue>=0 && anglePairIndex+actionValue <= 176)
                                  {
                                    anglePairIndex+=actionValue; 
                                    armServo2Left.write(leftAngles[anglePairIndex]); 
                                    armServo2Right.write(rightAngles[anglePairIndex]); 
                                   delay(10);  
                                  }         
                              }
                              break; 
                      
                              case 'w': //Third CR servo counting from the base of the arm. 
                              {
                               if(actionValue==1){     armServo4.write(CRSspeed+90);       }  //moving in the forward direction. 
                               else { armServo4.write(90-CRSspeed); }  //moving in the backward direction
                               delay(CRSruntime);   //for a short time. 
                               armServo4.write(90); //then stopping the motor from rotating.
                                
                              }
                              break; 
                      
                              case 'x': //Fourth servo counting from the base of the arm. 
                              {
                                
                               
                              }
                              break; 
                      
                              case 'y': //The wrist of the arm. Mini metal gear motor is used.  
                              {
                                 moveWrist(actionValue); 
                              }
                              break;
                      
                               case 'z': //The gripper mechanism is implemented using gear motor. 
                              {
                                 moveGripper(actionValue); 
                              }
                              break;
                              
                               default: //"c:val." here c comes as 1,2,3,.... indicating the index of the changed OA settings. 
                               {
                                   oaSettingsValue[Action - '0'] = actionValue; 
                               }     
                        } 
           }
           else 
           {
                        if(carMode==1 && millis() - preMillis > timeThreshold) //We will stop only when in mode-1 joystick driving mode. 
                        {
                          Stop(); 
                          preMillis = millis(); 
                        }
           }

             if(carMode==2) //Whether car is in collission avoidance mode. 
            {
               avoidObstacles();
            }
            
   
    
}
