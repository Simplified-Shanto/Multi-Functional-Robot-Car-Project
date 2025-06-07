#include<Servo.h>
//Pin declaration for the 74HC595 shift register
#define latchPin 13  //Pin 12 of 74HC595 shift register
#define clockPin 4  //Pin 11 of the 74HC595 shift register
#define dataPin  2 //Pin 14 of the 74HC595 shift register

boolean extraPinState[8] = {0, 0, 0, 0, 0, 0, 0, 0}; //Initially all extra pins are turned low

/*We can use the following pins of the 74HC595 shift register as new digital output pins: 
 * Shift register pins                  New digital pin
 * ^^^^^^^^^^^^^^^^^^^                  ^^^^^^^^^^^^^^^
 *        15                                  14  
 *        1                                   15
 *        2                                   16
 *        3                                   17
 *        4                                   18
 *        5                                   19
 *        6                                   20
 *        7                                   21
 */        

#define commonEnablePin 3
//Pin declaration for left motor. 
#define in1  15  
#define in2  16

//Pin declaration for right motor. 
#define in3  17
#define in4  18
#define servoPin 12
#define trigPin 14
#define echoPin A0
Servo sensorServo;
Servo testServo; // We'll check the health of our servo's using this one. 


void setup() {
   pinMode(commonEnablePin, OUTPUT);
   analogWrite(commonEnablePin, 0); 
   
  pinMode(latchPin, OUTPUT); 
  pinMode(clockPin, OUTPUT); 
  pinMode(dataPin, OUTPUT); 

  //Pulling all extra pins to LOW initially
  digitalWrite(latchPin, LOW); 
  shiftOut(dataPin, clockPin, MSBFIRST, 0); 
  digitalWrite(latchPin, HIGH); 
  
  delay(100); 

  Serial.begin(9600); 
  sensorServo.attach(servoPin); 
  sensorServo.write(90); 

    //pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
   
  pinMode(A0, INPUT); //Echopin of ultrasonic sensor
   //IR array pins
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT); 
  pinMode(A3, INPUT); 
  pinMode(A4, INPUT); 
  pinMode(A5, INPUT); 

  testServo.attach(8); 
  for(int i = 0; i<=10; i++)
  {
    test(); 
  }
  
}


void test()
{
   for(int i = 0; i <= 180; i++){
    testServo.write(i); 
    delay(15); 
  }

  delay(1000); 

  for(int i = 180; i>=0; i-=30)
  {
    testServo.write(i); 
    delay(300); 
  }

  delay(1000); 
}
 
unsigned long preMillis = millis(); 

int actionValue = 150; 
int timeThreshold = 100; //After how much time we will call Stop(); 


String command = "a";   //Command received via Serial
char Action = 'a'; //Which action we will take. Example: Mode setup
String speedString = "x"; 
unsigned short carMode = 1; // 1 - Joystick driving mood
                            // 2- Obstacle avoidance mood
unsigned short middleDistance = 0; 
unsigned short rightDistance = 0; 
unsigned short leftDistance = 0; 
unsigned short oaSpeed = 90; //PWM value while driving in obstacle avoidance (OA) mode. 

/*
 * Control Characters
 * Forward - 'f'
 * Backward - 'b'
 * Left - 'l'
 * Right - 'r'
 * 
 * Mode setup - 'm' 
 * OA variables settings - '0' to last index of oaSettings array. 
 * 
 * Serial command print format - "Command:Value" for example: "f:120." means go forward with a pwm of 120
 */
 
 #define noSettings 5               //Number of Settings
     //   String oaSettings[noSettings] =  {"OA BF PWM", "OA BF time", "OA LR PWM" , "OA LR time", "Safe Distance"};   //The obstacle avoidance mode settings and values. 
 unsigned short  oaSettingsValue[noSettings] = {90,         250,           200,          200,          8 };     //Time in millisecond, Safe distance is in inch. 
                                      /*Index:  0             1             2             3             4*/
                                      
