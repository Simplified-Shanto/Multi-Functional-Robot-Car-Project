#define varY A0
#define varX A1

void setup() {
  Serial.begin(9600); 

}

/*
 * Control Characters
 * Forward - 'f'
 * Backward - 'b'
 * Left - 'l'
 * Right - 'r'
 */
int yVal; 
int xVal; 
int lowVal = 490; //The value of the joystick for which we start to move. 
int highVal = 530; 

void loop() {
  xVal = analogRead(varX); 
  yVal = analogRead(varY); 
  
  if(xVal > highVal )
  {
    Serial.print('f');
    Serial.print(':'); 
    Serial.print(map(xVal, highVal, 1023, 50, 200)); //sending the speed. 
    Serial.print('.'); 
    
  }
  else if(xVal < lowVal )
  {
   Serial.print('b');
    Serial.print(':'); 
    Serial.print(map(xVal, lowVal, 0, 50, 200)); //sending the speed. 
    Serial.print('.'); 
  }
  else if(yVal > highVal)
  {
    Serial.print('r'); 
    Serial.print(':'); 
    Serial.print(map(yVal, highVal, 1023, 80, 120)); //sending the speed. 
    Serial.print('.'); 
  }
  else if(analogRead(varY) < lowVal)
  {
    Serial.print('l'); 
    Serial.print(':'); 
    Serial.print(map(yVal, lowVal, 0, 80, 120)); //sending the speed. 
    Serial.print('.'); 
    
  }
  }
