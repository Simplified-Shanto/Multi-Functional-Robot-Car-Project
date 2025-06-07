#include <IRremote.h> 
int RECV_PIN = 4; //IR receiver's signal pin 
IRrecv irrecv(RECV_PIN);
decode_results results;



//Remote's pin declaration
#define F 16718055  //Forward - 2 
#define B 16730805  //Backward - 8
#define L 16716015  //Left - 4
#define R 16734885  //Right - 6
#define longPress 4294967295  //repeat sequence


//Pin declaration for left motor. 
const int enL = 10; 
const int in1 = 12; 
const int in2 = 11; 

//Pin declaration for right motor. 
const int enR = 9; 
const int in3 = 7; 
const int in4 = 8; 


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







void setup() {
  
  pinMode(enL, OUTPUT); 
  pinMode(in1, OUTPUT); 
  pinMode(in2, OUTPUT); 

  pinMode(enR, OUTPUT); 
  pinMode(in3, OUTPUT); 
  pinMode(in4, OUTPUT); 

 irrecv.enableIRIn();
}


unsigned long val; 
unsigned long lastKey = longPress; 
unsigned long preMillis = millis(); 

int speedPWM = 150; 
int timeThreshold = 100; 
//Development Agenda
/* Add speed controlling system 
 *  Massive test on direction and tuning. 
 */

void loop() {
  
   if (irrecv.decode(&results)) {
    preMillis = millis(); 
    val = results.value; 
    
        irrecv.resume(); 

        if(val==longPress)//If it's a repeat sequence, we change val to the last valid key value
        {
        val = lastKey; 
        }
        else 
        {
          lastKey = val; 
        }

        
        switch(val)
        {
        case F: 
        {
       goForward(speedPWM); 
        timeThreshold = 250; 
        }
        break; 
        
        case B: 
        {
        goBackward(speedPWM); 
        timeThreshold = 250; 
        }
        break; 
        
        case L: 
        {
        goLeft(150); 
        timeThreshold = 80; 
        }
        break; 
        
        case R: 
        {
        goRight(150); 
        timeThreshold = 80; 
        }
        break; 

         default:
         break;  
          
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
