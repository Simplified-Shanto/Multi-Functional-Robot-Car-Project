#include<EEPROM.h>           //For saving settings value. 
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

//Define LCD pinout. 

//const int en = 2, rw = 1, rs = 0, d4 = 4, d5 = 5, d6 = 6, d7 = 7, bl = 3; 

//Define I2C address - change if required 
//const int i2c_addr = 0x3F; 

LiquidCrystal_I2C lcd(0x3F, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

#define xTrigger              D3    //This pin will be turned HIGH and the other LOW to read X axis potentiometer's value via A0
#define yTrigger              D0    //This pin will be turned HIGH and the other LOW to read Y axis potentiometer's value via A0

unsigned short xAxisValue() //Function to read X axis potentiometer's value via A0
{ 
  //Allowing only X axis potentiometer's value to be exposed to A0
  digitalWrite(xTrigger, HIGH); 
  digitalWrite(yTrigger, LOW); 
  delay(10); 
  return analogRead(A0); 
}      
unsigned short yAxisValue()
{
  //Allowing only Y axis potentiometer's value to be exposed to A0
  digitalWrite(xTrigger, LOW); 
  digitalWrite(yTrigger, HIGH); 
  delay(10); 

  return analogRead(A0); 
}                                                    

#define stickButton           D6                           //Built in button of joystick
#define buttonValue           digitalRead(stickButton)
//#define xAxisValue()                  analogRead(varX)
//#define yAxisValue()                 analogRead(varY)
#define  noSettings  12                                      //Number of settings
#define scaleFactor 5                                        //Scale factor for scaling data to save and retrieve values to EEPROM.
unsigned short Mode = 0;                                     //0 - Joystick driving mode
                                                             //1 - settings mode
                                                             //2 - Obstacle avoidance mood
short settingsIndex = 0;                            //Which settings we were tinkering. 
String settingsName[noSettings] = {"BF Lower PWM", "BF Upper PWM", "LR Lower PWM", "LR Upper PWM", "Joystick Low", "Joystick High", "OA BF PWM", "OA BF time", "OA LR PWM" , "OA LR time", "Safe Distance", "Save changes?"}; //Name of setting
short settingsValue[noSettings] = {70,        170,             80,          120,              400,              700,             90,         300,        200,          150,           6,                  0       }; //Initial parameters
/*                                      Index: 0         1               2            3                  4                5                6           7           8             9             10                  11      */ //Index
unsigned long lastPressed = millis(); 
String message = "a", Value = "0";                           //For storing serial message of OA mode , L, M, R distance. 
char   Direction = 'M'; 
unsigned short actionValue = 0;  
 
  void serialPrint(char c, int v)                             //for printing message to the Seiral Monitor in the format 'action:value.'
{
    Serial.print(c); 
    Serial.print(':'); 
    Serial.print(v); 
    Serial.print('.');
}


void setup() {
  
  Serial.begin(9600);
  pinMode(stickButton, INPUT_PULLUP); 
  pinMode(xTrigger, OUTPUT); 
  pinMode(yTrigger, OUTPUT); 
  pinMode(A0, INPUT); 
  lcd.begin(16, 2);                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                        
  //Welcome message
  lcd.setCursor(0, 0); 
  lcd.print("Ready to Rock!"); 

  /*   Let's keep this eeprom thing out for now
  for(int i = 0; i<= noSettings -2; i++)
  {
    settingsValue[i] = EEPROM.read(i)*(i==noSettings-2?1:scaleFactor);  //Retrieving data from the EEPROM to our array. 
                                                    //One byte can store 255 at maximum, so when we saved it, we scaled down the value by a factor of 5
                                                    //So here to use them we are scaling it up by 5. 
  }
  */
  delay(1000); 
  lcd.clear();
}



/*
 * Control Characters
 * Forward - 'f'
 * Backward - 'b'
 * Left - 'l'
 * Right - 'r'
 * 
 * Mode setup - 'm' 
 * 
 * Serial command print format - "Command:Value" for example: "f:120." means go forward with a pwm of 120
 */

 /*
  * Debug listing 
  * 1. Disable going front after facing obstacle feature
  * 2. Send front distance after 500 ms. 
  */
 

void loop() {
 
 
  if(buttonValue == 0 && millis() - lastPressed > 200) //Whether the joystick button is pressed - to change the mood. 
  {
   Mode++;  Mode%=3; //Limiting to 0,1,2 modes


   switch(Mode) //Taking action for selected new mode
   {
    case 0: //Joystick Driving mood. 
    {
       Serial.print("m:1."); //Giving the car a command to set it's mode to joystick driving mode
      lcd.clear();  
    }
     break; 
     
    case 1:  //Settings mode
    {
      lcd.clear(); 
       lcd.setCursor(0,1); 
    lcd.print("Settings mode"); 
    delay(600); 
    lcd.clear(); 
    }       
    break; 

    case 2:  //Obstacle avoidance mode
    {
      for(int i = 6; i<=noSettings-1; i++)       //Updating the settings parameters after exiting settings mode each time.
      {
        serialPrint(char('0' + i - 6), settingsValue[i]);  //Sending the changed values of the OA settings to the Car. 
      }
      lcd.clear(); 
      Serial.print("m:2."); //Giving the car a command to set it's mode to 2/oa mode
    }
    
    break;
    default: break; 
   }  
   lastPressed = millis(); 
  }






  if(Mode==2)       //In obstacle avoidance mode. 
  {
      lcd.setCursor(3,0); 
    lcd.print("OA mode"); 
    
    if(Serial.available())
    {
        message = Serial.readStringUntil('.');  
        Direction = message[0]; //The type of action the remote wants us to take. 
        Value = message.substring(2, message.length()); 
        actionValue = Value.toInt(); 

         switch(Direction)
    {
      case 'M': 
      { 
         lcd.setCursor(5, 1); 
         lcd.print("M:"); 
         if((actionValue-1)%10==0 || (actionValue+1)%10==0) { lcd.setCursor(7, 1);  lcd.print("   "); } //Clearing garbage value on the display. 
         lcd.setCursor(7, 1); 
         lcd.print(actionValue); 
      }
      break;
      
      case 'L':
      {
        lcd.setCursor(0, 1); 
         lcd.print("L:"); 
         lcd.setCursor(2, 1); 
         lcd.print(actionValue); 
      }
      break;
      
       case 'R':
      {
        lcd.clear(); //Clearing the display to print Right, Middle and Left distance after facing an obstacle. 
        delay(500); 
        lcd.setCursor(11, 1); 
         lcd.print("R:"); 
         lcd.setCursor(13, 1); 
         lcd.print(actionValue); 
      }
      break;

      default: break; 
      
    }
      
       lcd.setCursor(3,0); 
    lcd.print("OA mode"); 
    } 
  }






    if(Mode==1)               //Settings mode
    {
      if(millis() - lastPressed > 400)
      {
          if( yAxisValue()> 700 ) //Joystick is moved up
          {
            settingsIndex++; 
        settingsIndex%=noSettings; //Constraining the value upto 
        lcd.clear(); 
        lastPressed = millis(); 
          }

          else if(yAxisValue()< 300)
          {
            settingsIndex--; if(settingsIndex==-1) { settingsIndex = noSettings - 1; }
            lcd.clear(); 
            lastPressed = millis(); 
          }
      }

      if(millis() - lastPressed > 200)   //Checking for left-right joystick move, to change the value of the settings. 
      {
         if(xAxisValue()> 700)  //whether the joystick is moved right. 
          {
            if(settingsIndex == noSettings-1)  //The last setting - save changes
            { /*
              for(int i = 0; i <= noSettings - 2; i++) 
              { EEPROM.update(i, settingsValue[i]/(i==noSettings-2?1:scaleFactor)); }  //Updating the first noSettings - 1 bytes of the EEPROM with the values of the settings array. 
              */
              //Printing the success message. 
              lcd.clear(); 
              lcd.setCursor(4, 0); 
              lcd.print("Saved"); 
              lcd.setCursor(0, 1); 
              lcd.print("successfully!:)"); 
              delay(500);
              lcd.clear(); 
            }
            else  //If it's not the save menu. 
            {
           settingsValue[settingsIndex]+=(settingsIndex==noSettings-2?1:scaleFactor);    //If we are changing safe distance limit, change by 1, or change by scale factor. 
           if( (settingsValue[settingsIndex]+1)%10==0 || (settingsValue[settingsIndex]-1)%10==0 ) { lcd.clear(); } //Clearing the probable garbage value left by last small or greater number at 
                                                                                      //that position of the lcd display.
            }
           lastPressed = millis();                                                  
           
          }

          else if(xAxisValue() < 300)   //whether the joystick is moved left. 
          {
           settingsValue[settingsIndex]-=(settingsIndex==noSettings-2?1:scaleFactor);    //If we are changing safe distance limit, change by 1, or change by scale factor. 
           if( (settingsValue[settingsIndex]+1)%10==0 || (settingsValue[settingsIndex]-1)%10==0 ) { lcd.clear(); } //Clearing the probable garbage value left by last small or greater number at 
           lastPressed = millis();                                                    //that position of the lcd display. 
          }
        
      }

    
      //Printing the name of setting and value
      lcd.setCursor(0, 0); 
      lcd.print(settingsName[settingsIndex]); //Name
      lcd.setCursor(6, 1); 
      lcd.print(settingsValue[settingsIndex]); //Value
      lcd.setCursor(0, 1); 
      lcd.print(settingsIndex + 1); //Settings index. 
      lcd.setCursor(2, 1); 
      lcd.print('.'); 
       
    }

  
  
  
   if(Mode==0)  //Whether the controller is in joystck driving mode. 
   {
   lcd.setCursor(0, 0); 
               lcd.print("Driving mode"); 
                /*      The direction of the car is set to be compliant with 2D co-ordinate system considering the actual joystick X and Y axis 
                 *       increment and decrement. The car is considered to be placed along on the origin, having it's head directed towards the postive 
                 *       direction of Y axis. 
                 *       Y++  Go forward
                 *       Y-- Go backward
                 *       X++ Go right
                 *       X-- go left
                 *       
                 * 
                 * 
                 */
    
  if(yAxisValue() < settingsValue[4])
  {
    Serial.print('b'); 
    Serial.print(':'); 
    Serial.print(map(yAxisValue(), settingsValue[4], 0, settingsValue[0], settingsValue[1])); //sending the speed. 
    Serial.print('.'); 
    
  }
  else if(yAxisValue() > settingsValue[5]  )
  {
   Serial.print('f');
    Serial.print(':'); 
    Serial.print(map(yAxisValue(),settingsValue[5], 1023 , settingsValue[0], settingsValue[1])); //sending the speed. 
    Serial.print('.'); 
  }
  /*
   *String settingsName[noSettings] = {"BF Lower PWM", "BF Upper PWM", "LR Lower PWM", "LR Upper PWM", "Joystick Low", "Joystick High"}; //Name of setting
unsigned short settingsValue[noSettings] = {50, 170, 80, 100, 490, 530};  
   *
   */
  else if(xAxisValue() < settingsValue[4] )
  {
    Serial.print('l'); 
    Serial.print(':'); 
    Serial.print(map(xAxisValue(),settingsValue[4], 0 , settingsValue[2], settingsValue[3])); //sending the speed. 
    Serial.print('.'); 
  }
  
  else if( xAxisValue() > settingsValue[5])
  {
    Serial.print('r'); 
    Serial.print(':'); 
    Serial.print(map(xAxisValue(),settingsValue[5], 1023 , settingsValue[2], settingsValue[3])); //sending the speed. 
    Serial.print('.'); 
    
  }
  }
  
  }