void loop() {
 
   if (Serial.available()) {
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

         default:
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
      middleDistance = calculateDistance(); 
// serialPrint('M', middleDistance); 

      if(middleDistance < oaSettingsValue[4]) //oaSettingsValue[4] = safeDistance
      {
       Stop(); 
       sensorServo.write(0);                //Moving US sensor to the right
       delay(1000);                          //Keeping the sensor steady for 1000 ms. 
       rightDistance = calculateDistance();  //returns distance in inch unit
     //  serialPrint('R', rightDistance);      //Sending the reading to the controller.
       delay(500); 
       
       sensorServo.write(180); //Moving ultrasonic sensor to the left
       delay(1000);       //Keeping the sensor steady on the servo to get a stable reading. 
       leftDistance = calculateDistance();  //Calculating left distance. 
      // serialPrint('L', leftDistance);      //Sending the data to the controller. 
       delay(500); 
       
 
       sensorServo.write(90);               //Taking back the sensor on the servo to front direction 
       delay(1500);                          //Having a short delay before going to newly found direction. 
      
    
        if(rightDistance >= oaSettingsValue[4] && rightDistance >= leftDistance)  //Whether right side has enough safe
       {
        goRight(oaSettingsValue[2]);  
        delay(oaSettingsValue[3]); 
       }
       else if(leftDistance >= oaSettingsValue[4])   //oaSettingsValue[4]  = safe distance. 
       {
          goLeft(oaSettingsValue[2]); 
          delay(oaSettingsValue[3]);
       }
      else{
        goBackward(oaSettingsValue[0]); 
        delay(oaSettingsValue[1]); 
       }
       
      } 
      else 
      {
        goForward(oaSettingsValue[0]); 
      }
    }
    
    
}

//digtalWrite() function for the new extra pins
void DigitalWrite(int extraPinNumber, int state)
{
  extraPinState[extraPinNumber - 14] = state; 
  /*Since there's already 13 pins in arduino UNO and we are counting the extra pins from 14....to 21 */
  unsigned short decimalValue  = 0; 
  for(int i = 0; i <= 7; i++)  //Converting the 8 bit bit pattern to decimalvalue
  { 
    decimalValue+= (ceil(pow(2, i))*extraPinState[i]); 
  }
  digitalWrite(latchPin, LOW); //Don't changing the pin state while sending data serially
  shiftOut(dataPin, clockPin, MSBFIRST, decimalValue); 

  digitalWrite(latchPin, HIGH); 
}

int calculateDistance(){ 
  
  DigitalWrite(trigPin, LOW); //Extra pin number 14
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  DigitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  DigitalWrite(trigPin, LOW);
 long duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  
  return duration*0.0068; //returns distance in inch. 
}


void Stop()
{
  analogWrite(commonEnablePin, 0); 
}

void rightForward(int actionValue) //Sets the right motor to move forward
{
  analogWrite(commonEnablePin, actionValue); 
  DigitalWrite(in3, HIGH); 
  DigitalWrite(in4, LOW); 
}

void rightBackward(int actionValue)
{
   analogWrite(commonEnablePin, actionValue);  
  DigitalWrite(in3, LOW); 
  DigitalWrite(in4, HIGH); 
}

void leftForward(int actionValue)
{
 analogWrite(commonEnablePin, actionValue); 
  DigitalWrite(15, HIGH); 
  DigitalWrite(16, LOW); 
  
}

void leftBackward(int actionValue)
{
   analogWrite(commonEnablePin, actionValue);  
  DigitalWrite(in1, LOW); 
  DigitalWrite(in2, HIGH); 
}


void goForward(int actionValue)
{
 rightForward(actionValue); 
 leftForward(actionValue); 
}

void goBackward(int actionValue)
{
  rightBackward(actionValue); 
  leftBackward(actionValue); 
}

void goLeft(int actionValue)
{
 rightForward(actionValue); 
 leftBackward(actionValue); 
}


void goRight(int actionValue)
{
    leftForward(actionValue); 
    rightBackward(actionValue); 
}
