//Common enable pin for Enable1 and Enable2;
//Speed of 2 right wheels and 2 left wheels is controlled by this common pin. 
#define commonEnablePin 3  

//Pin declaration for left motor. 
#define in1   16
#define in2  19

//Pin declaration for right motor. 
#define in3  17
#define in4  20



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
