/*Written by: Nayeem Islam Shanto (islamshafiul283@gmail.com)
 *Intended board: Arduino Uno.
 * 
 *Modification start: 15/10/2023
 *Modification end: 
 *
 *Modifications done: 
 ** Second joint from base will contain 180degree servo whose positions
 * will be tuned by a arraylist.
 *
 *
 *Modifications planned: 
 *
 *Give two 2 servos o the first joint actual PWM pin thus they 
 *don't go to random position on startup. 
 *
 *  Updating comments according to current configurations. 
 *Writing the code neatly in separte files. 
 *
 *
 *
 * Spare Motor channel will be assigned to the mini gear motor for the 
 * joint before the claw. 
 * 
  
 * !!!!!!Check whether the code fits to the memory size with the angles - Checked. 
 * 
 */

#include<Servo.h>
//Pin declaration for the 74HC595 shift register
#define latchPin 8  //Pin 12 of 74HC595 shift register
#define clockPin 4  //Pin 11 of the 74HC595 shift register
#define dataPin  2 //Pin 14 of the 74HC595 shift register


/*The two servo motors used for the first joint 
 * doesn't align for some pair of angles. 
 * So we have modified the angle values to 
 * achieve the goal. 
*/
const byte leftAngles[177] = {2 ,    3 ,   4 ,   5 ,   6 ,   7 ,   8 ,   9 ,   10 , 11 ,   12 ,  13 ,  14 , 15 ,  16 ,  17 ,  18 ,  19 ,  20 ,  21 ,   22 , 23 ,   24 , 25 ,   26 ,  27 , 28 ,   29 ,  30 ,  31 ,  32 ,  33 ,  34 ,  35 ,  36 ,  37 , 38 ,  39 ,  40 ,  41 ,  42 ,  43 ,  44 ,  45 ,  46 ,  47 ,  48 ,   49 ,  50 ,  51 ,  52 ,  53 ,  54 , 55 ,  56 ,  57 ,  58 ,  59 ,  60 ,  61 ,  62 ,  63 ,   64 ,  65 , 66 ,  67 ,  68 ,  69 ,  70 ,  71 ,  72 ,  73 ,  74 ,  75 ,  76 ,  77 ,  78 ,  79 ,  80 ,  81 , 82 , 83 , 84 , 85 , 86 , 87 , 88 , 89 , 90 , 90 , 91 , 92 , 93 , 94 , 95 , 96 , 97 , 98 , 99 , 100 , 101 , 102 , 103 , 104 , 105 ,  106 , 107 ,  107, 108 , 109 , 110 , 111 , 112 , 113 , 114 , 115 , 116 , 117 , 118 , 119 , 120 , 121 , 122 , 123 , 124 , 125 , 125 , 126 , 127 , 128 , 129 , 130 , 131 , 132 , 133 ,133  , 134,  134 , 135 , 136 , 137 , 138 , 139 , 140 , 141 , 142 , 143 , 144 , 145 , 146 , 147 , 148 , 149 , 150 , 151 , 152 , 153 , 154 , 155 , 156 , 157 , 158 , 159 , 160 , 161 , 162 , 163 , 164 , 165 , 166 , 167, 168 , 169  , 170 , 171 , 172 , 173  };
const byte rightAngles[177] ={178 , 177 , 176 , 175 , 174 , 173 , 172 , 171 , 170 , 169 , 168 , 167 , 166 , 165 , 164 , 163 , 162 , 161 , 160 , 159 , 158 , 157 , 156 , 155 , 154 , 153 , 152 , 151 , 150 , 149 , 148 , 147 , 146 , 145 , 144 , 143 , 142 , 141 , 140 , 139 , 138 , 137 , 136 , 135 , 134 , 133 , 132 , 131 , 130 , 129 , 128 , 127 , 126 , 125 , 124 , 123 , 122 , 121 , 120 , 119 , 118 , 117 , 116 , 115 , 114 , 113 , 112 , 111 , 110 , 109 , 108 , 107 , 106 , 105 , 104 , 103 , 102 , 101 , 100 , 99 , 98 , 97 , 96 , 95 , 94 , 93 , 92 , 91 , 90 , 89 , 88 , 87 , 86 , 85 , 84 , 83 , 82 , 81 , 80 ,  79 ,  78 ,   77 , 76 ,  75 ,  74 ,   73 ,  72 ,  71 ,  70 ,  69 ,  68 ,  67 , 66  , 65  , 64  , 63  , 62  , 61  , 60  , 59  , 58  , 57  , 56  , 55  , 54  , 53  , 52  , 51  , 50  , 49  , 48  , 47  , 46  , 45  , 44  , 43  , 42  , 41  , 40  , 39  , 38  , 37  , 36  , 35  , 34  , 33  , 32  , 31  , 30  , 29  , 28  , 27  , 26 ,  25 ,  24  , 23  , 22  , 21  , 20 ,  19 ,  18  , 17 ,  16   , 15 , 14 ,   13 , 12  , 11 ,   10 ,   9 ,  8 ,  7 ,   6  ,   5 ,   4 ,   3 ,   2  };
byte anglePairIndex = 0; //Index of which angle is being written in the motor according 
                         //the above two lists. 

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
#define in1   16
#define in2  19

