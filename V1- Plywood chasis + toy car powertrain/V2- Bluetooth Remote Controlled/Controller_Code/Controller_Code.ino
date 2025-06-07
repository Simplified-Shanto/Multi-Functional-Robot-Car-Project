

#include <LiquidCrystal.h>
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

#define varY                  A0  //X axis of Joystick
#define varX                  A1  //Y axis of Joystick
#define stickButton           A2  //Built in button of joystick
#define buttonValue           digitalRead(stickButton)
#define xVal                  analogRead(varX)
#define yVal                  analogRead(varY)

void setup() {
  
  Serial.begin(9600);
  pinMode(stickButton, INPUT_PULLUP); 
  lcd.begin(16, 2);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
  //Welcome message
  lcd.setCursor(0, 0); 
  lcd.print("Ready to Rock!"); 
  delay(1000); 
  lcd.clear(); 
}



/*
 * Control Characters
 * Forward - 'f'
 * Backward - 'b'
 * Left - 'l'
 * Right - 'r'
 */
 
unsigned short Mode = 0; //0 - driving mode
                       //1 - settings mode

unsigned short settingsIndex = 0; //Which settings we were tinkering. 

#define  noSettings  6  //Number of settings
String settingsName[noSettings] = {"BF Lower PWM", "BF Upper PWM", "LR Lower PWM", "LR Upper PWM", "Joystick Low", "Joystick High"}; //Name of setting
unsigned short settingsValue[noSettings] = {50, 170, 80, 100, 490, 530}; 
unsigned short lastVal = 0; //Checking whether we need to clear the display 
unsigned long lastPressed = millis(); 

void loop() {
 
 
  if(buttonValue ==0 && millis() - lastPressed > 200)
  {
   Mode = 1 - Mode; 
   
   if(Mode==1)
   {
     lcd.clear(); 
       lcd.setCursor(0,1); 
    lcd.print("Settings mode"); 
    delay(1000); 
   }

     lcd.clear(); 
   lastPressed = millis(); 
  }


    if(Mode==1)
    {
      if(millis() - lastPressed > 400)
      {
          if( yVal > 700 ) //Joystick is moved up
          {
            settingsIndex++; 
        settingsIndex%=noSettings; //Constraining the value upto 
        lcd.clear(); 
        lastPressed = millis(); 
          }

          else if(yVal < 200)
          {
            settingsIndex--; if(settingsIndex==-1) { settingsIndex = noSettings - 1; }
            lcd.clear(); 
            lastPressed = millis(); 
          }
      }

      if(millis() - lastPressed > 200)
      {
         if(  xVal < 200) //Joystick is moved up
          {
           lastVal = settingsValue[settingsIndex]/10; 
           settingsValue[settingsIndex]++; 

            if(abs(lastVal - settingsValue[settingsIndex]/10) >= 1) { lcd.clear(); }
    
        lastPressed = millis(); 
          }
          

          else if(xVal > 700)
          {
             lastVal = settingsValue[settingsIndex]/10; 
           settingsValue[settingsIndex]--; 

            if(abs(lastVal - settingsValue[settingsIndex]/10) >= 1) { lcd.clear(); }
            lastPressed = millis(); 
          }
        
      }

    
     

      //Printing the name of setting and value
      lcd.setCursor(0, 0); 
      lcd.print(settingsName[settingsIndex]); //Name
      lcd.setCursor(6, 1); 
      lcd.print(settingsValue[settingsIndex]); //Value
      lcd.setCursor(0, 1); 
      lcd.print(settingsIndex + 1); //Settings index. 
      lcd.setCursor(1, 1); 
      lcd.print('.'); 
       
    }

  
  
  
   if(Mode==0)  //Whether the controller is in driving mode. 
   {
   lcd.setCursor(0, 0); 
               lcd.print("Driving mode"); 
    
  if(yVal > settingsValue[5] )
  {
    Serial.print('f');
    Serial.print(':'); 
    Serial.print(map(yVal, settingsValue[5], 1023, settingsValue[0], settingsValue[1])); //sending the speed. 
    Serial.print('.'); 
    
  }
  else if(yVal < settingsValue[4] )
  {
   Serial.print('b');
    Serial.print(':'); 
    Serial.print(map(yVal, settingsValue[4], 0, settingsValue[0], settingsValue[1])); //sending the speed. 
    Serial.print('.'); 
  }
  /*
   *String settingsName[noSettings] = {"BF Lower PWM", "BF Upper PWM", "LR Lower PWM", "LR Upper PWM", "Joystick Low", "Joystick High"}; //Name of setting
unsigned short settingsValue[noSettings] = {50, 170, 80, 100, 490, 530};  
   *
   */
  else if(xVal > settingsValue[5])
  {
    Serial.print('r'); 
    Serial.print(':'); 
    Serial.print(map(xVal, settingsValue[5], 1023, settingsValue[2], settingsValue[3])); //sending the speed. 
    Serial.print('.'); 
  }
  
  else if(xVal < settingsValue[4])
  {
    Serial.print('l'); 
    Serial.print(':'); 
    Serial.print(map(xVal, settingsValue[4], 0, settingsValue[2], settingsValue[3])); //sending the speed. 
    Serial.print('.'); 
    
  }

  
  }
  
  }
