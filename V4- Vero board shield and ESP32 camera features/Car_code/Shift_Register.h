//Pin declaration for the 74HC595 shift register
#define latchPin 8  //Pin 12 of 74HC595 shift register
#define clockPin 4  //Pin 11 of the 74HC595 shift register
#define dataPin  2 //Pin 14 of the 74HC595 shift register


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