//Pin declaration for right motor. 
#define in3  17
#define in4  20

//Pin declaration for the gripper motor. 
#define gripperIn1 14
#define gripperIn2 18

//Pin declaration for the unused 1 motor of the second l293d motor driver. 
#define wristMotorIn3 15
#define wristMotorIn4 21

#define servoPin 12

#define trigPin 13
#define echoPin A0

//PWM pins of arduino Uno -   3, 5, 6, 9, 10, 11

Servo sensorServo;  //Servomotor carrying the sonar sensor.
Servo armServo1; //Continuous rotation Servomotor at the base of the robotic arm
Servo armServo2Left; //Continuous rotation Left servo of the second joint from the bottom. 
Servo armServo2Right; //Continuous rotation Right servo for the second joint from the bottom. 
Servo armServo3; //The fourth servo motor from the base of the robotic arm
Servo armServo4; //The fifth servo motor from the base of the robotic arm
Servo armServo5; //The sixth servo motor from the base of the robotic arm

 byte servoPositions[6] = {90, leftAngles[anglePairIndex], rightAngles[anglePairIndex], 90, 90, 90}; //Initial positions of the servo motors used in the arm
                                                 //armServo1, armServo3 and armServo4 are continuous rotation, so we set 90 degree (stop). 

#define angleChangeDelay  5
#define CRSruntime   30    //The time period for which we will run (backward/forward) the Continous Rotation Servo (CSR) for 1 press. 
#define CRSspeed 15  //In range 0 to 90


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
   sensorServo.attach(servoPin); 
  sensorServo.write(90); 
  armServo1.attach(9); 
  armServo2Left.attach(10); 
  armServo2Right.attach(11); 
  armServo3.attach(5); 
  armServo4.attach(6); 
  armServo5.attach(7); 
  

  
                                        //Setting the servo's to their initial positions. 
                  unsigned short moveGap = 700; //The gap between writing initial angles of each successive servos
    armServo1.write(servoPositions[0]); 
    //delay(moveGap); 
    armServo2Left.write(servoPositions[1]); 
    armServo2Right.write(servoPositions[2]); 
    delay(moveGap); 
    armServo3.write(servoPositions[2]);
    delay(moveGap);  
    armServo4.write(servoPositions[3]); 
    delay(moveGap); 
    armServo5.write(servoPositions[4]); 
    delay(moveGap); 
    
   
  
  
  delay(100); 

  Serial.begin(9600); 

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input

   //IR array pins
  pinMode(A1, INPUT); 
  pinMode(A2, INPUT); 
  pinMode(A3, INPUT); 
  pinMode(A4, INPUT); 
  pinMode(A5, INPUT); 

  
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
          
         if(actionValue==1){     armServo3.write(CRSspeed+90);       }  //moving in the forward direction. 
         else { armServo3.write(90-CRSspeed); }  //moving in the backward direction
         delay(CRSruntime);   //for a short time. 
         armServo3.write(90); //then stopping the motor from rotating.

        }
        break; 

        case 'x': //Fourth servo counting from the base of the arm. 
        {
          /*  //Currently there's no motor as the fourth motor. 
           if(actionValue==1){     armServo4.write(CRSspeed+90);       }  //moving in the forward direction. 
         else { armServo4.write(90-CRSspeed); }  //moving in the backward direction
         delay(CRSruntime);   //for a short time. 
         armServo4.write(90); //then stopping the motor from rotating. 
         */
        }
        break; 

        case 'y': //Fifth servo motor. 
        {
      
           if(actionValue == 1)
          {
            DigitalWrite(wristMotorIn3, HIGH); 
            DigitalWrite(wristMotorIn4, LOW); 
            delay(10); 
            DigitalWrite(wristMotorIn3, LOW); 
          }
          else if(actionValue == -1)
          {
            DigitalWrite(wristMotorIn3, LOW); 
            DigitalWrite(wristMotorIn4, HIGH); 
            delay(10); 
            DigitalWrite(wristMotorIn4, LOW); 
          }
        
        }
        break;

         case 'z': //The gripper mechanism is implemented using gear motor. 
        {
            if(actionValue == 1)
          {
            DigitalWrite(gripperIn1, HIGH); 
            DigitalWrite(gripperIn2, LOW); 
            delay(10); 
            DigitalWrite(gripperIn1, LOW); 
          }
          else if(actionValue == -1)
          {
            DigitalWrite(gripperIn1, LOW); 
            DigitalWrite(gripperIn2, HIGH); 
            delay(10); 
            DigitalWrite(gripperIn2, LOW); 
          }
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
  
  digitalWrite(trigPin, LOW); //Extra pin number 14
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH); 
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
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
  DigitalWrite(in1, HIGH); 
  DigitalWrite(in2, LOW); 
  
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
