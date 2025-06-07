#include<Servo.h>
//Pin declaration for left motor. 
#define enL 6 
#define in1 12
#define in2 11

//Pin declaration for right motor. 
#define enR  5 
#define in3  7 
#define in4  8

#define servoPin 9
#define trigPin A0
#define echoPin A1

Servo sensorServo;


void setup() {
  Serial.begin(9600); 
  sensorServo.attach(servoPin); 
  sensorServo.write(90); 
  

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  
  pinMode(enL, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 

  pinMode(enR, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

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
 unsigned short  oaSettingsValue[noSettings] = {90,         300,           200,          150,          6 };     //Time in millisecond, Safe distance is in inch. 
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
        timeThreshold = 20; 
        }
        break; 
        
        case 'r': //Right direction
        if(carMode==1){
        goRight(actionValue); 
        timeThreshold = 20;   //Turning at the half speed of going
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
 serialPrint('M', middleDistance); 

      if(middleDistance <= oaSettingsValue[4]) //oaSettingsValue[4] = safeDistance
      {
       Stop(); 
       sensorServo.write(10);                //Moving US sensor to the right
       delay(1000);                          //Keeping the sensor steady for 500 ms. 
       rightDistance = calculateDistance();  //returns distance in inch unit
       serialPrint('R', rightDistance);      //Sending the reading to the controller.
       
       
       sensorServo.write(90);                //Moving ultrasonic sensor to the middle
       delay(1000);                          //Keeping the sensor steady on the servo to get a stable reading. 
       middleDistance = calculateDistance(); //Calculating front distance in inch unit
       serialPrint('M', middleDistance);     //Sending the front distance reading to the controller. 
       
       
       sensorServo.write(170); //Moving ultrasonic sensor to the left
       delay(1000);       //Keeping the sensor steady on the servo to get a stable reading. 
       leftDistance = calculateDistance();  //Calculating left distance. 
       serialPrint('L', leftDistance);      //Sending the data to the controller. 
       
 
       sensorServo.write(90);               //Taking back the sensor on the servo to front direction 
       delay(500);                          //Having a short delay before going to newly found direction. 


       if(middleDistance > oaSettingsValue[4]) //oaSettingsValue[4] = safe distance . Checking whether we can proceed in the front direction 
       {
        goForward(oaSettingsValue[2]); 
       // delay(50); 
       }
       else if(rightDistance >= oaSettingsValue[4] && rightDistance >= leftDistance)  //Whether right side has enough safe
       {
        goRight(oaSettingsValue[2]);  
        delay(oaSettingsValue[3]); 
       }
       else if(leftDistance >= oaSettingsValue[4]) 
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

void serialPrint(char c, int v) //for printing message to the Seiral Monitor in the format 'action:value.'
{
    Serial.print(c); 
    Serial.print(':'); 
    Serial.print(v); 
    Serial.print('.'); 
}


int calculateDistance(){ 
  
  digitalWrite(trigPin, LOW); 
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
  digitalWrite(enL, LOW); 
  digitalWrite(enR, LOW); 
}

void rightForward(int actionValue) //Sets the right motor to move forward
{
  analogWrite(enR, actionValue); 
   digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW); 
}

void rightBackward(int actionValue)
{
  analogWrite(enR, actionValue); 
   digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH); 
}

void leftForward(int actionValue)
{
analogWrite(enL, actionValue); 
   digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW); 
  
}

void leftBackward(int actionValue)
{
  analogWrite(enL, actionValue); 
   digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
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
