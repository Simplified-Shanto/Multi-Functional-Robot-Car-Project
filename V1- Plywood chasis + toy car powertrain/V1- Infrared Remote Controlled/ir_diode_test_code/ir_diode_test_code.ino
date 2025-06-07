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


void loop()
{
  Serial.print("Value = "); 
  Serial.print(analogRead(A3)); 
  Serial.print("\n"); 
  delay(500); 
}
