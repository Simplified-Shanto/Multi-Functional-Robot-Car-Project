#include <WiFi.h>
#include "file.h"
#include "camera.h"
#include "lapse.h"

// //Iqtiar's phone
// const char *ssid = "Redmi 10C";
// const char *password = "00001111";

const char* ssid = "Shanto's Note 11";
const char* password = "1234abcdefgh";

void startCameraServer();

void setup()
{
	Serial.begin(115200);
	Serial.setDebugOutput(true);
	Serial.println();
	initFileSystem();
	initCamera();

	WiFi.begin(ssid, password);


//The mechanism below, forces the esp to restart by itself 
//until it connects to the wi-fi. 
//We implemented it because, we observed that sometimes it kind of frozen and doesn't connect
//to the wi-fi network until we press the reset button manually, it's the same thing, but the
//ESP does it itself. 

byte connectionCounter = 0; 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.print(".");
    connectionCounter++; 
    if(connectionCounter==6)
    {
      esp_restart(); 
      connectionCounter=0; 
    }
  }


	Serial.println("");
	Serial.println("WiFi connected");
	startCameraServer();
	Serial.print("Camera Ready! Use 'http://");
	Serial.print(WiFi.localIP());
	Serial.println("' to connect");
}

void loop()
{
	unsigned long t = millis();
	static unsigned long ot = 0;
	unsigned long dt = t - ot;
	ot = t;
	processLapse(dt);
}
