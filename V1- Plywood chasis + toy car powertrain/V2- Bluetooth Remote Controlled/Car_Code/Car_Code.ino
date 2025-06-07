//Pin declaration for left motor. 
const int enL = 10; 
const int in1 = 12; 
const int in2 = 11; 

//Pin declaration for right motor. 
const int enR = 9; 
const int in3 = 7; 
const int in4 = 8; 

void setup() {
  Serial.begin(9600); 
  
  pinMode(enL, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 

  pinMode(enR, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

}
 
unsigned long preMillis = millis(); 

int speedPWM = 150; 
int timeThreshold = 100; 

String command = "a"; 
char Direction = 'a'; 
String speedString = "x"; 


void loop() {
   if (Serial.available()) {
    command = Serial.readStringUntil('.');  
    Direction = command[0];
    speedString = command.substring(2, command.length()); 
    speedPWM = speedString.toInt(); 
    preMillis = millis(); 
        switch(Direction)
        {
        case 'f': 
        {
        goForward(speedPWM); 
        timeThreshold = 100; 
        }
        break; 
        case 'b': 
        {
        goBackward(speedPWM); 
        timeThreshold = 100; 
        }
        break; 
        
        case 'l': 
        {
        goLeft(speedPWM);  //Turning half at a speed of going. 
        timeThreshold = 20; 
        }
        break; 
        
        case 'r': 
        {
        goRight(speedPWM); 
        timeThreshold = 20;   //Turning at the half speed of going
        }
        break; 

         default:
       Stop(); 
          
        }
       
   }
   else 
   {
    if(millis() - preMillis > timeThreshold)
    {
      Stop(); 
      preMillis = millis(); 
    }
   }


 
}



void Stop()
{
  digitalWrite(enL, LOW); 
  digitalWrite(enR, LOW); 
}

void rightForward(int speedPWM) //Sets the right motor to move forward
{
  analogWrite(enR, speedPWM); 
   digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW); 
}

void rightBackward(int speedPWM)
{
  analogWrite(enR, speedPWM); 
   digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH); 
}

void leftForward(int speedPWM)
{
analogWrite(enL, speedPWM); 
   digitalWrite(in1, HIGH); 
  digitalWrite(in2, LOW); 
  
}

void leftBackward(int speedPWM)
{
  analogWrite(enL, speedPWM); 
   digitalWrite(in1, LOW); 
  digitalWrite(in2, HIGH); 
}


void goForward(int speedPWM)
{
 rightForward(speedPWM); 
 leftForward(speedPWM); 
}

void goBackward(int speedPWM)
{
  rightBackward(speedPWM); 
  leftBackward(speedPWM); 
}

void goLeft(int speedPWM)
{
 rightForward(speedPWM); 
 leftBackward(speedPWM); 
}


void goRight(int speedPWM)
{
    leftForward(speedPWM); 
    rightBackward(speedPWM); 
}
