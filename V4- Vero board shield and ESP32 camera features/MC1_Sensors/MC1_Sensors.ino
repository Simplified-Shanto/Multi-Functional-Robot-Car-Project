#include <DHT_U.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>
#include <MQUnifiedsensor.h>
#include <string.h> 


/*
Hardware connections. Board; Arduino Nano. 
Analog Pins. 
-------------------------------------
A0   
A1   ldr 
A2   MQ4 output.
A3   DHT22 output. 
A4   SDA of BMP280
A5   SCL of BMP280
A6
A7

Digital Pins (PWM 3, 5, 6, 9, 10, 11)
-------------------------------------
D0(TX)     Software Serial RX A1 of arduino Uno
D1(RX)     Software Serial TX of A2 of Arduino Uno
D2
~D3
D4         Transistor switching pin. 
~D5
~D6
D7
D8
~D9
~D10
~D11
D12
D13

#define ldrPin A1


*/
// Use Ctrl+R to compile code. 

#define BMP_SCK (13)
#define BMP_MISO (12)
#define BMP_MOSI (11)
#define BMP_CS (10)
Adafruit_BMP280 bmp;  // I2C


#define powerPin 4
#define ldrPin A1
#define DHTPIN A3  // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment the type of sensor in use:
//#define DHTTYPE    DHT11     // DHT 11
#define DHTTYPE DHT22  // DHT 22 (AM2302)
//#define DHTTYPE    DHT21     // DHT 21 (AM2301)
// See guide for details on sensor wiring and usage:
//   https://learn.adafruit.com/dht/overview

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;



/************************Hardware Related MQ4 Macros************************************/
#define Board ("Arduino Nano")
#define Pin (A2)  //Analog input 4 of your arduino
/***********************Software Related MQ4 Macros************************************/
#define Type ("MQ-4")  //MQ4
#define Voltage_Resolution (5)
#define ADC_Bit_Resolution (10)  // For arduino UNO/MEGA/NANO
#define RatioMQ4CleanAir (4.4)   //RS / R0 = 60 ppm
/*****************************Globals***********************************************/
//Declare Sensor
MQUnifiedsensor MQ4(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);


void initializeSensors()
{
    // Initialize humidity and temperature sensor. .
  dht.begin();
  // Print temperature sensor details.
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
 

  bmp.begin();
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  //Set math model to calculate the PPM concentration and the value of constants
  MQ4.setRegressionMethod(0);  //_PPM =  pow(10, (log10(ratio)-b)/a)
  MQ4.init();
  //MQ-4 calibration
  float calcR0 = 0;
  for (int i = 1; i <= 10; i++) {
    MQ4.update();  // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ4.calibrate(RatioMQ4CleanAir);
  }
  MQ4.setR0(calcR0 / 10);

  //MQ-4 calibration error warnings.
  if (isinf(calcR0)) { Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); }
  if (calcR0 == 0) { Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); }
}

void setup() {
  Serial.begin(600);
  pinMode(powerPin, OUTPUT);
  digitalWrite(powerPin, LOW);
 // delay(1000);  What if we do not initialize in the setup at all? 
 // initializeSensors(); 
  //digitalWrite(powerPin, LOW); 
  
}


byte sensorOn = 0; //Whether sensor power is on or off. 
long long lastReadTime = millis(); //Last time in millis we accessed the sensor reading. 
#define readingInterval  5000   //The interval between successive readings in millisecond. 
String sensorReadings = ""; 

void loop() {
  // Delay between measurements.
 // delay(3000);
  if (Serial.available()) {
    char command;
    command = Serial.read();
    if (command == '1')
     {
      digitalWrite(powerPin, HIGH);
      delay(1000); //Having a short delay before initializing the sensors. 
      sensorOn = 1; 
      initializeSensors(); 
      delay(1000); 
     } 
    else if (command == '0') 
    {
      digitalWrite(powerPin, LOW);
      sensorOn = 0; 
    }
    //Serial.println("Got serial command!"); 
  }

  if(sensorOn==1)
  {
     if(millis() - lastReadTime > readingInterval)
     {
        lastReadTime = millis(); 
        //initializing the reading containing string. 
            sensorReadings = ""; 
            // Get temperature event and print its value.

                  sensors_event_t event;
                  dht.temperature().getEvent(&event);
                  // Get humidity event and print its value.
                  dht.humidity().getEvent(&event);
                  if (isnan(event.relative_humidity)) {
                    //Serial.print(F("ReadError"));
                    sensorReadings+="ReadError"; 
                  } else {
                    //Sending humidity reading from DHT22
                      // Serial.print(event.relative_humidity);
                      // Serial.print(F("%"));
                      // Serial.print("|"); 
                      sensorReadings+= String(event.relative_humidity) + "%|"; 
                  }
                  //Sending BMP280 temperature reading
                    // Serial.print(bmp.readTemperature());
                    // Serial.print(F("°C"));
                    // Serial.print("|"); 
                    sensorReadings+= String(bmp.readTemperature()) + "°C|"; 
                   
                  
                  
                //Sending BMP280 barometric pressure reading;
                  // Serial.print(bmp.readPressure());
                  // Serial.print(" Pa");
                  // Serial.print("|"); 
                   sensorReadings+= String(bmp.readPressure()) + "Pa|"; 



                  //MQ4 reading part.
                  MQ4.update();  // Update data, the arduino will read the voltage from the analog pin
                  MQ4.setA(-0.318);
                  MQ4.setB(1.133);               // A -> Slope, B -> Intersect with X - Axis
                  // Serial.print(MQ4.readSensor());
                  // Serial.print(" ppm");
                  // Serial.print("|"); 
                   sensorReadings+= String(MQ4.readSensor()) + " ppm|"; 

                  //Sending LDR reading. 
                  //Serial.print(analogRead(A0));
                  //Serial.print('!'); //Terminating character of sensor reading sending. 
                  sensorReadings+= String(analogRead(ldrPin)) + "!"; 
                  
                   Serial.print(sensorReadings); 
     }

  }
 
  
}
