//Board: Esp8266
//Objective: To get to know the useful functionalities of OLED display. 
//To implement a simple counter on the oled. 
#include <iostream> 
#include <Wire.h> 
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h> 
#include "DisplayFunctionality.h"

#include <Fonts/FreeSerif9pt7b.h> 

#define SCREEN_WIDTH  128 //OLED display width, in pixels. 
#define SCREEN_HEIGHT 64  //OLED display height, in pixels. 

//Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1); 

int Seconds = 0; 
long lastMillis = millis(); 

void setup()
{
  Serial.begin(9600); 
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) //Address of our 128x64 display is 0x3C
  {
    Serial.println(F("SSD1306 allocation failed"));  //By wrapping the output string inside F() we store the string in the program flash memory instread of SRAM. 
    for(;;); //Keep looping forever. 
  }

  delay(2000); //waiting for the display to set up itself 

  OLED.clearDisplay(); 
  OLED.setFont(&FreeSerif9pt7b); 
  OLED.setTextSize(1); 
  OLED.setTextColor(WHITE); 
  OLED.setCursor(30,20); 
  //DISPLAY static text
  OLED.println("Hi!"); 
  OLED.display(); 
  delay(1000); 

  OLED.clearDisplay(); 

  

}

String command = "nothingYet";
char commandType = 'b'; 
String commandValueString; 
int commandValue = 0; 
int millisDividend = 1000; 
int setCursorX = 0; 
int setCursorY = 0; 
//Command format: 'x:y.' where x is the type of command and y is the value.  
//s:x.    -> means set fontsize to x. 
//d:x.    -> set the millis() dividend to x. 
//c:x.    -> column value of setCursor()
//r:x.    -> row value of setCursor()
void loop()
{
 
  OLED.clearDisplay(); 
   delay(1000); 
 // OLED.invertDisplay(false); 
  OLED.setTextSize(1); 
  OLED.setCursor(45, 15); 
  OLED.println("Project");
  OLED.setCursor(25,30); 
  OLED.println("MFRC"); 
  OLED.setCursor(40,50);
 // OLED.println("LOOP");

  OLED.display(); 
  delay(4000); 
  

  OLED.clearDisplay(); 
  /*
  OLED.drawBitmap(0,0, ndcLogo, 128, 64, WHITE); 
  OLED.invertDisplay(true); 
  OLED.display(); 
  delay(4000); 
  */
  // OLED.invertDisplay(false); 
  OLED.setTextSize(1); 
  OLED.setCursor(20, 15); 
  OLED.println("Love for");
  OLED.setCursor(10,30); 
  OLED.println("Bangladesh"); 
  OLED.setCursor(40,50);
  //OLED.println("LOOP");
  delay(4000); 

}