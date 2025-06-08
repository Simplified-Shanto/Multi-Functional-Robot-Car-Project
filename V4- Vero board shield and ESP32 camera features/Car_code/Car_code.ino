//See CodeGuidance.h for detailed comments on required topics. 
//#include <iostream>
#include <SPI.h>  
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include <SoftwareSerial.h>

#define SCREEN_WIDTH  128 //OLED display width, in pixels. 
#define SCREEN_HEIGHT 64  //OLED display height, in pixels.
//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 

const byte rxPin = A3;
const byte txPin = A2;

// Set up a new SoftwareSerial object
SoftwareSerial MC2 (rxPin, txPin); //MC2 means the other microcontroller onboard. 


#include<Servo.h>
#include "Shift_Register.h"
#include "WheelMotors.h"
#include "RoboticArm.h"
#include "ObstacleAvoidance.h"
#include "Sensors.h"

#define SonarSensorAttached 1 
//Whether ultrasonic sensor is currently attached
                              //with the vehicle, otherwise we'll disable all 
                              //functionalaties related to it. 


void setup() {
  pinMode(latchPin, OUTPUT); 
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT); 

  //Pulling all extra pins to LOW initially
  digitalWrite(latchPin, LOW); 
  shiftOut(dataPin, clockPin, MSBFIRST, 0); 
  digitalWrite(latchPin, HIGH); 

  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //Address of our 128x64 display is 0x3C
  {
    //Serial.println(F("SSD1306 allocation failed"));  //By wrapping the output string inside F() we store the string in the program flash memory instread of SRAM. 
   // for(;;); //Keep looping forever. 
  }
   delay(2000); //waiting for the display to set up itself 

  OLED.clearDisplay(); 

  OLED.setTextSize(1); 
  OLED.setTextColor(WHITE); 
  OLED.setCursor(0,0); 
  //DISPLAY static text
  OLED.println("Hi! :)"); 
  OLED.display(); 
   
   delay(4000); 

  //Switching pin of sensor shield. 
  pinMode(sensorPowerPin, OUTPUT); 
 digitalWrite(sensorPowerPin, LOW); 
  //Declaring 4 wheel's motor's common enable pin and setting it to 0 (In range 0 to 255)
   pinMode(commonEnablePin, OUTPUT);
   analogWrite(commonEnablePin, 0); 

  // //Initializing the LCD display connected via I2C backpack 
  //    lcd.init(); 
  // lcd.backlight(); 
  // lcd.clear(); 
  // lcd.setCursor(0,0); 
  // lcd.print("Setup started. "); 

 //PWM pins in arduino Uno:  3, 5, 6, 9, 10, 11
  //Handling the servo motors
  sensorServo.attach(SensorServoPin); 
  sensorServo.write(90); 
  armServo1.attach(9); 
  armServo2Left.attach(10); 
  armServo2Right.attach(11); 
 // armServo3.attach(5); 
  armServo4.attach(6); //6th port of the servo power module. 
 // armServo5.attach(7); 
  

  initializeRoboticArm(); //Moves different servo motors to their desired
                           //initial angle. 
  delay(100); 

  Serial.begin(9600); 
  MC2.begin(9600); 

  /*Setting up sensors*/

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

// lcd.setCursor(0,0); 
// lcd.clear(); 
// lcd.print("Setup done."); 
// lcd.setCursor(1, 0); 
// lcd.print("Entering loop()"); 
  OLED.clearDisplay(); 
  OLED.setCursor(0,0); 
  OLED.println("Setup finished!"); 
  OLED.display(); 
  delay(500); 

}


unsigned long preMillis = millis(); 

int actionValue = 150; 
int timeThreshold = 100; //After how much time we will call Stop(); 


String command = "a";   //Command received via Serial
char Action = 'a'; //Which action we will take. Example: Mode setup
String speedString = "x"; 
unsigned short carMode = 1; // 1 - Joystick driving mood
                            // 2- Obstacle avoidance mood


 
void loop()
{
 
           if (Serial.available())
           {
                command = Serial.readStringUntil('.');  
                OLED.clearDisplay(); 
                OLED.println(command); 
                OLED.display(); 
                Action = command[0]; //The type of action the remote wants us to take. 
                speedString = command.substring(2, command.length()); 
                actionValue = speedString.toInt();   
                preMillis = millis(); 
        
                        switch(Action) //Which action we are going to take. 
                        {
                              case 'f': 
                              if(carMode==1){ 
                                if(SonarSensorAttached == 1) 
                                {
                                  if(calculateDistance()>3)
                                  {
                                     goForward(actionValue); 
                                     timeThreshold = 100; 
                                  }
                                }
                                else  //if the sonar sensor is not attached we'll not go for checking distance. 
                                {      
                             goForward(actionValue); 
                              timeThreshold = 100; 
                                }
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
                                if(carMode==3)
                                {   
                                    //Send command to MC2 to power up sensor shield and start sending readings; 
                                    MC2.print("1"); 

                                    OLED.clearDisplay(); 
                                    OLED.println("MC2 -> 1"); 
                                    OLED.display(); 
                                    
                                }
                                else 
                                {
                                    // MC2.print("0"); 
                                    // lcd.home(); 
                                    // lcd.print("MC2 -> 0"); 
                                }
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

            if(carMode==3)
            {
                if(MC2.isListening())
                {

                if(MC2.available())
                {
                 // lcd.print("MC2 is available()");
                String sensorReadings = MC2.readStringUntil('!'); 
                OLED.clearDisplay(); 
                OLED.println(sensorReadings); 
                OLED.display(); 
                }
               
                }
                else
                {
                  OLED.clearDisplay(); 
                  OLED.println("MC2 is not listening. "); 
                  OLED.display(); 
                }

            }
            
   
    
}

// void clearDisplay()
// {
//                   lcd.home(); 
//                   lcd.print("                "); 
//                   lcd.home(); 
// }
